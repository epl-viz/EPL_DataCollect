/* Copyright (c) 2017, EPL-Vizards
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the EPL-Vizards nor the
 *       names of its contributors may be used to endorse or promote products
 *       derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL EPL-Vizards BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*!
 * \file CaptureInstance.cpp
 * \brief Contains class CaptureInstance
 */

#include "CaptureInstance.hpp"
#include "EPLEnum2Str.hpp"
#include "PluginBase.hpp"
#include "PluginManager.hpp"
#include <capchild/capture_session.h>
#include <caputils/capture_ifinfo.h>
#include <epan/prefs.h>
#include <iostream>
#include <ws_capture.h>
#include <ws_dissect.h>

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
// TODO include windows alternatives
#else
#include <arpa/inet.h>
#include <netinet/in.h>
#endif

#if __cplusplus <= 201402L
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#else
#include <filesystem>
namespace fs = std::filesystem;
#endif

namespace EPL_DataCollect {

CaptureInstance::~CaptureInstance() {
  if (state == RUNNING)
    stopRecording();

  iHandler.stopLoop();

  if (dissect) {
    iHandler.setDissector(nullptr);
    ws_dissect_free(dissect);
    dissect = nullptr;
  }

  if (capture) {
    ws_capture_close(capture);
    capture = nullptr;
  }

  pluginManager.reset(this);
}

/*!
 * \brief Sets up Plugins, Cycle Builder, etc
 * \returns 0   on success
 * \returns -1  when the state of the CI is not SETUP
 * \returns 1   on PluginManager error; THIS SETS THE STATE TO ERRORED
 * \returns 100 when unable to start the build loop; THIS SETS THE STATE TO ERRORED
 */
CaptureInstance::CIErrorCode CaptureInstance::setupLoop() {
  std::lock_guard<std::recursive_mutex> lock(accessMutex);
  if (!pluginManager.init(this))
    return errorCleanup(FAILED_TO_INITIALISE_PLUGINS);

  if (dissect == nullptr)
    return errorCleanup(WIRESHARK_FAILED_TO_DISSECT_CAPTURE);

  prefs_reset();
  for (auto i : nodeCfg) {
    if (!i.second.autoDeduceSpecificProfile && !i.second.specificProfile.empty()) {
      if (i.first >= 240)
        continue;

      std::string cfgStr = "uat:epl_nodeid_profiles:\"";
      cfgStr += std::to_string(static_cast<int>(i.first));
      cfgStr += "\",\"";
      cfgStr += cfg.xddDir + "/" + i.second.specificProfile + "\"";
      char *      errStr = nullptr;
      std::string errStrSTD;
      auto        ret = prefs_set_pref(const_cast<char *>(cfgStr.c_str()), &errStr);
      if (errStr) {
        errStrSTD = errStr;
        g_free(errStr);
      }
      std::cout << "[CaptureInstance] dissector pref: '" << cfgStr << "' --> " << EPLEnum2Str::toStr(ret) << " ("
                << errStrSTD << ")" << std::endl;
    }
  }
  prefs_apply_all();

  startCycle.cycleNum = UINT32_MAX; // Build from start

  iHandler.setDissector(dissect);
  iHandler.startLoop();

  if (!builder.startLoop(startCycle)) {
    std::cerr << "[CaptureInstance] (setupLoop) Failed to start build loop" << std::endl;
    return errorCleanup(FAILED_TO_START_BUILD_LOOP);
  }

  state = RUNNING;
  return OK;
}

CaptureInstance::CIErrorCode CaptureInstance::errorCleanup(CIErrorCode retVal) {
  builder.stopLoop();
  iHandler.stopLoop();

  if (dissect) {
    ws_dissect_free(dissect);
    iHandler.setDissector(nullptr);
    dissect = nullptr;
  }

  if (capture) {
    ws_capture_close(capture);
    capture = nullptr;
  }

  state = ERRORED;
  return retVal;
}


/*!
 * \brief starts recording on the specified ethernet device
 *
 * A list of available devices can be obtained with getDevices()
 * \param  interfaceSTR The network device to use for the live capture
 * \sa setupLoop for return values
 */
CaptureInstance::CIErrorCode CaptureInstance::startRecording(std::string interfaceSTR) noexcept {
  std::lock_guard<std::recursive_mutex> lock(accessMutex);

  if (state != SETUP) {
    std::cerr << "[CaptureInstance] (startRecording) Invalid state " << EPLEnum2Str::toStr(state) << std::endl;
    return INVALID_CI_STATE;
  }

  auto list  = getDevices();
  bool found = false;
  for (auto const &i : list) {
    if (i == interfaceSTR) {
      found = true;
      break;
    }
  }

  if (!found) {
    std::cerr << "[CaptureInstance] (startRecording) Interface '" << interfaceSTR << "' does not exist!" << std::endl;
    return INTERFACE_DOES_NOT_EXIST;
  }

  int   flags   = 0;
  int   err     = 0;
  char *errInfo = nullptr;

  if (cfg.enableHardwareTimestamps) {
    flags = WS_CAPTURE_TSTAMP_ADAPTER_UNSYNCED;
  }

  capture = ws_capture_open_live(interfaceSTR.c_str(), flags, nullptr, &err, &errInfo);
  if (capture == nullptr) {
    if (errInfo) {
      std::cerr << "[CaptureInstance] ws_capture_open_live returned " << err << " (" << errInfo << ")" << std::endl;
      g_free(errInfo);
    } else {
      std::cerr << "[CaptureInstance] ws_capture_open_live returned " << err << std::endl;
    }
    return FAILED_TO_CAPTURE_ON_INTERFACE;
  }

  dissect = ws_dissect_capture(capture);
  if (dissect == nullptr) {
    ws_capture_close(capture);
    return WIRESHARK_FAILED_TO_DISSECT_CAPTURE;
  }

  return setupLoop();
}


/*!
 * \brief ends the live capture
 * \returns 0  on success; the new state is now DONE
 * \returns -1 when the current state is not RUNNING
 * \returns 1  on PluginManager error; THIS SETS THE STATE TO ERRORED
 * \returns 2  when the build loop could not be stopped; THIS SETS THE STATE TO ERRORED
 */
CaptureInstance::CIErrorCode CaptureInstance::stopRecording() noexcept {
  std::lock_guard<std::recursive_mutex> lock(accessMutex);

  if (state != RUNNING) {
    std::cerr << "[CaptureInstance] (stopRecording) Invalid state " << EPLEnum2Str::toStr(state) << std::endl;
    return INVALID_CI_STATE;
  }

  builder.stopLoop();

  state = DONE;
  return OK;
}


/*!
 * \brief loads a previously captured PCAP
 * \param  file The file to load
 * \returns 0  on success
 * \returns 10 if the file does not exists
 * \returns 11 if the path is not a file
 * \returns 12 if the wireshark failed to open the capture
 * \returns 13 if the wireshark dissector failed to load the capture
 * \sa setupLoop for return values
 */
CaptureInstance::CIErrorCode CaptureInstance::loadPCAP(std::string file) noexcept {
  std::lock_guard<std::recursive_mutex> lock(accessMutex);

  if (state != SETUP) {
    std::cerr << "[CaptureInstance] (loadPCAP) Invalid state " << EPLEnum2Str::toStr(state) << std::endl;
    return INVALID_CI_STATE;
  }

  fs::path filePath(file);

  if (!fs::exists(filePath)) {
    return FILE_DOES_NOT_EXIST;
  }

  if (!fs::is_regular_file(filePath)) {
    return NOT_A_REGULAR_FILE;
  }

  int   err     = 0;
  char *errInfo = nullptr;
  capture       = ws_capture_open_offline(file.c_str(), 0, &err, &errInfo);
  if (capture == nullptr) {
    if (errInfo) {
      std::cerr << "[CaptureInstance] ws_capture_open_offline returned " << err << " (" << errInfo << ")" << std::endl;
      g_free(errInfo);
    } else {
      std::cerr << "[CaptureInstance] ws_capture_open_offline returned " << err << std::endl;
    }
    return FAILED_TO_LOAD_FILE;
  }

  fileSize = fs::file_size(filePath);

  dissect = ws_dissect_capture(capture);
  if (dissect == nullptr) {
    ws_capture_close(capture);
    return WIRESHARK_FAILED_TO_DISSECT_CAPTURE;
  }

  return setupLoop();
}


/*!
 * \brief Returns a list of available network devices
 * \return std::vector<std::string>
 */
std::vector<std::string> CaptureInstance::getDevices() noexcept {
  std::lock_guard<std::recursive_mutex> lock(accessMutex);
  std::vector<std::string>              devList;
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32)
// TODO
#else
  GList *interf;
  int    err;
  char * errInfo;

