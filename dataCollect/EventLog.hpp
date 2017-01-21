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
 * \file EventLog.hpp
 * \brief Contains class EventLog
 * \todo IMPLEMENT
 */

#pragma once

#include "defines.hpp"
#include <vector>
#include "EventBase.hpp"

namespace EPL_DataCollect {

/*!
  * class EventLog
  * \brief Container for all events of a CaptureInstance
  *
  * The EventLog class is the main container for all events and has functions to
  * access them.
  * Furthermore it has logic for:
  *
  *  - detecting and handling duplicate events
  *  - continuation of events (extending the cycle range of previous events)
  *  - replacing events (see continuous events)
  *
  * \par Continuous events
  * Special events that should be active until the next event of the same type is
  * triggered (for example GEvImage) can have a flag set that activates this special
  * treatment.
  */
class EventLog {
 public:
  // Constructors/Destructors
  //


  /*!
   * Empty Constructor
   */
  EventLog();

  /*!
   * Empty Destructor
   */
  virtual ~EventLog();

  // Static Public attributes
  //

  // Public attributes
  //


  // Public attribute accessor methods
  //


  // Public attribute accessor methods
  //



  /*!
   * \brief Returns all events triggered after the last pollEvents call with the same
   * appID
   * \return std::vector<EventBase*>
   * \param  appID Identifies the user polling the events
   */
  std::vector<EventBase *> pollEvents( unsigned int appID = 0 ) {
    (void)appID;
    return std::vector<EventBase *>();
  }


  /*!
   * \brief Returns a unique ID that should be used in pollEvents
   * \return unsigned int
   */
  unsigned int getAppID() { return 0; }


  /*!
   * \brief Returns all events triggered within the cycle range
   * Use -1 for start/end to select the first/last cycle
   * \return std::vector<EventBase*>
   * \param  begin The begin of the cycle range
   * \param  end The end of the cycle range
   */
  std::vector<EventBase *> getEventsInRange( int begin = -1, int end = -1 ) {
    (void)begin;
    (void)end;
    return std::vector<EventBase *>();
  }


  /*!
   * \brief Returns all events triggered
   * Wrapper for getEventsInRange
   * \return std::vector<EventBase*>
   */
  std::vector<EventBase *> getAllEvents() { return getEventsInRange( -1, -1 ); }

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
