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

using std::chrono::milliseconds;
using std::chrono::nanoseconds;

extern "C" {
typedef struct ws_capture_t ws_capture_t;
typedef struct ws_dissect_t ws_dissect_t;
struct ws_dissection;
}

namespace EPL_DataCollect {

namespace intetnal {
struct parsePacketConfig;
}

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
  // Section Input handler complex data types

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
    uint32_t     cleanupInterval    = 50;
    uint8_t      prefetchSize       = 20; //!< \brief The number of cycles to prefetch
    uint8_t      checkPrefetch      = 13; //!< \brief Checks for prefetching every [num] cycles
    milliseconds loopWaitTimeout    = milliseconds(500);
    milliseconds deleteCyclesAfter  = milliseconds(5000);
    bool         enablePreSOCCycles = false; //!< \brief When true, Every SoA is treated as a SoC before the 1st SoC

    std::string eplFrameName = constants::EPL_DC_PLUGIN_PROTO_NAME;
  };

  struct Statistics {
    int64_t     packetsParsed     = 0;
    int64_t     cyclesParsed      = 0;
    nanoseconds timePacketsParsed = nanoseconds(0);
    nanoseconds timeCyclesParsed  = nanoseconds(0);
  };

  /*!
   * \brief Contains metadata for a packet
   *
   * flags bit field: has 8 indeces, should be accessed with writeFiled and getFiled
   *
   * | Index |             Description             |            Packet Type           |
   * | ----- | ----------------------------------- | -------------------------------- |
   * | 0     | Source                              | All                              |
   * | 1     | Destination                         | All                              |
   * | 2     | PacketType                          | All                              |
   * | 3     | NMTState                            | PRes, SoA, IdentResp, StatusResp |
   * | 4     | ASndServiceID / SoARequestServiceID | ASnd / SoA                       |
   * | 5     | NMTCommand / SDOCommandID           | NMTCmd / SDO                     |
   * | 6     | RESERVED                            |                                  |
   * | 7     | RESERVED                            |                                  |
   *
   */
  struct PacketMetadata final {
    enum Index : uint8_t { SOURCE = 0, DESTINATION = 1, PACKET_TYPE = 2, NMT_STATE = 3, SERVICE_ID = 4, COMMAND = 5 };

    uint64_t offset   = 0;
    uint64_t phOffset = 0;
    uint64_t flags    = 0;
    uint32_t cycleNum = UINT32_MAX;

    inline void writeFiled(uint8_t index, uint8_t data) {
      if (index >= 8)
        return;

      flags &= ~(static_cast<uint64_t>(0xFF) << index * 8); // Clear before setting
      flags |= static_cast<uint64_t>(data) << index * 8;
    }

    template <class TD>
    inline void writeFiled(Index index, TD data) {
      writeFiled(static_cast<uint8_t>(index), static_cast<uint8_t>(data));
    }

    inline uint8_t getFiled(uint8_t index) {
      if (index >= 8)
        return 0;

      return static_cast<uint8_t>((flags & (static_cast<uint64_t>(0xFF) << index * 8)) >> index * 8);
    }
  };

  struct ParsePacketConfig {
    ws_dissection * diss              = nullptr;
    PacketMetadata *metaData          = nullptr;
    uint64_t        index             = 0;
    bool            checkForNextCycle = false;
    bool            newCycle          = false;
  };

  class Locker {
   private:
    std::unique_lock<std::recursive_mutex> lk;
    std::vector<PacketMetadata> *          c;

   public:
    Locker() = delete;
    Locker(std::recursive_mutex &m, std::vector<PacketMetadata> *data) : lk(m), c(data) {}

    Locker(const Locker &) = delete;
    Locker(Locker &&)      = default;

    Locker &operator=(const Locker &) = delete;
    Locker &operator=(Locker &&) = default;

    inline std::vector<PacketMetadata> &operator*() noexcept { return *c; }
    inline std::vector<PacketMetadata> *operator->() noexcept { return c; }
  };

 private:
  // This data may only be accessed in parsing functions (parsePacket, parseCycle, setDissector)
  struct {
    std::recursive_mutex parserLocker;
    std::recursive_mutex offsetMapLocker;
    std::mutex           dissectLocker;

    ws_dissect_t *              dissect = nullptr;
    WiresharkParser::parserData workingData;

    std::vector<uint64_t> cycleOffsetMap;
    uint64_t              lastValidCyclePacket = 0;
    bool                  parserReachedEnd     = false;
    bool                  firstSoCArrived      = false;

    Packet latestSoC = Packet(nullptr, 0, 0, 0);

    std::vector<PacketMetadata> packetOffsetMap;
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

  Statistics stats;

#if EPL_DC_ENABLE_MOCKING // This is required for some unit tests
 public:
#endif
  bool buildLoopIsRunning   = false;
  bool keepBuildLoopRunning = false;

  uint32_t maxQueuedCycle         = 0;
  uint32_t queuedAfterLastCleanup = 0;
  uint32_t lastValidCycle         = UINT32_MAX;
  uint32_t lastCheckedForPrefetch = UINT32_MAX;

  CompletedCycle *updateQueue(CompletedCycle *oldCycle, uint32_t target = UINT32_MAX) noexcept;
  void builderLoop();
  bool waitForCycleCompletion(CompletedCycle *cd, milliseconds timeout) noexcept;

  Packet parsePacket(ParsePacketConfig &pCfg) noexcept;
  bool parseCycle(CompletedCycle *cd) noexcept;

 public:
  InputHandler() = default;
  virtual ~InputHandler();

  InputHandler(const InputHandler &) = delete;
  InputHandler(InputHandler &&)      = delete;

  InputHandler &operator=(const InputHandler &) = delete;
  InputHandler &operator=(InputHandler &&) = delete;

  mockable bool startLoop();
  mockable bool stopLoop();

  mockable std::vector<Packet> getCyclePackets(uint32_t cycleNum, milliseconds timeout = milliseconds(999999)) noexcept;

  mockable bool getReachedEnd(uint32_t target) noexcept;
  mockable bool getIsLoopRunning() const noexcept { return buildLoopIsRunning; }

  mockable void setDissector(ws_dissect_t *dissPTR);

  mockable void setConfig(Config newCFG) noexcept;
  mockable Config getConfig() const noexcept;
  mockable uint32_t getMaxQueuedCycle() const noexcept;

  mockable Locker getPacketsMetadata() noexcept;
  mockable Statistics getStats() const noexcept;

  mockable uint64_t getNumBytesRead() noexcept;
  mockable std::string generateWiresharkString(Packet const &p) noexcept;
  mockable std::string generateWiresharkString(uint64_t p) noexcept;
};
}
