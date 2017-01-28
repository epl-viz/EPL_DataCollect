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
 * \file EventBase.cpp
 * \brief Contains class EventBase
 */

#include "EventBase.hpp"
#include "Cycle.hpp"
#include "EPLEnum2Str.hpp"

namespace EPL_DataCollect {


/*!
 * This constructor must be called by a child class
 * \param evType The event type
 * \param evPluginID The plugin ID
 * \param evName The name of the event
 * \param evDesc The description
 * \param evFlags Flags for the new event \sa EvFlags
 * \param cycle Pointer to the first cycle the event occurred
 * \param evIndices Affected indices
 */
EventBase::EventBase(EvType      evType,
                     std::string evPluginID,
                     std::string evName,
                     std::string evDesc,
                     uint64_t    evFlags,
                     Cycle *     cycle,
                     INDEX_MAP   evIndices)
    : EventBase(evType, EPLEnum2Str::toStr(evType), evPluginID, evName, evDesc, evFlags, cycle, evIndices) {}


/*!
 * This constructor must be called by a child class
 * \param evType The event type
 * \param evTypeSTR The optional Type as a string for non standard events
 * \param evPluginID The plugin ID
 * \param evName The name of the event
 * \param evDesc The description
 * \param evFlags Flags for the new event \sa EvFlags
 * \param cycle Pointer to the first cycle the event occurred
 * \param evIndices Affected indices
 */
EventBase::EventBase(EvType      evType,
                     std::string evTypeSTR,
                     std::string evPluginID,
                     std::string evName,
                     std::string evDesc,
                     uint64_t    evFlags,
                     Cycle *     cycle,
                     INDEX_MAP   evIndices)
    : type(evType),
      typeSTR(evTypeSTR),
      flags(evFlags),
      pluginID(evPluginID),
      name(evName),
      description(evDesc),
      firstCycle(UINT32_MAX),
      lastCycle(UINT32_MAX),
      affectedIndices(evIndices) {

  if (cycle != nullptr) {
    firstCycle = cycle->getCycleNum();
    lastCycle  = firstCycle;
  }
}


/*!
  * \brief Get the type of the event
  * \return EvType
  */
EvType EventBase::getType() const noexcept { return type; }


/*!
 * \brief Get the type of the event as a string
 * \return The type of the event represented as a std::string
 */
std::string EventBase::getTypeAsString() const noexcept { return typeSTR; }


/*!
 * \brief Get the name of the Event
 * \return The name as a std::string
 */
std::string EventBase::getName() const noexcept { return name; }


/*!
 * \brief Returns the description of the event
 * \return The description as a std::string
 */
std::string EventBase::getDescription() const noexcept { return description; }


/*!
 * \brief Returns the ID of the plugin triggering the event
 * \return The plugin ID as a std::string
 */
std::string EventBase::getPluginID() const noexcept { return pluginID; }


/*!
 * \brief Returns the event type flags
 * \return unsigned long int
 */
uint64_t EventBase::getEventFlags() const noexcept { return flags; }


/*!
 * \brief Returns the cycle range
 * \param[out] first The first cycle where the event occurred
 * \param[out] last The last cycle where the event occurred
 */
void EventBase::getCycleRange(uint32_t *first, uint32_t *last) noexcept {
  if (first != nullptr)
    *first = firstCycle;

  if (last != nullptr)
    *last = lastCycle;
}


/*!
 * \brief Updates the cycle range
 * \note -1 represents no change
 * \param  first The first cycle
 * \param  last The last cycle
 */
void EventBase::updateRange(int first, int last) noexcept {
  if (first >= 0)
    firstCycle = static_cast<uint32_t>(first);

  if (last >= 0)
    lastCycle = static_cast<uint32_t>(last);
}


/*!
 * \brief Returns a list / map of affected OD indices by this event.
 * Also contains a descriptive std::string
 * \return unordered_map<int, std::string>
 */
EventBase::INDEX_MAP EventBase::getAffectedIndices() const noexcept { return affectedIndices; }


/*!
 *\brief Returns a pointer to Event specific metadata
 * Returns nullptr when no data is specified or the child class does not overwrite
 * this function
 *
 * \return unordered_map<std::string,std::string>*
 */
EventBase::MDATA_MAP *EventBase::getMetadata() noexcept { return nullptr; }

/*!
 * \brief Checks if two events are SIMILAR enough to combine
 */
bool EventBase::operator==(const EventBase &b) const {
  return type == b.type && typeSTR == b.typeSTR && flags == b.flags && pluginID == b.pluginID && name == b.name &&
         description == b.description;
}
}
