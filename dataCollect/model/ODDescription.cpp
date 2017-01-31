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
 * \file ODDescription.cpp
 * \brief Contains class ODDescription
 */

#include "ODDescription.hpp"

namespace EPL_DataCollect {

/*!
 * \brief Checks whether an index is already set
 * \param index The index to check
 * \returns Whether the entry description is already set
 */
bool ODDescription::exists(uint16_t index) noexcept {
  auto found = entries.find(index);
  if (found == entries.end())
    return false;

  return true;
}

/*!
 * \brief Adds a new entry to the OD description
 * \note This function can not override existing entries
 * \param index the index of the new entry description
 * \param desc the entry description
 * \returns false when the entry already exists
 */
bool ODDescription::setEntry(uint16_t index, ODEntryDescription desc) noexcept {
  if (exists(index))
    return false;

  entries.insert({index, desc});
  return true;
}

/*!
 * \brief Overrides an OD entry description
 * \note This function can not insert new entries
 * \param index the index of the new entry description
 * \param desc the entry description
 * \returns false when the entry does not exist
 */
bool ODDescription::overrideEntry(uint16_t index, ODEntryDescription desc) noexcept {
  if (!exists(index))
    return false;

  entries.at(index) = desc;
  return true;
}

/*!
 * \brief Returns a pointer to an entry description
 * \note This function can not insert new entries
 * \param index the index of the entry description
 * \returns nullptr if the entry does not exist
 */
ODEntryDescription *ODDescription::getEntry(uint16_t index) noexcept {
  if (!exists(index))
    return nullptr;

  return &entries.at(index);
}

/*!
 * \brief Applies a new description set on top of this one
 * \note already existing entries will be overwritten
 * \param desc the new description set
 * \todo Use c++17 merge
 */
void ODDescription::applyDesc(ODDescription &desc) noexcept {
  for (auto &i : desc.getEntries()) {
    if (exists(i.first)) {
      overrideEntry(i.first, i.second);
    } else {
      setEntry(i.first, i.second);
    }
  }
}


ODDescription::MAP &ODDescription::getEntries() noexcept { return entries; }
}
