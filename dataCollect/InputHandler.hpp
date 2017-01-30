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
 * \todo IMPLEMENT
 */

#pragma once

#include "defines.hpp"

#include "Packet.hpp"
#include <vector>

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
 public:
  // Constructors/Destructors
  //


  /*!
   * Empty Constructor
   */
  InputHandler();

  /*!
   * Empty Destructor
   */
  virtual ~InputHandler();

  // Static Public attributes
  //

  // Public attributes
  //


  // Public attribute accessor methods
  //


  // Public attribute accessor methods
  //



  /*!
   * \brief Returns all packets within a complete cycle.
   * \note Always call waitForCycle first
   * Throws if the cycle does not exist.
   *
   * \return std::vector<Packet>
   * \param  cycleNum The number of the cycle
   */
  std::vector<Packet> getCyclePackets(unsigned int cycleNum) {
    (void)cycleNum;
    return std::vector<Packet>();
  }


  /*!
   * \brief Waits until the specified cycle is available
   * \note This function should always be called before getCyclePackets
   * Returns false on timeout.
   * \return bool
   * \param  num The number of the cycle to wait for
   * \param  timeout The timeout in milliseconds (0 for no timeout)
   */
  bool waitForCycle(unsigned int num, unsigned long int timeout = 0) {
    (void)num;
    (void)timeout;
    return false;
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

  std::vector<Packet> packets;

 public:
  // Private attribute accessor methods
  //

 private:
 public:
  // Private attribute accessor methods
  //


 private:
};
}
