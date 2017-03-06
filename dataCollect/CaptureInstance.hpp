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
 * \file CaptureInstance.hpp
 * \brief Contains class CaptureInstance
 */

#pragma once

#include "defines.hpp"

#include "Cycle.hpp"
#include "CycleBuilder.hpp"
#include "CycleContainer.hpp"
#include "EventLog.hpp"
#include "InputHandler.hpp"
#include "PluginManager.hpp"
#include "SnapshotManager.hpp"
#include <memory>
#include <mutex>
#include <unordered_map>
#include <vector>

extern "C" {
typedef struct ws_capture_t ws_capture_t;
typedef struct ws_dissect_t ws_dissect_t;
}

namespace EPL_DataCollect {

/*!
  * class CaptureInstance
  * \brief Root class for the EPL-Viz backend
  * \note One CaptureInstance can run EXACTLY ONE capture / PCAP
  *
  * The Capture instance has 4 states:
  *  - SETUP   the capture has not run yet
  *  - RUNNING the capture is running
  *  - DONE    the Capture is complete / the file completely parsed; the CaptureInstance can no longer be used
  *  - ERRORED the CaptureInstance can no longer be used
  *
  * This class handles all relevant data necessary to process the ePL Packets
  */
class CaptureInstance {
 public:
  enum CIstate { SETUP, RUNNING, DONE, ERRORED };

  struct Config {
    SnapshotManager::Config smConfig;
    InputHandler::Config    ihConfig;

    std::string xddDir = constants::EPL_DC_BUILD_DIR_ROOT + "/xdd";
  };

  struct NodeConfig {
    bool        autoDeduceSpecificProfile = true;
    std::string baseProfile               = "302.xdd";
    std::string specificProfile           = "";
  };

 private:
  SnapshotManager snapshotManager;
  PluginManager   pluginManager;
  EventLog        eventLog;
  CycleContainer  cycleContainer;

  InputHandler iHandler;
  CycleBuilder builder;

  std::recursive_mutex accessMutex;
  CIstate              state = SETUP;

  ws_capture_t *capture = nullptr;
  ws_dissect_t *dissect = nullptr;

  Config     cfg;
  NodeConfig defaultNodeCfg;
  std::unordered_map<uint8_t, NodeConfig> nodeCfg;

  Cycle startCycle;

  mockable int setupLoop();

 public:
  CaptureInstance() : cycleContainer(this), builder(this) { cfg = getConfig(); }
  virtual ~CaptureInstance();

  CaptureInstance(const CaptureInstance &) = delete;
  CaptureInstance(CaptureInstance &&)      = delete;

  CaptureInstance &operator=(const CaptureInstance &) = delete;
  CaptureInstance &operator=(CaptureInstance &&) = delete;


  mockable int startRecording(std::string interface) noexcept;
  mockable int stopRecording() noexcept;
  mockable int loadPCAP(std::string file) noexcept;

  template <class C, class... ARGS>
  inline bool registerCycleStorage(std::string index, ARGS... args);

  mockable std::vector<std::string> getDevices() noexcept;

  mockable EventLog *getEventLog() noexcept;
  mockable SnapshotManager *getSnapshotManager() noexcept;
  mockable PluginManager *getPluginManager() noexcept;
  mockable CycleContainer *getCycleContainer() noexcept;
  mockable InputHandler *getInputHandler() noexcept;

  mockable CycleBuilder *getCycleBuilder() noexcept;

  mockable CIstate getState() noexcept;
  mockable Cycle *getStartCycle() noexcept;

  mockable Config getConfig() const noexcept;
  mockable void setConfig(Config newCfg) noexcept;

  mockable NodeConfig getDefaultNodeConfig() const noexcept;
  mockable void setDefaultNodeConfig(NodeConfig newCfg) noexcept;

  mockable NodeConfig getNodeConfig(uint8_t node) noexcept;
  mockable void setNodeConfig(uint8_t node, NodeConfig newCfg) noexcept;
};


/*!
 * \brief Adds a new cycle storage entry
 * Returns false when the index is already occupied
 * TEMPLATED
 * \return bool
 * \param  index The new index to register
 */
template <class C, class... ARGS>
bool CaptureInstance::registerCycleStorage(std::string index, ARGS... args) {
  if (state != SETUP)
    return false;

  return startCycle.registerCycleStorage(index, std::make_unique<C>(args...));
}
}
