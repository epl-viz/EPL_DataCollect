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
 */

#pragma once

#include "defines.hpp"
#include "EventBase.hpp"
#include <memory>
#include <mutex>
#include <plf_colony.h>
#include <unordered_map>
#include <vector>

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
  * \warning It is assumed that events are added in chronological (based on the cycle number) order!
  *
  * \par Continuous events
  * Special events that should be active until the next event of the same type is
  * triggered (for example GEvImage) can have a flag set that activates this special
  * treatment.
  */
class EventLog {
  typedef std::vector<std::unique_ptr<EventBase>> EV_CONTAINER;
  typedef std::unordered_map<uint32_t, uint32_t> POLL_MAP;

  static const uint32_t MAX_LATEST_KEEP = 5;

 private:
  std::mutex               accessMutex;
  EV_CONTAINER             events;
  POLL_MAP                 pollList;
  plf::colony<EventBase *> latestEvents;

  uint32_t nextAppID = 0;

 public:
  EventLog() = default;
  virtual ~EventLog();

  EventLog(const EventLog &) = default;
  EventLog(EventLog &&)      = default;

  EventLog &operator=(const EventLog &) = default;
  EventLog &operator=(EventLog &&) = default;

  mockable std::vector<EventBase *> pollEvents(uint32_t appID = 0) noexcept;
  mockable uint32_t getAppID() noexcept;
  mockable std::vector<EventBase *> getEventsInRange(int begin = -1, int end = -1) noexcept;
  mockable std::vector<EventBase *> getAllEvents() noexcept;
  mockable void addEvent(std::unique_ptr<EventBase> ev) noexcept;
};
}
