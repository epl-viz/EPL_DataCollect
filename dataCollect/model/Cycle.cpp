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
 * \file Cycle.cpp
 * \brief Contains class Cycle
 */

#include "Cycle.hpp"

namespace EPL_DataCollect {

Cycle::~Cycle() {}

Cycle::Cycle(const Cycle &c) {
  events    = c.events;
  packets   = c.packets;
  nodes     = c.nodes;
  nodeCount = c.nodeCount;
  cycleNum  = c.cycleNum;

  for (auto &i : c.cycleStorages) {
    cycleStorages[i.first] = i.second->clone();
  }
}

Cycle &Cycle::operator=(const Cycle &c) {
  events    = c.events;
  packets   = c.packets;
  nodes     = c.nodes;
  nodeCount = c.nodeCount;
  cycleNum  = c.cycleNum;

  for (auto &i : c.cycleStorages) {
    cycleStorages[i.first] = i.second->clone();
  }

  return *this;
}

/*!
 * \brief Returns the number of nodes
 * \return The amount of nodes as an unsigned char
 */
uint8_t Cycle::getNumNodes() const noexcept { return nodeCount; }


/*!
 * \brief Returns all events active in the cycle
 * \return The currently active events as an std::vector<EventBase*>
 */
std::vector<EventBase *> Cycle::getActiveEvents() const noexcept { return events; }


/*!
 * \brief Returns all packets in the cycle
 * \return The packets currently in the cycle as an std::vector<Packet>
 */
std::vector<Packet> Cycle::getPackets() const noexcept { return packets; }


/*!
 * \brief Returns the cycle number of this Cycle
 * \return The current cycle number as an unsigned int
 */
uint32_t Cycle::getCycleNum() const noexcept { return cycleNum; }


/*!
 * \brief Returns a pointer to the specified node
 * \param  node The ID / index of the node
 * \return The node given by the ID as type Node or nullptr if the node does not exist
 */
Node *Cycle::getNode(uint8_t node) noexcept {
  auto n = nodes.find(node);

  if (n == nodes.end())
    return nullptr;

  return &n->second;
}


/*!
 * \brief Returns the specified OD entry of the node with the given index
 * \param  node The ID / index of the node
 * \param entry The ID / index of the OD entry
 * \return The pointer to the requested OD entry of the specified node or nullptr if the node or entry does not exist
 */
ODEntry *Cycle::getODEntry(uint8_t node, uint16_t entry) noexcept {
  Node *n  = nullptr;
  OD *  od = nullptr;

  n = getNode(node);

  // Check if node was found
  if (n == nullptr)
    return nullptr;

  od = n->getOD();

  // Check if OD contains requested entry
  if (!od->hasEntry(entry))
    return nullptr;

  return od->getEntry(entry);
}


/*!
 * \brief Returns a pointer to the cycle storage corresponding to the ID
 * \note Returns nullptr on error
 * \return The pointer to the cycle storage as a CycleStorageBase*
 * \param  id The ID of the storage
 */
CycleStorageBase *Cycle::getCycleStorage(std::string id) noexcept {
  auto cs = cycleStorages.find(id);

  // Check if an entry with id exists
  if (cs == cycleStorages.end())
    return nullptr; // No entry with id found

  return cs->second.get();
}


/*!
 * \brief Registers a new cycle storage
 * \param id  the id of the new CS
 * \param ptr a pointer to the new CycleStorage
 * \returns true on success, false if id already exists
 */
bool Cycle::registerCycleStorage(std::string id, std::unique_ptr<CycleStorageBase> ptr) noexcept {
  // Check if id already exists
  if (getCycleStorage(id) != nullptr) {
    return false;
  }

  cycleStorages[id] = std::move(ptr);
  return true;
}


/*!
   * \brief Updates the packet list and increases the cycle counter.
   * This will clear the old packet list and increment the cycle counter by one.
   * \note This function will NOT change the OD entries!
   *
   * C++: friend class CycleBuilder
   * \param  newPackets The packets to apply
   */
void Cycle::updatePackets(std::vector<Packet> newPackets) noexcept {
  packets = newPackets;
  cycleNum++;
}

bool Cycle::operator==(const Cycle &b) const {
  return events == b.events && /*packets == b.packets && nodes == b.nodes &&*/ nodeCount == b.nodeCount &&
         cycleNum == b.cycleNum;
}
}
