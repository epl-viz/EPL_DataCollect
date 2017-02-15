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
 * \file Packet.hpp
 * \brief Contains class Packet
 */

#pragma once

#include "defines.hpp"
#include "InputParser.hpp"
#include "ODDescription.hpp"
#include "PacketDiff.hpp"
#include "EPLEnums.h"
#include <chrono>
#include <plf_colony.h>
#include <vector>

namespace EPL_DataCollect {

/*!
 * class Packet
 * \brief Represents a packet on the Ethernet bus
 */
class Packet {
 public:
  typedef std::chrono::system_clock::time_point TIME_POINT;

 private:
  MessageType             type = PT_UNDEF;
  plf::colony<PacketDiff> diffs;

  std::string wiresharkSTR = "";
  std::string miscData     = "<N/A>";

  uint32_t nodeSource;
  uint32_t nodeDest;

  TIME_POINT timeStamp;

 public:
  Packet() = delete;
  virtual ~Packet();

  Packet(const WiresharkParser::parserData *const data);

  Packet(const Packet &) = default;
  Packet(Packet &&)      = default;

  Packet &operator=(const Packet &) = default;
  Packet &operator=(Packet &&) = default;

  bool operator==(const Packet &r) const;

  mockable MessageType getType() const noexcept;
  mockable plf::colony<PacketDiff> *getDiffs() noexcept;
  mockable std::string getMiscData() const noexcept;
  mockable std::string getWiresharkString() const noexcept;
  mockable uint32_t getSrcNode() const noexcept;
  mockable uint32_t getDestNode() const noexcept;
  mockable TIME_POINT getTimeStamp() const noexcept;

  mockable void addDiff(uint16_t index, ODEntryContainer entry) noexcept;
};
}
