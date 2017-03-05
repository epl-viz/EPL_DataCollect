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
 * \file Node.hpp
 * \brief The Node class represents a device in an ethernetPOWERLINK network
 * \todo Add more information on this representation of the Nodes and implement attribute accessors
 */

#pragma once

#include "defines.hpp"
#include "InputHandler.hpp"
#include "OD.hpp"

namespace EPL_DataCollect {

class CycleBuilder;

/*!
  * class Node
  * \brief Representation of a Node in ethernetPOWERLINK
  */
class Node {
 public:
  typedef struct WiresharkParser::parserData::ASnd::IdentResponse IDENT;

 private:
  OD       od;                     //!< \brief The Object dictionary of the Node
  NMTState status = NMTState::OFF; //!< \brief Current status of the Node
  uint8_t  id;                     //!< \brief The ID of this node

  IDENT identity;

 public:
  Node() = delete;
  Node(uint8_t nodeID) : id(nodeID) {}
  virtual ~Node();

  Node(const Node &) = default;
  Node(Node &&)      = default;

  Node &operator=(const Node &) = delete;
  Node &operator=(Node &&) = delete;

  friend CycleBuilder;

  mockable OD *getOD() noexcept;
  mockable ODDescription *getODDesc() noexcept;
  mockable NMTState getStatus() const noexcept;
  mockable std::string getStatusStr() noexcept;
  mockable uint8_t getID() const noexcept;
  mockable IDENT getIdentity() const noexcept;

#if EPL_DC_ENABLE_MOCKING == 0
 private:
#endif
  mockable void setStatus(NMTState newStatus) noexcept;
  mockable void setIdentity(IDENT i) noexcept;
};
}
