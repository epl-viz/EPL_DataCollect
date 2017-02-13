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
#include "InputParser.hpp"
#include <ws_capture.h>
#include <ws_dissect.h>

namespace EPL_DataCollect {

using namespace WiresharkParser;

InputHandler::~InputHandler() {}

Packet InputHandler::parsePacket(ws_dissection *diss) noexcept {
  std::lock_guard<std::recursive_mutex> lock(parserLocker);

  parserData d;
  d.eplFrameName = &eplFrameName;
  d.wsOther      = "";

  proto_tree_children_foreach(diss->edt->tree, foreachFunc, reinterpret_cast<gpointer>(&d));

  Packet packet(d.pType, d.cID, d.descPTR, d.wsString, d.wsOther, d.src, d.dst, d.tp, d.transactID, d.numSegments);
  return packet;
}

/*!
 * \brief Returns all packets within a complete cycle.
 * \note Always call waitForCycle first
 * Throws if the cycle does not exist.
 *
 * \return a std::vector of Packet
 * \todo IMPLEMENT
 * \param  cycleNum The number of the cycle
 */
std::vector<Packet> InputHandler::getCyclePackets(uint32_t cycleNum) noexcept {
  (void)cycleNum;
  return std::vector<Packet>();
}

/*!
 * \brief Waits until the specified cycle is available
 * \note This function should always be called before getCyclePackets
 * Returns false on timeout.
 * \return bool
 * \param  num The number of the cycle to wait for
 * \param  timeout The timeout in milliseconds (0 for no timeout)
 * \todo IMPLEMENT
 */
bool InputHandler::waitForCycle(uint32_t num, uint32_t timeout) noexcept {
  (void)num;
  (void)timeout;
  return false;
}

void InputHandler::setDissector(ws_dissect_t *dissPTR) { dissect = dissPTR; }

/*!
 * \brief Sets the frame name of the dissected wireshark packet
 * \param name The new name
 */
void InputHandler::setEPLFrameName(std::string name) noexcept { eplFrameName = name; }

/*!
 * \brief Returns the current name of the EPL Frame (used for parsing the wireshark packet)
 */
std::string InputHandler::getEPLFrameName() const noexcept { return eplFrameName; }

/*!
 * \brief Sets the frame name of the dissected wireshark packet
 * \param name The new name
 */
void InputHandler::setEthernetFrameName(std::string name) noexcept { ethernetFrameName = name; }

/*!
 * \brief Returns the current name of the EPL Frame (used for parsing the wireshark packet)
 */
std::string InputHandler::getEthernetFrameName() const noexcept { return ethernetFrameName; }
}
