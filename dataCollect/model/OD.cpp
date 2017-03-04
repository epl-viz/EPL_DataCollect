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
 * \file OD.cpp
 * \brief Contains class OD
 */

#include "OD.hpp"
#include "EPLEnum2Str.hpp"
#include <iostream>

namespace EPL_DataCollect {

OD::~OD() {}

/*!
 * \brief Checks if there is an entry with the given index
 * \returns true if there is an entry with given index, false if not
 * \param index The index to check for
 */
bool OD::hasEntry(uint16_t index) const noexcept {
  auto s = entries.find(index);
  return (s != entries.end());
}

/*!
 * \brief Returns a pointer to the entry with the given index
 *
 * This function will insert values which are not yet written into the entry map, provided that there is an
 * ODEntryDescription for the requested Entry.
 *
 * \returns The pointer to the requested ODEntry or a nullptr if the value cannot be loaded
 * \param  index The ID of the ODEntry
 */
ODEntry *OD::getEntry(uint16_t index) noexcept {
  // Check if entry has to be loaded in first
  if (!hasEntry(index)) {
    // Check if the OD Description contains the missing entry
    if (!odDesc->exists(index))
      return nullptr; // The value does not exist

    ODEntryDescription *entryDesc = odDesc->getEntry(index);     // Retrieve description for the entry
    ODEntryContainer    entry     = constructODEntry(entryDesc); // Construct requested ODEntryContainer

    entries.insert({index, entry}); // Write the ODEntry into the map
  }

  // At this point, the entry is guaranteed to be in the OD
  return *entries.at(index);
}

/*!
 * \brief Returns a pointer to the OD Description
 * \returns The pointer to the OD Description
 */
ODDescription *OD::getODDesc() noexcept { return odDesc.get(); }

/*!
 * \brief Constructs an ODEntry wrapped into an ODEntryContainer from the given ODEntryDescription
 * \param entryDesc The ODEntryDescription to base the ODEntry on
 * \returns The ODEntryContainer of the constructed ODEntry
 */
ODEntryContainer OD::constructODEntry(ODEntryDescription *entryDesc) const noexcept {
  ObjectType      ot  = entryDesc->type;                        // Retrieve the type of the entry
  ObjectDataType  odt = entryDesc->dataType;                    // Retrieve the data type of the entry
  ObjectClassType oct = ODEntryContainer::getOCTbyODT(ot, odt); // Convert data type to class type

  return ODEntryContainer(oct, odt);
}
}
