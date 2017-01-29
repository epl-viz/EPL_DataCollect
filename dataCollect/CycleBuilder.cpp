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
 * \file CycleBuilder.cpp
 * \brief Contains class CycleBuilder
 */

#include "CycleBuilder.hpp"
#include <chrono>

namespace EPL_DataCollect {

CycleBuilder::~CycleBuilder() {
  std::lock_guard<std::recursive_mutex> lock(accessMutex);
  if (isLoopRunning)
    stopLoop();

  if (loopThread.joinable()) {
    loopThread.join();
  }
}

void CycleBuilder::buildNextCycle() noexcept {}


/*!
 * \brief Main cycle builder loop. Runs in a seperate thread
 * \todo IMPLEMENT
 */
void CycleBuilder::buildLoop() noexcept {

  {
    std::lock_guard<std::mutex> lk(startLoopSignal);
    isLoopRunning = true;
    startLoopWait.notify_all();
  }

  //! \todo IMPLEMENT
  while (keepLoopAlive) {
    std::this_thread::sleep_for(std::chrono::nanoseconds(1000));
    buildNextCycle();
  }

  std::lock_guard<std::mutex> lk(stopLoopSignal);
  isLoopRunning = false;
  stopLoopWait.notify_all();
}

/*!
 * \brief Starts the build loop
 * Returns false if a loop is already running
 * \return bool
 */
bool CycleBuilder::startLoop(Cycle startPoint) noexcept {
  std::lock_guard<std::recursive_mutex> lock(accessMutex);

  if (isLoopRunning)
    return false;

  currentCycle = startPoint;

  keepLoopAlive = true;
  loopThread    = std::thread(&CycleBuilder::buildLoop, this);

  while (!isLoopRunning) {
    std::unique_lock<std::mutex> lk(startLoopSignal);
    startLoopWait.wait(lk);
  }

  return true;
}


/*!
 * \brief Stopps the build loop
 * Returns false if no loop is running
 * \return bool
 */
bool CycleBuilder::stopLoop() noexcept {
  std::lock_guard<std::recursive_mutex> lock(accessMutex);

  if (!isLoopRunning)
    return false;

  keepLoopAlive = false;

  while (isLoopRunning) {
    std::unique_lock<std::mutex> lk(stopLoopSignal);
    stopLoopWait.wait(lk);
  }

  return true;
}


/*!
 * \brief Returns whether the build loop is running or not
 * \return bool
 */
bool CycleBuilder::isRunning() noexcept {
  std::lock_guard<std::recursive_mutex> lock(accessMutex);
  return isLoopRunning;
}


/*!
 * \brief (Re)constructs a specific cycle
 * This function uses a specific start cycle as a base to compute the targetCycle.
 * \return Cycle
 * \param  targetCycle The target cycle
 * \param  start The base cycle to iterate over
 * \todo IMPLEMENT
 */
Cycle CycleBuilder::seekCycle(uint32_t targetCycle, Cycle start) noexcept {
  std::lock_guard<std::recursive_mutex> lock(accessMutex);
  (void)targetCycle;
  return start;
}


/*!
 * \brief Returns the last cycle completely processed by the buildLoop
 * \return Cycle
 * \todo IMPLEMENT
 */
Cycle CycleBuilder::getCurrentCycle() noexcept {
  std::lock_guard<std::recursive_mutex> lock(accessMutex);
  return currentCycle;
}
}
