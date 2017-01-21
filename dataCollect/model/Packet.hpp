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
 * \todo IMPLEMENT
 */


#pragma once



#include "defines.hpp"
#include <vector>
#include "ODDescription.hpp"
#include "PacketDiff.hpp"

namespace EPL_DataCollect {

/*!
 * \brief The type of a packet
 * \todo IMPLEMENT
 */
enum PacketType { PKT_A };

/*!
  * class Packet
  * \brief Represents a packet on the Ethernet bus
  */
class Packet {
 public:
  // Constructors/Destructors
  //


  /*!
   * Empty Constructor
   */
  Packet();

  /*!
   * Empty Destructor
   */
  virtual ~Packet();

  Packet( const Packet & ) = default;
  Packet( Packet && )      = default;

  Packet &operator=( const Packet & ) = default;
  Packet &operator=( Packet && ) = default;

  // Static Public attributes
  //

  // Public attributes
  //


  // Public attribute accessor methods
  //


  // Public attribute accessor methods
  //



  /*!
   * \brief Returns the packet type
   * \return PacketType
   */
  PacketType getType() { return PKT_A; }


  /*!
   * \brief Returns the changes of the packet in the OD
   * \return std::vector<PacketDiff>
   */
  std::vector<PacketDiff> getDiffs() { return std::vector<PacketDiff>(); }


  /*!
   * \brief Returns other data as a std::string
   * \return std::string
   */
  std::string getOtherData() { return ""; }


  /*!
   * \brief Returns the wireshark data parsed and formated
   * \return std::string
   */
  std::string getWiresharkString() { return ""; }


  /*!
   * \brief Returns a pointer to the parsed OD information
   * \return ODDescription *
   */
  ODDescription *getODDesc() { return odDesc; }

 protected:
  // Static Protected attributes
  //

  // Protected attributes
  //

 public:
  // Protected attribute accessor methods
  //

 protected:
 public:
  // Protected attribute accessor methods
  //

 protected:
 private:
  // Static Private attributes
  //

  // Private attributes
  //

  ODDescription *odDesc;

 public:
  // Private attribute accessor methods
  //

 private:
 public:
 private:
};
}
