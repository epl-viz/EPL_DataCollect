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
#include "PluginManager.hpp"

namespace EPL_DataCollect {

CaptureInstance::~CaptureInstance() {}

/*!
 * \brief Sets up Plugins, Cycle Builder, etc
 * \returns 0   on success
 * \returns -1  when the state of the CI is not SETUP
 * \returns 1   on PluginManager error; THIS SETS THE STATE TO ERRORED
 * \returns 100 when unable to start the build loop; THIS SETS THE STATE TO ERRORED
 */
int CaptureInstance::setupLoop() {
  std::lock_guard<std::recursive_mutex> lock(accessMutex);
  if (state != SETUP) {
    return -1;
  }

  if (!pluginManager.init(this)) {
    state = ERRORED;
    return 1;
  }

  if (!builder.startLoop(startCycle)) {
    state = ERRORED;
    return 100;
  }

  state = RUNNING;
  return 0;
}


/*!
 * \brief Loads the specified XDD file
 * \param  path the xdd file to load
 * \todo IMPLEMENT
 */
void CaptureInstance::loadXDD(std::string path) noexcept {
  std::lock_guard<std::recursive_mutex> lock(accessMutex);
  (void)path;
}


/*!
 * \brief starts recording on the specified ethernet device
 *
 * A list of available devices can be obtained with getDevices()
 * \param  interface The network device to use for the live capture
 * \todo IMPLEMENT
 * \sa setupLoop for return values
 */
int CaptureInstance::startRecording(std::string interface) noexcept {
  std::lock_guard<std::recursive_mutex> lock(accessMutex);
  (void)interface;

  if (state != SETUP) {
    return -1;
  }

  return setupLoop();
}


/*!
 * \brief ends the live capture
 * \returns 0  on success; the new state is now DONE
 * \returns -1 when the current state is not RUNNING
 * \returns 2  when the build loop could not be stopped; THIS SETS THE STATE TO ERRORED
 */
int CaptureInstance::stopRecording() noexcept {
  std::lock_guard<std::recursive_mutex> lock(accessMutex);
  if (state != RUNNING) {
    return -1;
  }

  if (!builder.stopLoop()) {
    state = ERRORED;
    return 2;
  }

  state = DONE;
  return 0;
}


/*!
 * \brief loads a previously captured PCAP
 * \param  file The file to load
 * \todo IMPLEMENT
 * \sa setupLoop for return values
 */
int CaptureInstance::loadPCAP(std::string file) noexcept {
  std::lock_guard<std::recursive_mutex> lock(accessMutex);
  (void)file;

  if (state != SETUP) {
    return -1;
  }

  return setupLoop();
}


/*!
 * \brief Returns a list of available network devices
 * \return std::vector<std::string>
 * \todo IMPLEMENT
 */
std::vector<std::string> CaptureInstance::getDevices() noexcept {
  std::lock_guard<std::recursive_mutex> lock(accessMutex);
  return std::vector<std::string>();
}


/*!
 * \brief Returns a pointer to the EventLog
 * \note The pointer is valid for the entire lifetime of the CaptureInstance instance
 * \return EventLog
 */
EventLog *CaptureInstance::getEventLog() noexcept {
  std::lock_guard<std::recursive_mutex> lock(accessMutex);
  return &eventLog;
}


/*!
 * \brief Returns a pointer to the PluginManager
 * \note The pointer is valid for the entire lifetime of the CaptureInstance instance
 * \return PluginManager
 */
PluginManager *CaptureInstance::getPluginManager() noexcept {
  std::lock_guard<std::recursive_mutex> lock(accessMutex);
  return &pluginManager;
}


/*!
 * \brief Returns a pointer to the CycleContainer
 * \note The pointer is valid for the entire lifetime of the CaptureInstance instance
 * \return CycleContainer
 */
CycleContainer *CaptureInstance::getCycleContainer() noexcept {
  std::lock_guard<std::recursive_mutex> lock(accessMutex);
  return &cycleContainer;
}

/*!
 * \brief Returns the current state of the CaptureInstance
 */
CaptureInstance::CIstate CaptureInstance::getState() noexcept {
  std::lock_guard<std::recursive_mutex> lock(accessMutex);
  return state;
}
}
