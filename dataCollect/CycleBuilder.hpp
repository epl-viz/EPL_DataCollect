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
 * \file CycleBuilder.hpp
 * \brief Contains class CycleBuilder
 */

#pragma once

#include "Cycle.hpp"
#include <condition_variable>
#include <mutex>
#include <thread>

namespace EPL_DataCollect {

/*!
  * class CycleBuilder
  * \brief Updates the cycle
  *
  * This class is responsible for deterministically generating / updating cycle with
  * the provided packet from the InputHandler and the invocation of the
  * PluginManager
  */
class CycleBuilder {
 private:
  Cycle                currentCycle;
  std::recursive_mutex accessMutex;
  std::thread          loopThread;

  std::mutex              startLoopSignal;
  std::mutex              stopLoopSignal;
  std::condition_variable startLoopWait;
  std::condition_variable stopLoopWait;

  bool isLoopRunning = false;
  bool keepLoopAlive = false;

 public:
  CycleBuilder() = default;
  virtual ~CycleBuilder();

  CycleBuilder(const CycleBuilder &) = delete;
  CycleBuilder(CycleBuilder &&)      = delete;

  CycleBuilder &operator=(const CycleBuilder &) = delete;
  CycleBuilder &operator=(CycleBuilder &&) = delete;

  mockable bool startLoop(Cycle startPoint) noexcept;
  mockable bool stopLoop() noexcept;
  mockable bool isRunning() noexcept;

  mockable Cycle seekCycle(uint32_t targetCycle, Cycle start) noexcept;
  mockable Cycle getCurrentCycle() noexcept;

#if EPL_DC_ENABLE_MOCKING == 0
 private:
#endif
  mockable void buildNextCycle() noexcept;
  mockable void buildLoop() noexcept;
};
}
