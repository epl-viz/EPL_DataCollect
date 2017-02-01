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

namespace EPL_DataCollect {

OD::~OD() {}

/*!
 * \brief Checks if there is an entry with the given index
 * \return true if there is an entry with given index, false if not
 * \param index The index to check for
 */
bool OD::hasEntry(uint16_t index) const noexcept {
  auto s = entries.find(index);
  return (s != entries.end());
}

/*!
 * \brief Returns a pointer to the entry with the given index
 * \return The pointer to the requested ODEntry
 * \param  index The ID of the ODEntry
 * \todo Implement inserting new entries
 */
ODEntry *OD::getEntry(uint16_t index) noexcept {
  if (hasEntry(index)) {
    return *entries.at(index);
  } else {
    return nullptr;
  }
}

/*!
 * \brief Returns a pointer to the OD Description
 * \return The pointer to the OD Description
 */
ODDescription *OD::getODDesc() noexcept { return &odDesc; }
}
