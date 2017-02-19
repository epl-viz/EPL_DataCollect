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
#include <memory>
#include <plf_colony.h>
#include <vector>

namespace EPL_DataCollect {

/*!
 * class Packet
 * \brief Represents a packet on the Ethernet bus
 */
class Packet {
 public:
  struct ASnd;

  using s_SoC        = struct WiresharkParser::parserData::SoC;
  using s_PReq       = struct WiresharkParser::parserData::PReq;
  using s_PRes       = struct WiresharkParser::parserData::PRes;
  using s_SoA        = struct WiresharkParser::parserData::SoA;
  using s_ASnd       = struct ASnd;
  using s_IResp      = struct WiresharkParser::parserData::ASnd::IdentResponse;
  using s_NMTRequest = struct WiresharkParser::parserData::ASnd::NMTRequest;
  using s_NMTCmd     = struct WiresharkParser::parserData::ASnd::NMTCmd;
  using s_StatResp   = struct WiresharkParser::parserData::ASnd::StatusResponse;
  using s_SyncResp   = struct WiresharkParser::parserData::ASnd::SyncResponse;
  using s_SDO        = struct WiresharkParser::parserData::ASnd::SDO;

  typedef std::chrono::system_clock::time_point TIME_POINT;

  struct ASnd {
    ASndServiceID        RequestedServiceID     = ASndServiceID::RESERVED_0;
    uint8_t              RequestedServiceTarget = UINT8_MAX;
    std::vector<uint8_t> Data;
  };

 private:
  PacketType              type = PacketType::UNDEF;
  plf::colony<PacketDiff> diffs;

  std::string wiresharkSTR = "";
  std::string miscData     = "<N/A>";

  uint32_t nodeSource;
  uint32_t nodeDest;

  TIME_POINT timeStamp;

 public:
  std::shared_ptr<s_SoC>        SoC            = nullptr;
  std::shared_ptr<s_PReq>       PReq           = nullptr;
  std::shared_ptr<s_PRes>       PRes           = nullptr;
  std::shared_ptr<s_SoA>        SoA            = nullptr;
  std::shared_ptr<s_ASnd>       ASnd           = nullptr;
  std::shared_ptr<s_IResp>      IdentResponse  = nullptr;
  std::shared_ptr<s_NMTRequest> NMTRequest     = nullptr;
  std::shared_ptr<s_NMTCmd>     NMTCmd         = nullptr;
  std::shared_ptr<s_StatResp>   StatusResponse = nullptr;
  std::shared_ptr<s_SyncResp>   SyncResponse   = nullptr;
  std::shared_ptr<s_SDO>        SDO            = nullptr;

  Packet() = delete;
  virtual ~Packet();

  Packet(const WiresharkParser::parserData *const data);

  Packet(const Packet &) = default;
  Packet(Packet &&)      = default;

  Packet &operator=(const Packet &) = default;
  Packet &operator=(Packet &&) = default;

  bool operator==(const Packet &r) const;

  mockable PacketType getType() const noexcept;
  mockable plf::colony<PacketDiff> *getDiffs() noexcept;
  mockable std::string getMiscData() const noexcept;
  mockable std::string getWiresharkString() const noexcept;
  mockable uint32_t getSrcNode() const noexcept;
  mockable uint32_t getDestNode() const noexcept;
  mockable TIME_POINT getTimeStamp() const noexcept;

  mockable void addDiff(uint16_t index, ODEntryContainer entry) noexcept;
};
}
