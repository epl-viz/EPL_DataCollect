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
 * \file InputHandler.hpp
 * \brief Contains class InputHandler
 */

#pragma once

#include "defines.hpp"

#include "InputParser.hpp"
#include "Packet.hpp"
#include <chrono>
#include <condition_variable>
#include <mutex>
#include <plf_colony.h>
#include <queue>
#include <thread>
#include <vector>

using std::chrono_literals::operator""ms;
using std::chrono::milliseconds;

extern "C" {
typedef struct ws_capture_t ws_capture_t;
typedef struct ws_dissect_t ws_dissect_t;
struct ws_dissection;
}

namespace EPL_DataCollect {

/*!
  * class InputHandler
  * \brief The InputHandler is a wrapper for the libwireshark backend
  *
  * The input handler accumulates a set of packets, representing a full cycle, on
  * request.
  * The C / Wireshark style representation of those packets is then transformed into
  * a more usable C++ representation (The \sa Packet class).
  *
  * The ODDescription is also copied (\sa ODDescription).
  */
class InputHandler {
 public:
  typedef std::chrono::system_clock::time_point TP;

  static const uint8_t DONE            = 0b00000001; //!< \brief The cycle is completely processed
  static const uint8_t ERROR           = 0b00000010; //!< \brief There was a error
  static const uint8_t IS_EOF          = 0b00000100; //!< \brief Reached the end of capture
  static const uint8_t NO_DELETE       = 0b00001000; //!< \brief Do not delete this entry (unless CLEANUP is set)
  static const uint8_t CLEANUP         = 0b00010000; //!< \brief Clean this entry up (overrides NO_DELETE)
  static const uint8_t QC_PREFETCH_HIT = 0b00100000; //!< \brief The entry was already queued
  static const uint8_t QC_ALREADY_DONE = 0b01000000; //!< \brief The entry was already processed when required
  static const uint8_t USED_SEEK       = 0b10000000; //!< \brief Debug flag to identify seeks

  struct CompletedCycle {
    uint8_t             flags = 0;
    uint32_t            num   = UINT32_MAX;
    std::vector<Packet> packets;
    TP                  tp;

    CompletedCycle() = delete;
    CompletedCycle(uint32_t n) : num(n) {}
  };

  struct QueuedCycle {
    bool            cleanup = false;
    CompletedCycle *out     = nullptr;

    QueuedCycle() = delete;
    QueuedCycle(bool c, CompletedCycle *o) : cleanup(c), out(o) {}
  };

  struct Config {
    uint32_t     cleanupInterval   = 50;
    uint8_t      prefetchSize      = 20; //!< \brief The number of cycles to prefetch
    uint8_t      checkPrefetch     = 13; //!< \brief Checks for prefetching every [num] cycles
    milliseconds loopWaitTimeout   = 500ms;
    milliseconds deleteCyclesAfter = 5000ms;

    std::string eplFrameName = "Ethernet POWERLINK";
  };

 private:
  // This data may only be accessed in parsing functions (parsePacket, parseCycle, setDissector)
  struct {
    std::recursive_mutex parserLocker;

    ws_dissect_t *              dissect = nullptr;
    WiresharkParser::parserData workingData;

    std::vector<uint64_t> cycleOffsetMap;
    bool                  parserReachedEnd = false;

    Packet latestSoC = Packet(nullptr);

    // TODO remove this vector and use the next offset instead
    std::vector<uint64_t> packetOffsetMap;
  } pData;

  std::mutex accessMutex;
  std::mutex configMutex;

  Config cfg;

  std::thread loopThread;

  std::deque<QueuedCycle> buildQueue;
  std::mutex              buildQueueMutex;

  plf::colony<CompletedCycle> cycles;
  std::mutex                  cyclesMutex;

  std::mutex              startLoopMutex;
  std::mutex              stopLoopMutex;
  std::condition_variable startLoopWait;
  std::condition_variable stopLoopWait;

  std::condition_variable waitForWorkSignal;
  std::condition_variable waitForDoneWorkSignal;

  bool buildLoopIsRunning   = false;
  bool keepBuildLoopRunning = false;

  uint32_t maxQueuedCycle         = 0;
  uint32_t queuedAfterLastCleanup = 0;
  uint32_t lastCheckedForPrefetch = UINT32_MAX;

  CompletedCycle *updateQueue(CompletedCycle *oldCycle, uint32_t target = UINT32_MAX) noexcept;
  void builderLoop();
  bool waitForCycleCompletion(CompletedCycle *cd, milliseconds timeout) noexcept;

 public:
  InputHandler() = default;
  virtual ~InputHandler();

  InputHandler(const InputHandler &) = delete;
  InputHandler(InputHandler &&)      = delete;

  InputHandler &operator=(const InputHandler &) = delete;
  InputHandler &operator=(InputHandler &&) = delete;

  mockable Packet parsePacket(ws_dissection *diss) noexcept;
  mockable bool parseCycle(CompletedCycle *cd) noexcept;

  mockable bool startLoop();
  mockable bool stopLoop();

  mockable std::vector<Packet> getCyclePackets(uint32_t cycleNum, milliseconds timeout = 10000000ms) noexcept;

  mockable void setDissector(ws_dissect_t *dissPTR);

  mockable void setConfig(Config newCFG) noexcept;
  mockable Config getConfig() const noexcept;
};
}
