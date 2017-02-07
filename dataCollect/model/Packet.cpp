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

namespace EPL_DataCollect {

/*!
 * \brief The constructor
 * \param t         The type of the packet
 * \param cID       The command ID of the packet
 * \param oDesc     A pointer to the odDescription
 * \param wireSTR   The wireshark string representation of the packet
 * \param misc     Miscellaneous data
 * \param src       The ID of the sending node
 * \param dest      The ID of the receiving node
 * \param ts        The timestamp of the packet
 * \param tID       The Packet transaction ID
 * \param nSegments The number of segments this packet was split
 */
Packet::Packet(PacketType     t,
               CommandID      cID,
               ODDescription *oDesc,
               std::string    wireSTR,
               std::string    misc,
               uint32_t       src,
               uint32_t       dest,
               TIME_POINT     ts,
               uint8_t        tID,
               uint32_t       nSegments)
    : type(t),
      commandID(cID),
      odDesc(oDesc),
      wiresharkSTR(wireSTR),
      miscData(misc),
      nodeSource(src),
      nodeDest(dest),
      timeStamp(ts),
      transactionID(tID),
      numOfSegments(nSegments) {}

Packet::~Packet() {}

bool Packet::operator==(const Packet &r) const {
  return ((type == r.type) && (commandID == r.commandID) && (odDesc == r.odDesc) && (wiresharkSTR == r.wiresharkSTR) &&
          (miscData == r.miscData) && (nodeSource == r.nodeSource) && (timeStamp == r.timeStamp) &&
          (transactionID == r.transactionID) && (numOfSegments == r.numOfSegments));
}

/*!
 * \brief Returns the packet type
 * \return The packet type as a PacketType
 */
PacketType Packet::getType() const noexcept { return type; }


/*!
 * \brief Returns the Command ID of the packet
 * \returns The Command ID of this packet as a CommandID
 */
CommandID Packet::getCommandID() const noexcept { return commandID; }


/*!
 * \brief Returns the transaction ID of the packet
 * \returns The transaction ID of this packet as an unsigned char
 */
uint8_t Packet::getTransactionID() const noexcept { return transactionID; }


/*!
 * \brief Returns the number of segments this packet is split into
 * \returns The number of segments this packet is split into as an unsigned integer
 */
uint32_t Packet::getNumSegments() const noexcept { return numOfSegments; }


/*!
 * \brief Returns the changes of the packet in the OD
 * \returns The created diffs as a pointer to a plf::colony
 */
plf::colony<PacketDiff> *Packet::getDiffs() noexcept { return &diffs; }


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
 * \brief Returns a pointer to the parsed OD information (The OD Description)
 * \return The pointer to the ODDescription of this packet
 */
ODDescription *Packet::getODDesc() noexcept { return odDesc; }


/*!
 * \brief Returns the ID of the source (sending) node
 * \returns The ID of the source node as an unsigned integer
 */
uint32_t Packet::getSrcNode() const noexcept { return nodeSource; }


/*!
 * \brief Returns the ID of the destination (receiving) node
 * \returns The ID of the destination node as an unsigned integer
 */
uint32_t Packet::getDestNode() const noexcept { return nodeDest; }


/*!
 * \brief Returns the timestamp of this packet
 * \returns The timestamp of this packet as a TIME_POINT
 */
Packet::TIME_POINT Packet::getTimeStamp() const noexcept { return timeStamp; }

/*!
 * \brief Creates a new diff of data stored in 'entry' for the OD entry with given 'index' and adds it to the colony
 * \param index The index in the OD of the data that has been changed by the diff
 * \param entry The container of the data to create the diff for
 */
void Packet::addDiff(uint16_t index, ODEntryContainer entry) noexcept { diffs.emplace(index, std::move(entry)); }
}
