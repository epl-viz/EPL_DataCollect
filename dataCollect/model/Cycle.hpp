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
 * \file Cycle.hpp
 * \brief Contains class Cycle
 * \todo IMPLEMENT
 */


#pragma once



#include "defines.hpp"
#include "CycleStorageBase.hpp"
#include "EventBase.hpp"
#include "Node.hpp"
#include "Packet.hpp"

namespace EPL_DataCollect {

/*!
  * class Cycle
  * \brief Root entity of the ethernetPOWERLINK model
  *
  * Stores all information available during (after completion) of the specific
  * cycle.
  */
class Cycle {
 public:
  // Constructors/Destructors
  //


  /*!
   * Empty Constructor
   */
  Cycle();

  /*!
   * Empty Destructor
   */
  virtual ~Cycle();

  Cycle(const Cycle &) = default;
  Cycle(Cycle &&)      = default;

  Cycle &operator=(const Cycle &) = default;
  Cycle &operator=(Cycle &&) = default;

  // Static Public attributes
  //

  // Public attributes
  //


  // Public attribute accessor methods
  //


  // Public attribute accessor methods
  //



  /*!
   * \brief Returns the number of nodes
   * \return unsigned int
   */
  unsigned int getNumNodes() { return 0; }


  /*!
   * \brief Returns the specified node
   * \return Node
   * \param  node The ID / index of the node
   */
  Node getNode(unsigned int node) {
    (void)node;
    return Node();
  }


  /*!
   * \brief Returns all events active in the cycle
   * \return std::vector<EventBase*>
   */
  std::vector<EventBase *> getActiveEvents() { return std::vector<EventBase *>(); }


  /*!
   * \brief Returns all packets in the cycle
   * \return std::vector<Packet>
   */
  std::vector<Packet> getPackets() { return std::vector<Packet>(); }


  /*!
   * \brief Returns the cycle number of this Cycle
   * \return unsigned int
   */
  unsigned int getCycleNum() { return 0; }


  /*!
   * \brief Returns a pointer to the cycle storage corresponding to the ID
   * \note Returns nullptr on error
   * \return CycleStorageBase *
   * \param  id The ID of the storage
   */
  CycleStorageBase *getCycleStorage(std::string id) {
    (void)id;
    return nullptr;
  }

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

  EventBase *events;
  Packet     packets;

 public:
  // Private attribute accessor methods
  //

 private:
 public:
  // Private attribute accessor methods
  //


 private:
  /*!
   * \brief Updates the packet list and increases the cycle counter.
   * This will clear the old packet list and increment the cycle counter by one.
   * \note This function will NOT change the OD entries!
   *
   * C++: friend class CycleBuilder
   * \param  newPackets The packets to apply
   */
  void updatePackets(std::vector<Packet> newPackets) { (void)newPackets; }
};
}
