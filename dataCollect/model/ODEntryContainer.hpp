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
 * \file ODEntryContainer.hpp
 * \brief Contains class ODEntry as well as all derived classes
 */

#pragma once

#include "defines.hpp"
#include "ODEntry.hpp"
#include <type_traits>
#include <vector>

namespace EPL_DataCollect {

namespace internal {

template <class C>
constexpr uint32_t calcMax(uint32_t prev) {
  if (sizeof(C) > prev)
    return sizeof(C);

  return prev;
}

/*!
 * \brief Calculates the maximum size of an OD Entry
 */
constexpr uint32_t calcSize() {
  uint32_t size = 0;

  size = calcMax<ODEntryInt>(size);
  size = calcMax<ODEntryUInt>(size);
  size = calcMax<ODEntryBool>(size);
  size = calcMax<ODEntryReal>(size);
  size = calcMax<ODEntryString>(size);
  size = calcMax<ODEntryArrayInt>(size);
  size = calcMax<ODEntryArrayUInt>(size);
  size = calcMax<ODEntryArrayBool>(size);
  size = calcMax<ODEntryArrayReal>(size);
  size = calcMax<ODEntryComplex>(size);

  return size;
}
};

/*!
 * Container class for storing any ODEntry
 */
class ODEntryContainer final {
 private:
  char data[internal::calcSize()]; //!< The data storage
  bool movedFrom = false;

  template <class C>
  C *init(ObjectDataType type) noexcept;

 public:
  ODEntryContainer() = delete;
  ~ODEntryContainer();

  ODEntryContainer(ObjectClassType type, ObjectDataType dt);
  ODEntryContainer(ObjectDataType type, ObjectType ot = ObjectType::VAR);

  ODEntryContainer(ODEntry *entry);

  ODEntryContainer(const ODEntryContainer &);
  ODEntryContainer(ODEntryContainer &&);

  ODEntryContainer &operator=(const ODEntryContainer &);
  ODEntryContainer &operator=(ODEntryContainer &&);

  static constexpr ObjectClassType getOCTbyODT(ObjectType ot, ObjectDataType dt) noexcept;

  template <class C>
  C *getData() noexcept;

  inline ODEntry *operator*() noexcept { return getData<ODEntry>(); }
  inline ODEntry *operator->() noexcept { return getData<ODEntry>(); }
};

/*!
 * \brief Returns a pointer to the data
 */
template <class C>
C *ODEntryContainer::getData() noexcept {
  static_assert(sizeof(C) <= internal::calcSize(), "Invalid data type");
  static_assert(std::is_base_of<ODEntry, C>::value, "C is not derived from ODEntry");
  return reinterpret_cast<C *>(data);
}

/*!
 * Initializes the container wit an object
 */
template <class C>
C *ODEntryContainer::init(ObjectDataType type) noexcept {
  static_assert(sizeof(C) <= internal::calcSize(), "Invalid data type");
  static_assert(std::is_base_of<ODEntry, C>::value, "C is not derived from ODEntry");

  return new (data) C(type); // This created C on top of the already allocated memory data
}

constexpr ObjectClassType ODEntryContainer::getOCTbyODT(ObjectType ot, ObjectDataType dt) noexcept {
  if (ot == ObjectType::RECORD)
    return ObjectClassType::COMPLEX;

  if (ot == ObjectType::ARRAY) {
    switch (dt) {
      case ObjectDataType::BOOLEAN: return ObjectClassType::ARRAY_BOOL;
      case ObjectDataType::INTEGER8:
      case ObjectDataType::INTEGER16:
      case ObjectDataType::INTEGER24:
      case ObjectDataType::INTEGER32:
      case ObjectDataType::INTEGER40:
      case ObjectDataType::INTEGER48:
      case ObjectDataType::INTEGER56:
      case ObjectDataType::INTEGER64: return ObjectClassType::ARRAY_INTEGER;

      case ObjectDataType::UNSIGNED8:
      case ObjectDataType::UNSIGNED16:
      case ObjectDataType::UNSIGNED24:
      case ObjectDataType::UNSIGNED32:
      case ObjectDataType::UNSIGNED40:
      case ObjectDataType::UNSIGNED48:
      case ObjectDataType::UNSIGNED56:
      case ObjectDataType::UNSIGNED64: return ObjectClassType::ARRAY_UNSIGNED;

      case ObjectDataType::REAL32:
      case ObjectDataType::REAL64: return ObjectClassType::ARRAY_REAL;
      default: break;
    }
  }

  switch (dt) {
    case ObjectDataType::BOOLEAN: return ObjectClassType::BOOL;
    case ObjectDataType::INTEGER8:
    case ObjectDataType::INTEGER16:
    case ObjectDataType::INTEGER24:
    case ObjectDataType::INTEGER32:
    case ObjectDataType::INTEGER40:
    case ObjectDataType::INTEGER48:
    case ObjectDataType::INTEGER56:
    case ObjectDataType::INTEGER64: return ObjectClassType::INTEGER;

    case ObjectDataType::UNSIGNED8:
    case ObjectDataType::UNSIGNED16:
    case ObjectDataType::UNSIGNED24:
    case ObjectDataType::UNSIGNED32:
    case ObjectDataType::UNSIGNED40:
    case ObjectDataType::UNSIGNED48:
    case ObjectDataType::UNSIGNED56:
    case ObjectDataType::UNSIGNED64: return ObjectClassType::UNSIGNED;

    case ObjectDataType::REAL32:
    case ObjectDataType::REAL64: return ObjectClassType::REAL;

    case ObjectDataType::VISIBLE_STRING:
    case ObjectDataType::OCTET_STRING:
    case ObjectDataType::UNICODE_STRING: return ObjectClassType::STRING;

    default: return ObjectClassType::COMPLEX;
  }
}
}
