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
 * \file EventLog.cpp
 * \brief Contains class EventLog
 */

#include "EventLog.hpp"

namespace EPL_DataCollect {

EventLog::~EventLog() {}

/*!
 * \brief Returns a unique ID that should be used in pollEvents
 * \return unsigned int
 */
uint32_t EventLog::getAppID() noexcept {
  std::lock_guard<std::mutex> lock(accessMutex);
  return nextAppID++;
}

/*!
 * \brief Returns all events triggered after the last pollEvents call with the same
 * appID
 * \return std::vector<EventBase*>
 * \param  appID Identifies the user polling the events
 */
std::vector<EventBase *> EventLog::pollEvents(uint32_t appID) noexcept {
  std::lock_guard<std::mutex> lock(accessMutex);
  if (pollList.find(appID) == pollList.end())
    pollList[appID] = 0;

  std::vector<EventBase *> evList;

  uint32_t i;
  for (i = pollList[appID]; i < events.size(); i++) {
    evList.push_back(events[i].get());
  }

  pollList[appID] = i;
  return evList;
}


/*!
 * \brief Returns all events triggered within the cycle range
 * Use -1 for start/end to select the first/last cycle
 * \return std::vector<EventBase*>
 * \param  begin The begin of the cycle range
 * \param  end The end of the cycle range
 */
std::vector<EventBase *> EventLog::getEventsInRange(int begin, int end) noexcept {
  std::lock_guard<std::mutex> lock(accessMutex);
  std::vector<EventBase *>    evList;

  uint32_t cS;
  for (auto &i : events) {
    i->getCycleRange(&cS, nullptr);

    if (static_cast<int>(cS) < begin)
      continue;

    if (static_cast<int>(cS) > end && end >= 0)
      continue;

    evList.push_back(i.get());
  }

  return evList;
}

/*!
 * \brief Returns all events triggered
 * Wrapper for getEventsInRange
 * \return std::vector<EventBase*>
 */
std::vector<EventBase *> EventLog::getAllEvents() noexcept { return getEventsInRange(-1, -1); }

/*!
 * \brief Adds a new event to the event log
 * \note This function will increase the cycle range of past events, if possible
 * \param ev The event to add
 */
void EventLog::addEvent(std::unique_ptr<EventBase> ev) noexcept {
  std::lock_guard<std::mutex> lock(accessMutex);

  uint32_t evStart, evEnd;
  uint32_t endRange;
  ev->getCycleRange(&evStart, &evEnd);

  if (evStart < nextCycle)
    return;

  auto it = latestEvents.begin();
  while (it != latestEvents.end()) {
    auto i = *it;
    i->getCycleRange(nullptr, &endRange);

    if (evStart == endRange + 1) {
      if (*ev.get() == *i) {
        i->updateRange(-1, static_cast<int>(evEnd));
        return;
      }
    }
    // remove outdated / cold events
    else if (static_cast<int64_t>(endRange) <= static_cast<int64_t>(evStart) - static_cast<int64_t>(MAX_LATEST_KEEP)) {
      it = latestEvents.erase(it);
      continue;
    }

    ++it;
  }

  events.emplace_back(std::move(ev));
  latestEvents.insert(events.back().get());
}

void EventLog::setNextCycle(uint32_t cycle) { nextCycle = cycle > nextCycle ? cycle : nextCycle; }
}
