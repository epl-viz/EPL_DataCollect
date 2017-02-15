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
#include <mutex>
#include <vector>

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
 private:
  ws_dissect_t *              dissect = nullptr;
  WiresharkParser::parserData workingData;

  std::recursive_mutex parserLocker;
  std::string          eplFrameName      = "Ethernet POWERLINK";
  std::string          ethernetFrameName = "Ethernet";

 public:
  InputHandler() = default;
  virtual ~InputHandler();

  InputHandler(const InputHandler &) = delete;
  InputHandler(InputHandler &&)      = delete;

  InputHandler &operator=(const InputHandler &) = delete;
  InputHandler &operator=(InputHandler &&) = delete;

  mockable Packet parsePacket(ws_dissection *diss) noexcept;

  mockable std::vector<Packet> getCyclePackets(uint32_t cycleNum) noexcept;
  mockable bool waitForCycle(uint32_t num, uint32_t timeout = 0) noexcept;

  mockable void setDissector(ws_dissect_t *dissPTR);

  mockable void setEPLFrameName(std::string name) noexcept;
  mockable std::string getEPLFrameName() const noexcept;

  mockable void setEthernetFrameName(std::string name) noexcept;
  mockable std::string getEthernetFrameName() const noexcept;
};
}
