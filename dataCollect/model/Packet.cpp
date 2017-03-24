/* Copyright (c) 2017, EPL-Vizards
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or misc materials provided with the distribution.
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
 * \file Packet.cpp
 * \brief Contains class Packet
 */

#include "Packet.hpp"
#include <EPLEnum2Str.hpp>

namespace EPL_DataCollect {

using namespace WiresharkParser;
using PT  = PacketType;
using AID = ASndServiceID;

/*!
 * \brief The constructor
 * \param data Pointer to the paresed wireshark data
 * \warning data must not be nullptr
 */
Packet::Packet(const parserData *const data, uint64_t of) {
  if (!data) {
    type         = PT::UNDEF;
    wiresharkSTR = "THE BEGINNING OF TIME";
    miscData     = "THE BEGINNING OF TIME";
    return;
  }

  type         = data->pType;
  state        = data->nmtState;
  nodeSource   = data->src;
  nodeDest     = data->dst;
  wiresharkSTR = data->wsString;
  miscData     = data->wsOther;
  timeStamp    = data->tp;
  diffs        = data->diffs;
  offset       = of;

  switch (type) {
    case PT::START_OF_CYCLE: SoC = std::make_shared<s_SoC>(data->SoC); break;
    case PT::START_OF_ASYNC: SoA = std::make_shared<s_SoA>(data->SoA); break;
    case PT::POLL_REQUEST: PReq  = std::make_shared<s_PReq>(data->PReq); break;
    case PT::POLL_RESPONSE: PRes = std::make_shared<s_PRes>(data->PRes); break;
    case PT::ASYNC_SEND:
      ASnd                         = std::make_shared<s_ASnd>();
      ASnd->RequestedServiceTarget = data->ASnd.RequestedServiceTarget;
      ASnd->RequestedServiceID     = data->ASnd.RequestedServiceID;
      ASnd->Data                   = data->ASnd.Data;

      switch (data->ASnd.RequestedServiceID) {
        case AID::IDENT_RESPONSE: IdentResponse   = std::make_shared<s_IResp>(data->ASnd.IdentResponse); break;
        case AID::STATUS_RESPONSE: StatusResponse = std::make_shared<s_StatResp>(data->ASnd.StatusResponse); break;
        case AID::NMT_REQUEST: NMTRequest         = std::make_shared<s_NMTRequest>(data->ASnd.NMTRequest); break;
        case AID::NMT_COMMAND: NMTCmd             = std::make_shared<s_NMTCmd>(data->ASnd.NMTCmd); break;
        case AID::SDO: SDO                        = std::make_shared<s_SDO>(data->ASnd.SDO); break;
        case AID::BEGIN_MANUFACTURER_SPECIFIC:
        case AID::END_MANUFACTURER_SPECIFIC:
        case AID::RESERVED_0: break;
      }
      break;
    case PT::AINV:
    case PT::AMNI:
    case PT::UNDEF: break;
  }
}

Packet::~Packet() {}

bool Packet::operator==(const Packet &r) const {
  return ((type == r.type) && (wiresharkSTR == r.wiresharkSTR) && (miscData == r.miscData) &&
          (nodeSource == r.nodeSource) && (timeStamp == r.timeStamp));
}

/*!
 * \brief Returns the packet type
 * \return The packet type as a MessageType
 */
PacketType Packet::getType() const noexcept { return type; }

/*!
 * \brief Returns the packet type as str
 * \return The packet type as a str
 */
std::string Packet::getTypeStr() const noexcept { return EPLEnum2Str::toStr(getType()); }

/*!
 * \brief Returns the packet time as long int
 * \return The packet type as a long int
 */
int64_t Packet::getTime() const noexcept {
  return std::chrono::duration_cast<std::chrono::nanoseconds>(getTimeStamp().time_since_epoch()).count();
}

/*!
 * \brief Returns the sent NMTState
 */
NMTState Packet::getState() const noexcept { return state; }


/*!
 * \brief Returns the changes of the packet in the OD
 * \returns The created diffs as a pointer to a plf::colony
 */
const plf::colony<PacketDiff> *Packet::getDiffs() const noexcept { return &diffs; }


/*!
 * \brief Returns miscellaneous data
 * \return The miscellaneous data of this packet as a std::string
 */
std::string Packet::getMiscData() const noexcept { return miscData; }


/*!
 * \brief Returns the data that was parsed and formatted by Wireshark
 * \return The packet data as given by Wireshark as a std::string
 */
std::string Packet::getWiresharkString() const noexcept { return wiresharkSTR; }


/*!
 * \brief Returns the ID of the source (sending) node
 * \returns The ID of the source node as an unsigned integer
 */
uint8_t Packet::getSrcNode() const noexcept { return nodeSource; }


/*!
 * \brief Returns the ID of the destination (receiving) node
 * \returns The ID of the destination node as an unsigned integer
 */
uint8_t Packet::getDestNode() const noexcept { return nodeDest; }


/*!
 * \brief Returns the timestamp of this packet
 * \returns The timestamp of this packet as a TIME_POINT
 */
Packet::TIME_POINT Packet::getTimeStamp() const noexcept { return timeStamp; }

/*!
 * \brief Returns the packet offset in the file
 */
uint64_t Packet::getOffset() const noexcept { return offset; }

/*!
 * \brief Creates a new diff of data stored in 'entry' for the OD entry with given 'index' and adds it to the colony
 * \param diff  The index in the OD of the data that has been changed by the diff
 */
void Packet::addDiff(PacketDiff diff) noexcept { diffs.emplace(std::move(diff)); }
}
