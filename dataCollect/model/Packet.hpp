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

  typedef struct WiresharkParser::parserData::SoC                  s_SoC;
  typedef struct WiresharkParser::parserData::PReq                 s_PReq;
  typedef struct WiresharkParser::parserData::PRes                 s_PRes;
  typedef struct WiresharkParser::parserData::SoA                  s_SoA;
  typedef struct ASnd                                              s_ASnd;
  typedef struct WiresharkParser::parserData::ASnd::IdentResponse  s_IResp;
  typedef struct WiresharkParser::parserData::ASnd::NMTRequest     s_NMTRequest;
  typedef struct WiresharkParser::parserData::ASnd::NMTCmd         s_NMTCmd;
  typedef struct WiresharkParser::parserData::ASnd::StatusResponse s_StatResp;
  typedef struct WiresharkParser::parserData::ASnd::SyncResponse   s_SyncResp;
  typedef struct WiresharkParser::parserData::ASnd::SDO            s_SDO;

  typedef std::chrono::system_clock::time_point TIME_POINT;

  struct ASnd {
    ASndServiceID        RequestedServiceID     = ASndServiceID::RESERVED_0;
    uint8_t              RequestedServiceTarget = UINT8_MAX;
    std::vector<uint8_t> Data;
  };

 private:
  PacketType              type = PacketType::UNDEF;
  NMTState                state;
  plf::colony<PacketDiff> diffs;

  std::string wiresharkSTR = "";
  std::string miscData     = "<N/A>";

  uint8_t nodeSource;
  uint8_t nodeDest;

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
  mockable std::string getTypeStr() const noexcept;
  mockable NMTState getState() const noexcept;
  mockable const plf::colony<PacketDiff> *getDiffs() const noexcept;
  mockable std::string getMiscData() const noexcept;
  mockable std::string getWiresharkString() const noexcept;
  mockable uint8_t getSrcNode() const noexcept;
  mockable uint8_t getDestNode() const noexcept;
  mockable TIME_POINT getTimeStamp() const noexcept;
  mockable uint64_t getTime() const noexcept;

  mockable void addDiff(PacketDiff diff) noexcept;
};
}