  interf = capture_interface_list(&err, &errInfo, nullptr);

  if (!interf) {
    if (err == 0)
      std::cerr << "[CaptureInstance] (getDevices) There are no interfaces on which a capture can be done" << std::endl;
    else {
      std::cerr << "[CaptureInstance] (getDevices) " << errInfo << std::endl;
      g_free(errInfo);
    }
    return devList;
  }

  do {
    if_info_t *if_info = reinterpret_cast<if_info_t *>(interf->data);
    devList.emplace_back(if_info->name);

  } while ((interf = interf->next));

  free_interface_list(interf);
#endif
  return devList;
}

std::string CaptureInstance::getCurrentFilePath() noexcept {
  if (!capture)
    return "";

  return ws_capture_filename(capture);
}


/*!
 * \brief Returns a pointer to the EventLog
 * \note The pointer is valid for the entire lifetime of the CaptureInstance instance
 * \return The pointer to the EventLog
 */
EventLog *CaptureInstance::getEventLog() noexcept { return &eventLog; }


/*!
 * \brief Returns a pointer to the PluginManager
 * \note The pointer is valid for the entire lifetime of the CaptureInstance instance
 * \return The pointer to the PluginManager
 */
PluginManager *CaptureInstance::getPluginManager() noexcept { return &pluginManager; }


