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
 * \file InputHandler.cpp
 * \brief Contains class InputHandler
 */

#include "InputHandler.hpp"
#include <iostream>
#include <ws_capture.h>
#include <ws_dissect.h>

namespace EPL_DataCollect {

using namespace WiresharkParser;
using namespace std::chrono;

InputHandler::~InputHandler() {
  if (buildLoopIsRunning)
    stopLoop();

  if (loopThread.joinable()) {
    loopThread.join();
  }
}

Packet InputHandler::parsePacket(ws_dissection *diss) noexcept {
  std::lock_guard<std::recursive_mutex> lock(pData.parserLocker);

  // Reset the parser data
  pData.workingData.~parserData();
  new (&pData.workingData) parserData;

  {
    std::lock_guard<std::mutex> configLock(configMutex);
    pData.workingData.eplFrameName = &cfg.eplFrameName;
  }

  char *wsStr = nullptr;
  ws_dissect_tostr(diss, &wsStr);
  pData.workingData.wsString = wsStr;
  g_free(wsStr);

  auto duration        = seconds(diss->timestamp.secs) + nanoseconds(diss->timestamp.nsecs);
  pData.workingData.tp = system_clock::time_point(duration_cast<system_clock::duration>(duration));

  proto_tree_children_foreach(diss->edt->tree, foreachFunc, reinterpret_cast<gpointer>(&pData.workingData));

  Packet packet(&pData.workingData);
  return packet;
}

/*!
 * \brief small error handling helper function for InputHandler::parseCycle
 * \returns always false
 */
inline bool parseCycleError(std::mutex &m, InputHandler::CompletedCycle *cd) {
  std::lock_guard<std::mutex> cLkErr(m);
  cd->packets.clear();
  cd->tp = system_clock::now();
  cd->flags |= InputHandler::DONE | InputHandler::ERROR;
  return false;
}

#ifdef errorFN
#error "errorFN already defined"
#endif
#define errorFN() parseCycleError(cyclesMutex, cd)

bool InputHandler::parseCycle(CompletedCycle *cd) noexcept {
  std::lock_guard<std::recursive_mutex> lock(pData.parserLocker);
  std::lock_guard<std::recursive_mutex> lockOffset(pData.offsetMapLocker);
  static std::vector<Packet>            tempPKG; // static: reuse memory
  ws_dissection                         diss;

  tempPKG.clear(); // tempPKG is static (less malloc) ==> must be cleared

  if (!cd)
    return false;

  // Only forward 1 Cycle max
  if (cd->num > pData.cycleOffsetMap.size())
    return errorFN();


  if (cd->num == pData.cycleOffsetMap.size()) {
    // #################
    // ## Read at end ##
    // #################

    if (pData.parserReachedEnd)
      return errorFN();

    tempPKG.emplace_back(pData.latestSoC);

    // cycleOffsetMap contains a map of already COMPLETELY parsed Cycles
    auto currentCyclePacketIndex = pData.packetOffsetMap.size() - 1;

    PacketMetadata metaData;
    metaData.cycleNum = cd->num;

    while (true) {
      int  err;
      auto ret = ws_dissect_next(pData.dissect, &diss, &err, nullptr);
      if (ret == 0 && err == 0) {
        pData.parserReachedEnd = true;
        return errorFN();
      }

      Packet tmp = parsePacket(&diss);

      metaData.flags  = 0;
      metaData.offset = static_cast<uint64_t>(diss.offset);
      metaData.writeFiled(PacketMetadata::SOURCE, pData.workingData.src);
      metaData.writeFiled(PacketMetadata::DESTINATION, pData.workingData.dst);
      metaData.writeFiled(PacketMetadata::PACKET_TYPE, pData.workingData.pType);

      switch (pData.workingData.pType) {
        case PacketType::START_OF_ASYNC:
          metaData.writeFiled(PacketMetadata::SERVICE_ID, pData.workingData.SoA.RequestedServiceID);
          [[clang::fallthrough]];
        case PacketType::POLL_RESPONSE: metaData.writeFiled(PacketMetadata::NMT_STATE, pData.workingData.pType); break;
        case PacketType::ASYNC_SEND:
          metaData.writeFiled(PacketMetadata::SERVICE_ID, pData.workingData.ASnd.RequestedServiceID);
          switch (pData.workingData.ASnd.RequestedServiceID) {
            case ASndServiceID::SDO:
              metaData.writeFiled(PacketMetadata::COMMAND, pData.workingData.ASnd.SDO.CMD.CommandID);
              break;
            case ASndServiceID::NMT_COMMAND:
              metaData.writeFiled(PacketMetadata::COMMAND, pData.workingData.ASnd.NMTCmd.NMTCommandId);
              break;
            default: break;
          }

          break;
        default: break;
      }

      pData.packetOffsetMap.emplace_back(metaData);

      if (tmp.getType() == PacketType::START_OF_CYCLE) {
        pData.latestSoC = tmp;
        break;
      }

      tempPKG.emplace_back(tmp);
    }

    pData.cycleOffsetMap.emplace_back(currentCyclePacketIndex);

    std::lock_guard<std::mutex> cLk(cyclesMutex);
    cd->packets = std::move(tempPKG);
    cd->tp      = system_clock::now();
    cd->flags |= DONE;



  } else {
    // ##################
    // ## Seek packets ##
    // ##################

    auto next  = cd->num + 1;
    auto first = pData.cycleOffsetMap[cd->num];
    auto last  = next < pData.cycleOffsetMap.size() ? pData.cycleOffsetMap[next] : pData.packetOffsetMap.size();

    // Iterate from first to last - 1 (last is the NEXT SoC)
    for (auto i = first; i < last; ++i) {
      if (ws_dissect_seek(
                pData.dissect, &diss, static_cast<int64_t>(pData.packetOffsetMap[i].offset), nullptr, nullptr) != 1) {
        return errorFN();
      }

      tempPKG.emplace_back(parsePacket(&diss));
    }

    std::lock_guard<std::mutex> cLk(cyclesMutex);
    cd->packets = std::move(tempPKG);
    cd->tp      = system_clock::now();
    cd->flags |= DONE | USED_SEEK;
  }

  return true;
}

#undef errorFN


bool InputHandler::waitForCycleCompletion(CompletedCycle *cd, milliseconds timeout) noexcept {
  system_clock::time_point start = system_clock::now();
  std::mutex               wait;

  while (true) {
    if (system_clock::now() - start > timeout)
      break;

    std::unique_lock<std::mutex> lk(wait);
    waitForDoneWorkSignal.wait_until(lk, start + timeout);

    std::lock_guard<std::mutex> cLk(cyclesMutex);
    if (cd->flags & DONE)
      return true;
  }

  return false;
}


/*!
 * \brief Returns all packets within a complete cycle.
 * \note Waits until the specified cycle is available
 *
 * \return a std::vector of Packet
 * \param  cycleNum The number of the cycle
 * \param  timeout The timeout in milliseconds (0 for no timeout)
 */
std::vector<Packet> InputHandler::getCyclePackets(uint32_t cycleNum, milliseconds timeout) noexcept {
  std::lock_guard<std::mutex> accessLock(accessMutex);

  CompletedCycle *cd = nullptr;

  // Check if packet already exists
  {
    std::lock_guard<std::mutex> cLk(cyclesMutex);
    for (auto it = cycles.begin(); it != cycles.end();) {
      if (it->num == cycleNum) {
        it->flags |= NO_DELETE | QC_PREFETCH_HIT; // Prevent automatic cleanup of this entry
        cd = &(*it);

        if (cd->flags & DONE) {
          cd->flags |= QC_ALREADY_DONE;
        }
      }

      ++it;
    }
  }

  if (cd == nullptr) {
    cd = updateQueue(nullptr, cycleNum);
    // ERROR
    if (cd == nullptr) {
      return std::vector<Packet>();
    }
  } else {
    // Tell the work queue manager to prefetch stuff
    updateQueue(cd);
  }

  if (cd->flags & DONE) {
    return cd->packets;
  } else {
    if (!waitForCycleCompletion(cd, timeout)) {
      return std::vector<Packet>();
    }
  }


  cd->flags |= CLEANUP;
  return cd->packets;
}

InputHandler::CompletedCycle *InputHandler::updateQueue(InputHandler::CompletedCycle *oldCycle,
                                                        uint32_t                      target) noexcept {

  CompletedCycle *            newCD = nullptr;
  CompletedCycle *            qCD   = nullptr;
  std::lock_guard<std::mutex> lkCFG(configMutex);

  if (!oldCycle) {
    if (target == UINT32_MAX) {
      return nullptr;
    }

    if (target > (maxQueuedCycle + 1)) {
      return nullptr;
    }

    if (pData.parserReachedEnd && target > maxQueuedCycle) {
      return nullptr;
    }

    std::lock_guard<std::mutex> lk1(cyclesMutex);
    std::lock_guard<std::mutex> lk2(buildQueueMutex);
    newCD = &(*cycles.emplace(target));
    buildQueue.emplace_back(false, newCD);
    ++queuedAfterLastCleanup;
    lastCheckedForPrefetch = UINT32_MAX;
  } else {
    target = oldCycle->num;
  }

  if (target > maxQueuedCycle) {
    maxQueuedCycle = target;
  }

  if (lastCheckedForPrefetch >= cfg.checkPrefetch) {
    std::vector<bool> alreadyQueued;
    alreadyQueued.resize(cfg.prefetchSize, false);

    std::lock_guard<std::mutex> cLk(cyclesMutex);
    for (auto &i : cycles) {
      int32_t dist = static_cast<int32_t>(i.num) - static_cast<int32_t>(target);
      if (dist < cfg.prefetchSize && dist >= 0) {
        alreadyQueued[static_cast<uint32_t>(dist)] = true;
      }
    }

    std::lock_guard<std::mutex> lk2(buildQueueMutex);

    uint32_t toQueue;
    for (uint32_t i = 0; i < alreadyQueued.size(); ++i) {
      if (!alreadyQueued[i]) {
        toQueue = target + i;
        qCD     = &(*cycles.emplace(toQueue));
        buildQueue.emplace_back(false, qCD);
        ++queuedAfterLastCleanup;
      }
    }

    if (queuedAfterLastCleanup >= cfg.cleanupInterval) {
      buildQueue.emplace_back(true, nullptr);
      queuedAfterLastCleanup = 0;
    }

    lastCheckedForPrefetch = 0;
  } else {
    ++lastCheckedForPrefetch;
  }

  return newCD;
}


void InputHandler::builderLoop() {
  {
    std::lock_guard<std::mutex> lk(startLoopMutex);
    buildLoopIsRunning = true;
    startLoopWait.notify_all();
  }

  QueuedCycle current(true, nullptr);

  while (keepBuildLoopRunning) {
    {
      std::unique_lock<std::mutex> qLk(buildQueueMutex);
      if (buildQueue.empty()) {
        waitForWorkSignal.wait_for(qLk, cfg.loopWaitTimeout);
        continue;
      }

      current = buildQueue.front();
      buildQueue.pop_front();
    }


    if (!current.cleanup) {
      parseCycle(current.out);
      waitForDoneWorkSignal.notify_all(); // let waiting threads continue
    } else {
      std::lock_guard<std::mutex> lk(cyclesMutex);
      std::lock_guard<std::mutex> lkCFG(configMutex);
      auto                        now = system_clock::now();

      auto it = cycles.begin();
      while (it != cycles.end()) {
        if (it->flags & CLEANUP) {
          it = cycles.erase(it);
          continue;
        }

        if (it->flags & NO_DELETE || !(it->flags & DONE)) {
          ++it;
          continue;
        }

        if ((now - it->tp) > cfg.deleteCyclesAfter) {
          it = cycles.erase(it);
          continue;
        }

        ++it;
      }
    }
  }

  std::lock_guard<std::mutex> lk(stopLoopMutex);
  buildLoopIsRunning = false;
  stopLoopWait.notify_all();
}

bool InputHandler::startLoop() {
  std::lock_guard<std::mutex> accessLock(accessMutex);

  // Does loop already run?
  if (keepBuildLoopRunning)
    return false;

  keepBuildLoopRunning = true;

  loopThread = std::thread(&InputHandler::builderLoop, this);

  while (true) {
    std::unique_lock<std::mutex> lk(startLoopMutex);
    if (!buildLoopIsRunning) {
      startLoopWait.wait(lk);
    } else {
      break;
    }
  }

  return true;
}


bool InputHandler::stopLoop() {
  std::lock_guard<std::mutex> accessLock(accessMutex);

  // Does loop already run?
  if (!keepBuildLoopRunning)
    return false;

  keepBuildLoopRunning = false;

  while (true) {
    std::unique_lock<std::mutex> lk(stopLoopMutex);
    if (buildLoopIsRunning) {
      stopLoopWait.wait(lk);
    } else {
      break;
    }
  }

  return true;
}

bool InputHandler::getReachedEnd(uint32_t target) noexcept {
  std::lock_guard<std::mutex> accessLock(accessMutex);
  if (!buildLoopIsRunning)
    return true;

  if (pData.parserReachedEnd && target > maxQueuedCycle)
    return true;

  return false;
}

void InputHandler::setDissector(ws_dissect_t *dissPTR) {
  std::lock_guard<std::mutex>           accessLock(accessMutex);
  std::lock_guard<std::recursive_mutex> lock(pData.parserLocker);
  pData.dissect = dissPTR;

  pData.cycleOffsetMap.clear();
  pData.packetOffsetMap.clear();

  if (pData.dissect) {
    ws_dissection diss;
    if (ws_dissect_next(pData.dissect, &diss, nullptr, nullptr) == 0) {
      pData.parserReachedEnd = true;
      return;
    }

    pData.latestSoC = parsePacket(&diss);
    pData.packetOffsetMap.emplace_back(static_cast<uint64_t>(diss.offset));
  }
}

/*!
 * \brief Sets the config struct of the class
 * \param newCFG The new config struct
 */
void InputHandler::setConfig(Config newCFG) noexcept {
  std::lock_guard<std::mutex> accessLock(accessMutex);
  std::lock_guard<std::mutex> configLock(configMutex);
  cfg = newCFG;
}

/*!
 * \brief Returns the current config
 */
InputHandler::Config InputHandler::getConfig() const noexcept { return cfg; }

/*!
 * \brief Returns the max. queued cycle
 */
uint32_t InputHandler::getMaxQueuedCycle() const noexcept { return maxQueuedCycle; }

InputHandler::Locker InputHandler::getPacketsMetadata() noexcept {
  return Locker(pData.offsetMapLocker, &pData.packetOffsetMap);
}
}
