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
 * \file Cycle.hpp
 * \brief Contains class Cycle
 */

#pragma once

#include "defines.hpp"
#include "CycleStorageBase.hpp"
#include "EventBase.hpp"
#include "Node.hpp"
#include "Packet.hpp"
#include <memory>

namespace EPL_DataCollect {

class CycleBuilder;

/*!
  * class Cycle
  * \brief Root entity of the ethernetPOWERLINK model
  *
  * Stores all information available during (and after completion of) the specific
  * cycle.
  *
  * \todo Update methods are still to be implemented
  */
class Cycle {
 public:
  typedef std::unordered_map<std::string, std::unique_ptr<CycleStorageBase>> CS_MAP;

 private:
  std::vector<EventBase *> events;
  std::vector<Packet>      packets;

  std::unordered_map<uint8_t, Node> nodes;
  CS_MAP cycleStorages;

  uint8_t  nodeCount = 0;
  uint32_t cycleNum  = 0;

 public:
  Cycle() = default;
  virtual ~Cycle();

  Cycle(const Cycle &);
  Cycle(Cycle &&) = default;

  Cycle &operator=(const Cycle &);
  Cycle &operator=(Cycle &&) = default;

  friend CycleBuilder;

  mockable std::vector<EventBase *> getActiveEvents() const noexcept;
  mockable std::vector<Packet> getPackets() const noexcept;
  mockable uint32_t getCycleNum() const noexcept;
  mockable uint8_t getNumNodes() const noexcept;

  mockable Node *getNode(uint8_t node) noexcept;
  mockable ODEntry *getODEntry(uint8_t node, uint16_t entry) noexcept;
  mockable CycleStorageBase *getCycleStorage(std::string id) noexcept;
  mockable bool registerCycleStorage(std::string id, std::unique_ptr<CycleStorageBase> ptr) noexcept;

  std::string getData(const char *index);
  bool setDataStr(const char *index, const char *var);
  bool setDataInt(const char *index, int var);
  bool setDataBool(const char *index, bool var);

  bool operator==(const Cycle &b) const;

#if EPL_DC_ENABLE_MOCKING == 0
 private:
#endif
  mockable bool addNode(uint8_t node);
  mockable void updatePackets(std::vector<Packet> newPackets) noexcept;
};
}
