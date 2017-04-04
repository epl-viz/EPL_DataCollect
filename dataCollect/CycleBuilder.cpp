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
#include "CaptureInstance.hpp"
#include "EPLEnum2Str.hpp"
#include "XDDParser.hpp"
#include <chrono>
#include <iostream>

using namespace std::chrono;

namespace EPL_DataCollect {

CycleBuilder::CycleBuilder(CaptureInstance *ptr) : parent(ptr), ih(ptr->getInputHandler()) {}

CycleBuilder::~CycleBuilder() {
  std::lock_guard<std::recursive_mutex> lock(accessMutex);
  if (isLoopRunning)
    stopLoop();

  if (loopThread.joinable()) {
    loopThread.join();
  }
}

void CycleBuilder::addNode(uint8_t nID) noexcept {
  currentCycle.addNode(nID);

  auto cfg = parent->getNodeConfig(nID);

  std::string xddDir = parent->getConfig().xddDir + '/';
  std::cout << "[CycleBuilder] Adding Node " << static_cast<int>(nID) << std::endl;

  if (!cfg.baseProfile.empty()) {
    std::string xddPath = xddDir + cfg.baseProfile;

    std::cout << "[CycleBuilder]   -- Loading baseProfile " << xddPath << std::endl;

    auto ret = XDDParser::parseXDD(currentCycle.getNode(nID)->getOD(), xddPath);
    if (ret != XDDParser::SUCCESS) {
      std::cerr << "[CycleBuilder] Failed to parse XDD [" << EPLEnum2Str::toStr(ret) << "]" << xddPath << " for node "
                << static_cast<int>(nID) << std::endl;
    }
    currentCycle.getNode(nID)->xddFiles.emplace_back(xddPath);
  }

  if (!cfg.autoDeduceSpecificProfile && cfg.specificProfile != "") {
    std::string xddPath = xddDir + cfg.specificProfile;
    std::cout << "[CycleBuilder]   -- Loading specific profile " << xddPath << std::endl;

    auto ret = XDDParser::parseXDD(currentCycle.getNode(nID)->getOD(), xddPath);
    if (ret != XDDParser::SUCCESS) {
      std::cerr << "[CycleBuilder] Failed to parse XDD [" << EPLEnum2Str::toStr(ret) << "]" << xddPath << " for node "
                << static_cast<int>(nID) << std::endl;
    }
    currentCycle.getNode(nID)->xddFiles.emplace_back(xddPath);
  }
}

enum {
  NODEID_DYNAMIC                = 0,
  NODEID_MN                     = 240,
  NODEID_DIAGNOSTIC             = 253,
  NODEID_LEGACY_ETHERNET_ROUTER = 254,
  NODEID_BROADCAST              = 255
};

void CycleBuilder::buildNextCycle() noexcept {
  uint32_t currentCycleNum = currentCycle.getCycleNum();
  uint32_t nextCycleNum    = currentCycleNum + 1;

  if (currentCycleNum == UINT32_MAX) {
    nextCycleNum = 0;
  }

  std::vector<Packet> packets = ih->getCyclePackets(nextCycleNum, seconds(1));

  if (ih->getReachedEnd(nextCycleNum)) {
    reachedEnd = true;
    return;
  }

  if (packets.empty()) {
    return;
  }

  std::lock_guard<std::mutex> lock(currentCycleAccessMutex);

  currentCycle.cycleNum = nextCycleNum;
  currentCycle.updatePackets(packets);

  parent->getEventLog()->setNextCycle(nextCycleNum);


  // Process packets
  for (auto const &i : packets) {
    if (i.getPhysicalFileOffset() > maxProcessingOffset)
      maxProcessingOffset = i.getPhysicalFileOffset();

    if (i.getType() == PacketType::UNDEF)
      continue;

    auto src = i.getSrcNode();
    auto dst = i.getDestNode();
    if (src != NODEID_BROADCAST && src != NODEID_DYNAMIC && currentCycle.getNode(src) == nullptr)
      addNode(src);

    if (dst != NODEID_BROADCAST && dst != NODEID_DYNAMIC && currentCycle.getNode(dst) == nullptr)
      addNode(dst);

    uint8_t updateTargetNode = src;

    // Check if the DESTINATION is changed
    if (i.getType() == PacketType::ASYNC_SEND) {
      if (i.ASnd.get() == nullptr) {
        std::cerr << "[CycleBuilder] Internal error! Invalid parsed packet! ASnd == nullptr" << std::endl;
        break;
      }

      if (i.ASnd->RequestedServiceID == ASndServiceID::SDO) {
        updateTargetNode = dst;
      }
    } else if (i.getType() == PacketType::POLL_REQUEST) {
      updateTargetNode = dst;
    }

    // Update OD entries
    Node *node       = currentCycle.getNode(src);
    Node *targetNode = currentCycle.getNode(updateTargetNode);

    if (!node) {
      std::cerr << "[CycleBuilder] Internal error! Invalid node" << std::endl;
      continue;
    }

    if (targetNode) {
      OD *od = targetNode->getOD();
      for (auto const &j : *i.getDiffs()) {
        uint16_t index = j.getIndex();
        if (index == UINT16_MAX)
          continue;

        ODEntryContainer entry = j.getEntry(od);

        if (od->hasEntry(index)) {
          od->entries.at(index) = entry;
        } else {
          std::cout << "[CycleBuilder] Entry " << std::hex << index << " of Node " << std::dec
                    << static_cast<int>(updateTargetNode) << " does not exist" << std::endl;
        }
      }
    }

    // Handle other packet stuff
    switch (i.getType()) {
      case PacketType::POLL_RESPONSE:
      case PacketType::START_OF_ASYNC: node->status = i.getState(); break;
      case PacketType::ASYNC_SEND:
        if (i.ASnd.get() == nullptr) {
          std::cerr << "[CycleBuilder] Internal error! Invalid parsed packet! ASnd == nullptr" << std::endl;
          break;
        }

        switch (i.ASnd->RequestedServiceID) {
          case ASndServiceID::STATUS_RESPONSE: node->status = i.getState(); break;
          case ASndServiceID::IDENT_RESPONSE: {
            if (i.IdentResponse.get() == nullptr) {
              std::cerr << "[CycleBuilder] Internal error! Invalid parsed packet! IdentResponse == nullptr"
                        << std::endl;
              break;
            }

            node->setIdentity(*i.IdentResponse);

            auto nodeCfg = parent->getNodeConfig(src);
            if (i.IdentResponse->Profile >= 400 && nodeCfg.autoDeduceSpecificProfile) {
              std::cout << "[CycleBuilder] Autodetected Node " << static_cast<int>(src) << " Profile "
                        << i.IdentResponse->Profile << std::endl;

              std::string xddPath = parent->getConfig().xddDir + '/';
              xddPath += std::to_string(i.IdentResponse->Profile) + ".xdd";

              auto ret = XDDParser::parseXDD(currentCycle.getNode(src)->getOD(), xddPath);
              if (ret != XDDParser::SUCCESS) {
                std::cerr << "[CycleBuilder] Failed to parse XDD [" << EPLEnum2Str::toStr(ret) << "]" << xddPath
                          << " for node " << static_cast<int>(src) << std::endl;
              } else {
                nodeCfg.autoDeduceSpecificProfile = false;
                parent->setNodeConfig(src, nodeCfg);
                currentCycle.getNode(src)->xddFiles.emplace_back(xddPath);
              }
            }
          }

          break;
          default: break;
        }


        break;
      default: break;
    }
  }


  parent->getPluginManager()->processCycle(&currentCycle);


  // Process Events
  auto events = parent->getEventLog()->pollEvents(appID);
  for (auto i : events) {
    currentCycle.events.emplace_back(i);
  }

  // Remove old events
  while (true) {
    uint32_t toRemove = UINT32_MAX;

    for (uint32_t i = 0; i < currentCycle.events.size(); ++i) {
      EventBase *it = currentCycle.events[i];

      uint32_t maxCycle;
      it->getCycleRange(nullptr, &maxCycle);

      if (maxCycle < nextCycleNum) {
        toRemove = i;
        break;
      }
    }

    if (toRemove == UINT32_MAX)
      break;

    currentCycle.events.erase(currentCycle.events.begin() + toRemove);
  }

  parent->getSnapshotManager()->registerCycle(&currentCycle);
}


/*!
 * \brief Main cycle builder loop. Runs in a seperate thread
 */
void CycleBuilder::buildLoop() noexcept {

  {
    std::lock_guard<std::mutex> lk(startLoopSignal);
    isLoopRunning = true;
    startLoopWait.notify_all();
  }

  stats.cycleCount = 0;

  auto start = high_resolution_clock::now();

  while (keepLoopAlive) {
    buildNextCycle();
    ++stats.cycleCount;
    std::lock_guard<std::recursive_mutex> sleep(sleepMutex);

    if (reachedEnd)
      break;
  }

  auto end          = high_resolution_clock::now();
  stats.packetCount = parent->getInputHandler()->getPacketsMetadata()->size();
  stats.totalTime   = end - start;
  stats.eventsCount = static_cast<uint32_t>(parent->getEventLog()->getAllEvents().size());
  std::cout << std::endl << "               STATISTICS" << std::endl;
  std::cout << "               ==========" << std::endl << std::endl;
  std::cout << "[CycleBuilder] Cycle count:                   " << stats.cycleCount << std::endl;
  std::cout << "[CycleBuilder] Packet count:                  " << stats.packetCount << std::endl;
  std::cout << "[CycleBuilder] Events Count:                  " << stats.eventsCount << std::endl;
  std::cout << "[CycleBuilder] Total Cycle processing time:   " << stats.totalTime.count() << "ns" << std::endl;
  std::cout << "[CycleBuilder] Average Cycle processing time: " << stats.totalTime.count() / stats.cycleCount << "ns"
            << std::endl
            << std::endl;

  {
    std::lock_guard<std::mutex> lk(stopLoopSignal);
    isLoopRunning = false;
    stopLoopWait.notify_all();
  }

  parent->stopRecording();
}

/*!
 * \brief Starts the build loop
 * Returns false if a loop is already running
 * \return bool
 */
bool CycleBuilder::startLoop(Cycle startPoint) noexcept {
  std::lock_guard<std::recursive_mutex> lock(accessMutex);
  std::unique_lock<std::mutex>          lk1(startLoopSignal);

  appID = parent->getEventLog()->getAppID();

  if (isLoopRunning)
    return false;

  currentCycle = startPoint;

  keepLoopAlive = true;
  loopThread    = std::thread(&CycleBuilder::buildLoop, this);

  while (true) {
    if (!isLoopRunning) {
      startLoopWait.wait(lk1);
    } else {
      break;
    }
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
  std::unique_lock<std::mutex>          lk2(stopLoopSignal);

  if (!isLoopRunning)
    return false;

  keepLoopAlive = false;

  while (true) {
    if (isLoopRunning) {
      stopLoopWait.wait(lk2);
    } else {
      break;
    }
  }

  return true;
}


/*!
 * \brief Blocks the current thread until the builder loop finished
 */
void CycleBuilder::waitForLoopToFinish() noexcept {
  std::unique_lock<std::mutex> lock(stopLoopSignal);

  if (!isLoopRunning)
    return;

  while (true) {
    if (isLoopRunning) {
      stopLoopWait.wait(lock);
    } else {
      break;
    }
  }
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
 */
Cycle CycleBuilder::seekCycle(uint32_t targetCycle, Cycle start) noexcept {
  std::lock_guard<std::recursive_mutex> lock(accessMutex);

  currentCycle = start;

  if (targetCycle <= currentCycle.getCycleNum())
    return std::move(currentCycle);

  while (targetCycle > currentCycle.getCycleNum() && targetCycle < ih->getMaxQueuedCycle())
    buildNextCycle();

  return std::move(currentCycle);
}


/*!
 * \brief Returns the last cycle completely processed by the buildLoop
 * \return Cycle
 */
Cycle CycleBuilder::getCurrentCycle() noexcept {
  std::lock_guard<std::recursive_mutex> lock(accessMutex);
  std::lock_guard<std::mutex>           lockCurCycle(currentCycleAccessMutex);
  return currentCycle;
}

/*!
 * \brief Retruns a wrapper object for a Cycle pointer
 * The returned object locks the CycleBuilder until it is destroyed
 */
CycleBuilder::Locker CycleBuilder::getCurrentCyclePTR() noexcept {
  std::lock_guard<std::recursive_mutex> lock(accessMutex);
  std::lock_guard<std::recursive_mutex> sleep(sleepMutex);
  return Locker(sleepMutex, &currentCycle);
}

CycleBuilder::Statistics CycleBuilder::getStats() const noexcept { return stats; }

uint64_t CycleBuilder::getMaxProcessingOffset() const noexcept { return maxProcessingOffset; }
}
