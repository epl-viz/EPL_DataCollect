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
  PacketType              type      = PT_UNDEF;
  CommandID               commandID = CMD_ID_NIL;
  ODDescription *         odDesc    = nullptr;
  plf::colony<PacketDiff> diffs;

  std::string wiresharkSTR = "";
  std::string otherData    = "<N/A>";

  uint32_t nodeSource;
  uint32_t nodeDest;

  TIME_POINT timeStamp;

  uint8_t  transactionID = 0;
  uint32_t numOfSegments = 0;

 public:
  Packet() = delete;
  virtual ~Packet();

  Packet(PacketType     t,
         CommandID      cID,
         ODDescription *oDesc,
         std::string    wireSTR,
         std::string    other,
         uint32_t       src,
         uint32_t       dest,
         TIME_POINT     ts,
         uint8_t        tID       = 0,
         uint32_t       nSegments = 0);

  Packet(const Packet &) = default;
  Packet(Packet &&)      = default;

  Packet &operator=(const Packet &) = default;
  Packet &operator=(Packet &&) = default;

  mockable PacketType getType() const noexcept;
  mockable uint8_t getTransactionID() const noexcept;
  mockable uint32_t getNumSegments() const noexcept;
  mockable plf::colony<PacketDiff> *getDiffs() noexcept;
  mockable std::string getOtherData() const noexcept;
  mockable std::string getWiresharkString() const noexcept;
  mockable ODDescription *getODDesc() noexcept;
  mockable uint32_t getSrcNode() const noexcept;
  mockable uint32_t getDestNode() const noexcept;
  mockable TIME_POINT getTimeStamp() const noexcept;

  mockable void addDiff(uint16_t index, std::shared_ptr<ODEntry> entry) noexcept;
};
}