/*!
 * \brief Returns a pointer to the CycleContainer
 * \note The pointer is valid for the entire lifetime of the CaptureInstance instance
 * \return The pointer to the CycleContainer
 */
CycleContainer *CaptureInstance::getCycleContainer() noexcept { return &cycleContainer; }

/*!
 * \brief Returns the current state of the CaptureInstance
 */
CaptureInstance::CIstate CaptureInstance::getState() noexcept { return state; }

/*!
 * \brief Returns a pointer to the CycleBuilder
 * \note The pointer is valid for the entire lifetime of the CaptureInstance instance
 * \return  The pointer to the CycleBuilder
 */
CycleBuilder *CaptureInstance::getCycleBuilder() noexcept { return &builder; }

/*!
 * \brief Returns a pointer to the SnapshotManager
 * \note The pointer is valid for the entire lifetime of the CaptureInstance instance
 * \return The pointer to the SnapshotManager
 */
SnapshotManager *CaptureInstance::getSnapshotManager() noexcept { return &snapshotManager; }

/*!
 * \brief Returns a pointer to the InputHandler
 * \note The pointer is valid for the entire lifetime of the InputHandler instance
 * \return The pointer to the InputHandler
 */
InputHandler *CaptureInstance::getInputHandler() noexcept { return &iHandler; }

/*!
 * \brief Returns a pointer to the start cycle
 */
Cycle *CaptureInstance::getStartCycle() noexcept { return &startCycle; }


/*!
 * \brief Returns the current configuration of the COMPLETE capture instance
 * \note This includes all child classes (InputHandler, ect.)
 */
CaptureInstance::Config CaptureInstance::getConfig() const noexcept {
  Config newCfg   = cfg;
  newCfg.smConfig = snapshotManager.getConfig();
  newCfg.ihConfig = iHandler.getConfig();
  return newCfg;
}

/*!
 * \brief sets the config of the ENTIRE capture instance
 * \note This includes all child classes (InputHandler, ect.)
 * \param newCfg the new config
 */
void CaptureInstance::setConfig(CaptureInstance::Config newCfg) noexcept {
  cfg = newCfg;
  iHandler.setConfig(cfg.ihConfig);
  snapshotManager.setConfig(cfg.smConfig);
}


/*!
 * \brief Returns the current default node configuration
 * This node configuration will be used for ALL autodetected nodes
 */
CaptureInstance::NodeConfig CaptureInstance::getDefaultNodeConfig() const noexcept { return defaultNodeCfg; }

/*!
 * \brief sets the default node configuration
 * This node configuration will be used for ALL autodetected nodes
 */
void CaptureInstance::setDefaultNodeConfig(CaptureInstance::NodeConfig newCfg) noexcept { defaultNodeCfg = newCfg; }


/*!
 * \brief Returns the configuration of one node
 * \param node   the ID of the node
 * \warning If the node was not set yet, then the node will be initialized with the CURRENT default node config
 */
CaptureInstance::NodeConfig CaptureInstance::getNodeConfig(uint8_t node) noexcept {
  if (nodeCfg.find(node) == nodeCfg.end())
    nodeCfg[node] = defaultNodeCfg;

  return nodeCfg[node];
}

/*!
 * \brief Sets the node configuration of one node
 * \param node   the ID of the node
 * \param newCfg the new config
 */
void CaptureInstance::setNodeConfig(uint8_t node, CaptureInstance::NodeConfig newCfg) noexcept {
  nodeCfg[node] = newCfg;
}

/*!
 * \brief Returns the size of the current file
 * \note This may not be equivalent to the file size on the disk!
 */
uint64_t CaptureInstance::getFileSize() noexcept {
  std::lock_guard<std::recursive_mutex> lock(accessMutex);

  return fileSize;
}

/*!
 * \brief Returns the current file processing offset
 *
 * The offset represents how much of the file has been processed.
 */
uint64_t CaptureInstance::getCurrentFileProcessingOffset() noexcept {
  std::lock_guard<std::recursive_mutex> lock(accessMutex);
  return builder.getMaxProcessingOffset();
}
}
