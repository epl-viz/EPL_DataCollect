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
 * \file OD.hpp
 * \brief Contains class OD
 */


#pragma once


#include "defines.hpp"

#include "ODDescription.hpp"
#include "ODEntryContainer.hpp"
#include <unordered_map>


namespace EPL_DataCollect {

class CycleBuilder;

/*!
  * class OD
  * \brief Representation of the Object Dictionary of a Node
  *
  * Each entry in the Object Dictionary has, per definition, a specified 16-bit index.
  * This class realizes this by providing a map that uses a 16-bit unsigned integer for keys and the class
  * ODEntryContainer, which encapsules generic data, for values.
  */
class OD {
 private:
  std::unordered_map<uint16_t, ODEntryContainer> entries; //!< \brief The entries in the Object Dictionary
  ODDescription odDesc; //!< \brief The description of the Object Dictionary of the Node

 public:
  OD() = default;
  virtual ~OD();

  OD(const OD &) = default;
  OD(OD &&)      = default;

  OD &operator=(const OD &) = default;
  OD &operator=(OD &&) = default;

  mockable bool hasEntry(uint16_t index) const noexcept;

  mockable ODDescription *getODDesc() noexcept;
  mockable ODEntry *getEntry(uint16_t index) noexcept;

  friend class CycleBuilder;

 private:
  mockable ODEntryContainer constructODEntry(ODEntryDescription *entryDesc) const noexcept;
};
}
