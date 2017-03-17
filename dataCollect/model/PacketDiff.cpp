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
 * \file PacketDiff.cpp
 * \brief Contains class PacketDiff
 */

#include "PacketDiff.hpp"
#include "OD.hpp"
#include <iostream>

namespace EPL_DataCollect {

PacketDiff::PacketDiff(uint16_t index, uint8_t sIndex, uint64_t entry, BitLength length)
    : odIndex(index), subIndex(sIndex), valInt(entry), bitLength(length) {}

PacketDiff::PacketDiff(uint16_t index, uint8_t sIndex, double entry)
    : odIndex(index), subIndex(sIndex), valReal(entry) {}

PacketDiff::~PacketDiff() {}

/*!
 * \brief Get the Index of the OD
 * \returns The Index
 */
uint16_t PacketDiff::getIndex() const noexcept { return odIndex; }

/*!
 * \brief Returns the subIndex
 */
uint8_t PacketDiff::getSubIndex() const noexcept { return subIndex; }

/*!
 * \brief Get The new OD Entry
 * \returns a ODEntryContainer
 */
ODEntryContainer PacketDiff::getEntry(OD *od) const noexcept {
  ODEntry *entryPTR = od->getEntry(odIndex);

  if (entryPTR) {
    ODEntryContainer entry(entryPTR);

    switch (entry->getType()) {
      case ObjectClassType::INTEGER:
        switch (bitLength) {
          case L8: entry.getData<ODEntryInt>()->data  = static_cast<int64_t>(static_cast<int8_t>(valInt)); break;
          case L16: entry.getData<ODEntryInt>()->data = static_cast<int64_t>(static_cast<int16_t>(valInt)); break;
          case L32: entry.getData<ODEntryInt>()->data = static_cast<int64_t>(static_cast<int32_t>(valInt)); break;
          case L64: entry.getData<ODEntryInt>()->data = static_cast<int64_t>(static_cast<int64_t>(valInt)); break;
        }
        break;
      case ObjectClassType::UNSIGNED: entry.getData<ODEntryUInt>()->data = valInt; break;
      case ObjectClassType::BOOL: entry.getData<ODEntryBool>()->data     = valInt != 0; break;
      case ObjectClassType::REAL: entry.getData<ODEntryReal>()->data     = valReal; break;
      case ObjectClassType::STRING: entry.getData<ODEntryString>()->data = std::to_string(valInt); break;
      case ObjectClassType::ARRAY_INTEGER:
        switch (bitLength) {
          case L8:
            entry.getData<ODEntryArrayInt>()->data[subIndex] = static_cast<int64_t>(static_cast<int8_t>(valInt));
            break;
          case L16:
            entry.getData<ODEntryArrayInt>()->data[subIndex] = static_cast<int64_t>(static_cast<int16_t>(valInt));
            break;
          case L32:
            entry.getData<ODEntryArrayInt>()->data[subIndex] = static_cast<int64_t>(static_cast<int32_t>(valInt));
            break;
          case L64:
            entry.getData<ODEntryArrayInt>()->data[subIndex] = static_cast<int64_t>(static_cast<int64_t>(valInt));
            break;
        }
        break;
      case ObjectClassType::ARRAY_UNSIGNED: entry.getData<ODEntryArrayUInt>()->data[subIndex] = valInt; break;
      case ObjectClassType::ARRAY_BOOL: entry.getData<ODEntryArrayBool>()->data[subIndex]     = valInt != 0; break;
      case ObjectClassType::ARRAY_REAL: entry.getData<ODEntryArrayReal>()->data[subIndex]     = valReal; break;
      case ObjectClassType::COMPLEX:
        auto                    desc = od->getODDesc()->getEntry(odIndex);
        ODEntryComplexContainer temp;

        if (subIndex < desc->subEntries.size()) {
          temp.init(desc->subEntries[subIndex].dataType);
        } else {
          temp.init(ObjectDataType::UNSIGNED64);
        }

        switch (temp->getType()) {
          case ObjectClassType::INTEGER:
            switch (bitLength) {
              case L8: entry.getData<ODEntryInt>()->data  = static_cast<int64_t>(static_cast<int8_t>(valInt)); break;
              case L16: entry.getData<ODEntryInt>()->data = static_cast<int64_t>(static_cast<int16_t>(valInt)); break;
              case L32: entry.getData<ODEntryInt>()->data = static_cast<int64_t>(static_cast<int32_t>(valInt)); break;
              case L64: entry.getData<ODEntryInt>()->data = static_cast<int64_t>(static_cast<int64_t>(valInt)); break;
            }
            break;
          case ObjectClassType::UNSIGNED: temp.getData<ODEntryUInt>()->data = valInt; break;
          case ObjectClassType::BOOL: temp.getData<ODEntryBool>()->data     = valInt != 0; break;
          case ObjectClassType::REAL: temp.getData<ODEntryReal>()->data     = valReal; break;
          default: temp->setFromString(std::to_string(valInt), subIndex);
        }

        entry.getData<ODEntryComplex>()->data[subIndex] = temp;
        break;
    }

    return entry;
  } else {
    ODEntryContainer fallback(ObjectDataType::UNSIGNED64, subIndex == 0 ? ObjectType::VAR : ObjectType::ARRAY);

    if (fallback->getType() == ObjectClassType::UNSIGNED)
      fallback.getData<ODEntryUInt>()->data = valInt;
    else if (fallback->getType() == ObjectClassType::ARRAY_UNSIGNED)
      fallback.getData<ODEntryArrayUInt>()->data[subIndex] = valInt;

    return fallback;
  }
}
}
