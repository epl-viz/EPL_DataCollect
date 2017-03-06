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
 * \file ODEntryComplex.hpp
 * \brief Contains class ODEntryComplex as well as its container class
 */

#pragma once

#include "ODEntry.hpp"
#include <string.h>


namespace EPL_DataCollect {
namespace internal {

template <class C>
constexpr uint32_t calcMaxComplex(uint32_t prev) {
  if (sizeof(C) > prev)
    return sizeof(C);

  return prev;
}

/*!
 * \brief Calculates the maximum size of an OD Entry
 */
constexpr uint32_t calcSizeComplex() {
  uint32_t size = 0;

  size = calcMaxComplex<ODEntryInt>(size);
  size = calcMaxComplex<ODEntryUInt>(size);
  size = calcMaxComplex<ODEntryBool>(size);
  size = calcMaxComplex<ODEntryReal>(size);

  return size;
}
};

class ODEntryComplexContainer {
 private:
  char data[internal::calcSizeComplex()]; //!< The data storage
  bool isInit = false;

  template <class C>
  inline C *initCC(ObjectDataType type) noexcept;

 public:
  inline ODEntryComplexContainer() = default;
  inline ~ODEntryComplexContainer();

  inline ODEntryComplexContainer(ObjectClassType type, ObjectDataType dt);
  inline ODEntryComplexContainer(ObjectDataType type, ObjectType ot = ObjectType::VAR);

  inline ODEntryComplexContainer(ODEntry *entry);

  inline ODEntryComplexContainer(const ODEntryComplexContainer &);
  inline ODEntryComplexContainer(ODEntryComplexContainer &&);

  inline ODEntryComplexContainer &operator=(const ODEntryComplexContainer &);
  inline ODEntryComplexContainer &operator=(ODEntryComplexContainer &&);
  inline ODEntryComplexContainer &operator=(ODEntry *entry);

  inline ODEntry *init(ObjectClassType type, ObjectDataType dt);
  inline ODEntry *init(ObjectDataType type, ObjectType ot = ObjectType::VAR);

  inline bool getIsInit() const noexcept { return isInit; }

  inline static constexpr ObjectClassType getOCTbyODT(ObjectType ot, ObjectDataType dt) noexcept;

  template <class C>
  inline C *getData() noexcept;

  inline ODEntry *operator*() noexcept { return getData<ODEntry>(); }
  inline ODEntry *operator->() noexcept { return getData<ODEntry>(); }
};

/*!
 * \brief Returns a pointer to the data
 */
template <class C>
C *ODEntryComplexContainer::getData() noexcept {
  static_assert(sizeof(C) <= internal::calcSizeComplex(), "Invalid data type");
  static_assert(std::is_base_of<ODEntry, C>::value, "C is not derived from ODEntry");

  if (!isInit)
    return nullptr;

  return reinterpret_cast<C *>(data);
}

/*!
 * Initializes the container wit an object
 */
template <class C>
C *ODEntryComplexContainer::initCC(ObjectDataType type) noexcept {
  static_assert(sizeof(C) <= internal::calcSizeComplex(), "Invalid data type");
  static_assert(std::is_base_of<ODEntry, C>::value, "C is not derived from ODEntry");

  isInit = true;
  return new (data) C(type); // This created C on top of the already allocated memory data
}

constexpr ObjectClassType ODEntryComplexContainer::getOCTbyODT(ObjectType ot, ObjectDataType dt) noexcept {
  (void)ot;
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

    default: return ObjectClassType::UNSIGNED;
  }

  return ObjectClassType::UNSIGNED;
}


ODEntryComplexContainer::~ODEntryComplexContainer() {}

ODEntryComplexContainer::ODEntryComplexContainer(const ODEntryComplexContainer &c) {
  if (c.isInit)
    memcpy(data, c.data, internal::calcSizeComplex());
}

ODEntryComplexContainer::ODEntryComplexContainer(ODEntryComplexContainer &&c) {
  if (c.isInit)
    memcpy(data, c.data, internal::calcSizeComplex());
}

ODEntryComplexContainer &ODEntryComplexContainer::operator=(const ODEntryComplexContainer &c) {
  if (this != &c && c.isInit)
    memcpy(data, c.data, internal::calcSizeComplex());

  return *this;
}

ODEntryComplexContainer &ODEntryComplexContainer::operator=(ODEntryComplexContainer &&c) {
  if (this != &c && c.isInit)
    memcpy(data, c.data, internal::calcSizeComplex());

  return *this;
}

ODEntryComplexContainer::ODEntryComplexContainer(ODEntry *entry) {
  auto t = entry->getType();
  if (t == ObjectClassType::INTEGER || t == ObjectClassType::UNSIGNED || t == ObjectClassType::BOOL ||
      t == ObjectClassType::REAL)
    entry->clone(data);
}

ODEntryComplexContainer &ODEntryComplexContainer::operator=(ODEntry *entry) {
  auto t = entry->getType();
  if (t == ObjectClassType::INTEGER || t == ObjectClassType::UNSIGNED || t == ObjectClassType::BOOL ||
      t == ObjectClassType::REAL)
    entry->clone(data);

  return *this;
}

ODEntryComplexContainer::ODEntryComplexContainer(ObjectClassType type, ObjectDataType dt) { init(type, dt); }

ODEntryComplexContainer::ODEntryComplexContainer(ObjectDataType type, ObjectType ot)
    : ODEntryComplexContainer(getOCTbyODT(ot, type), type) {}

ODEntry *ODEntryComplexContainer::init(ObjectClassType type, ObjectDataType dt) {
  switch (type) {
    case ObjectClassType::INTEGER: return initCC<ODEntryInt>(dt);
    case ObjectClassType::UNSIGNED: return initCC<ODEntryUInt>(dt);
    case ObjectClassType::BOOL: return initCC<ODEntryBool>(dt);
    case ObjectClassType::REAL: return initCC<ODEntryReal>(dt);
    default: return initCC<ODEntryUInt>(dt);
  }
}

ODEntry *ODEntryComplexContainer::init(ObjectDataType type, ObjectType ot) { return init(getOCTbyODT(ot, type), type); }




class ODEntryComplex final : public ODEntry {
 public:
  std::vector<ODEntryComplexContainer> data;

  ODEntryComplex(const ODEntryComplex &) = default;
  ODEntryComplex(ODEntryComplex &&)      = delete;

  ODEntryComplex &operator=(const ODEntryComplex &) = delete;
  ODEntryComplex &operator=(ODEntryComplex &&) = delete;

  ODEntryComplex(ObjectDataType dt);
  ODEntryComplex() = delete;
  REAL_TYPE getNumericValue(uint8_t subIndex = 0) override;
  void setFromString(std::string str, uint8_t subIndex = 0) override;
  std::string toString(uint8_t subIndex = 0) override;
  int getArraySize() override;

  friend class ODEntryContainer;

  std::unique_ptr<ODEntry> clone() override;

 private:
  void clone(void *pos) override;
};
}
