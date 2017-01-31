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
 * \file Packet.cpp
 * \brief Contains class Packet
 */

#include "Packet.hpp"

namespace EPL_DataCollect {

/*!
 * \brief The constructor
 * \param t         The type of the packet
 * \param cID       the command ID of the packet
 * \param oDesc     a pointer to the odDescription
 * \param wireSTR   The wireshark string representation of the packet
 * \param other     Other data
 * \param src       The ID of the sending node
 * \param dest      The ID of the recieving node
 * \param ts        The timeStamp of the packet
 * \param tID       The Packet transaction ID
 * \param nSegments The number of segments this packet was split
 */
Packet::Packet(PacketType     t,
               CommandID      cID,
               ODDescription *oDesc,
               std::string    wireSTR,
               std::string    other,
               uint32_t       src,
               uint32_t       dest,
               TIME_POINT     ts,
               uint8_t        tID,
               uint32_t       nSegments)
    : type(t),
      commandID(cID),
      odDesc(oDesc),
      wiresharkSTR(wireSTR),
      otherData(other),
      nodeSource(src),
      nodeDest(dest),
      timeStamp(ts),
      transactionID(tID),
      numOfSegments(nSegments) {}

Packet::~Packet() {}

/*!
 * \brief Returns the packet type
 * \return PacketType
 */
PacketType Packet::getType() const noexcept { return type; }


/*!
 * \brief Returns the Command ID of the packet
 */
CommandID Packet::getCommandID() const noexcept { return commandID; }


/*!
 * \brief Returns the transaction id of the packet
 */
uint8_t Packet::getTransactionID() const noexcept { return transactionID; }


/*!
 * \brief Returns the number of segments this packet was split
 */
uint32_t Packet::getNumSegments() const noexcept { return numOfSegments; }


/*!
 * \brief Returns the changes of the packet in the OD
 * \returns the diffs as a pointer to a plf::colony
 */
plf::colony<PacketDiff> *Packet::getDiffs() noexcept { return &diffs; }


/*!
 * \brief Returns other data as a std::string
 * \return std::string
 */
std::string Packet::getOtherData() const noexcept { return otherData; }


/*!
 * \brief Returns the wireshark data parsed and formated
 * \return std::string
 */
std::string Packet::getWiresharkString() const noexcept { return wiresharkSTR; }


/*!
 * \brief Returns a pointer to the parsed OD information
 * \return ODDescription *
 */
ODDescription *Packet::getODDesc() noexcept { return odDesc; }


/*!
 * \brief Returns the ID of the sending node
 */
uint32_t Packet::getSrcNode() const noexcept { return nodeSource; }


/*!
 * \brief Returns the ID of the recieving node
 */
uint32_t Packet::getDestNode() const noexcept { return nodeDest; }


/*!
 * \brief Returns the time Stamp
 */
Packet::TIME_POINT Packet::getTimeStamp() const noexcept { return timeStamp; }


void Packet::addDiff(uint16_t index, ODEntryContainer entry) noexcept { diffs.emplace(index, std::move(entry)); }
}
