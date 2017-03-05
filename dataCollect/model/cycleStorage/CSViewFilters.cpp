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
 * \file CSViewFilters.hpp
 * \brief Contains class CSViewFilters
 */

#include "CSViewFilters.hpp"

namespace EPL_DataCollect {

CSViewFilters::~CSViewFilters() {}

double CSViewFilters::getNumericValue() { return 0; }
bool   CSViewFilters::isNumericValue() { return false; }

std::unique_ptr<CycleStorageBase> CSViewFilters::clone() { return std::make_unique<CSViewFilters>(*this); }

/*!
 * \brief Returns whether a view should include the index
 * \note The set FilterType will also be used to process the result
 */
bool CSViewFilters::Filter::includeIndex(uint16_t index) noexcept {
  if (setMap.find(index) == setMap.end())
    return type == EXCLUDE;

  return setMap[index] ? type == INCLUDE : type == EXCLUDE;
}



/*!
 * \brief Creates a new filter
 * \param type the type of the filter
 * \param name the name of the filter
 * \returns the ID of the new filter
 */
uint32_t CSViewFilters::newFilter(FilterType type, std::string name) noexcept {
  filters.emplace_back(type, name);
  return static_cast<uint32_t>(filters.size() - 1);
}

/*!
 * \brief Returns all filters
 */
std::vector<CSViewFilters::Filter> CSViewFilters::getFilters() const noexcept { return filters; }

/*!
 * \brief Returns a TEMPORARY pointer to the filter
 * \warning The returned pointer may be invalid for the next cycle
 * \param id the ID of the filter
 * \returns nullptr on error
 */
CSViewFilters::Filter *CSViewFilters::getFilter(uint32_t id) const noexcept {
  return id < filters.size() ? const_cast<Filter *>(&filters[id]) : nullptr;
}



/*!
 * \brief Marks the index to be excluded or included (depending on the set type)
 * \param index The index to set
 */
void CSViewFilters::Filter::setIndex(uint16_t index) noexcept { setMap[index] = type == INCLUDE; }

/*!
 * \brief Marks the index NOT to be excluded or included (depending on the set type)
 * \param index The index to set
 */
void CSViewFilters::Filter::unSetIndex(uint16_t index) noexcept { setMap[index] = type == EXCLUDE; }

/*!
 * \brief Returns wheteer an index is already set
 * \param index the index to check
 */
bool CSViewFilters::Filter::isSet(uint16_t index) noexcept { return setMap.find(index) != setMap.end(); }
}
