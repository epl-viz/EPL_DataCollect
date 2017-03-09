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
 * \file ODEntry.hpp
 * \brief Contains class ODEntry as well as all derived classes
 */

#pragma once

#include "defines.hpp"
#include "EPLEnums.h"
#include <memory>
#include <vector>

namespace EPL_DataCollect {

class ODEntryContainer;

/*!
  * class ODEntry
  * \brief Represents an entry in the OD
  */
class ODEntry {
 public:
  typedef double REAL_TYPE;

 private:
  ObjectClassType type;
  ObjectDataType  dataType;
  bool            isNumerical;

 public:
  ODEntry() = delete;
  ODEntry(ObjectClassType t, ObjectDataType dt, bool num);

  virtual ~ODEntry();

  ODEntry(const ODEntry &) = default;
  ODEntry(ODEntry &&)      = default;

  ODEntry &operator=(const ODEntry &) = default;
  ODEntry &operator=(ODEntry &&) = default;

  mockable ObjectClassType getType() const noexcept;
  mockable ObjectDataType getDataType() const noexcept;
  mockable bool           isNumericValue() const noexcept;

  /*!
   * \brief Returns a numeric Representation of the Entry
   * \return double
   */
  virtual REAL_TYPE getNumericValue(int subIndex = -1) = 0;
  virtual void setFromString(std::string str, uint8_t subIndex = 0) = 0;
  virtual std::string toString(int subIndex = -1) = 0;
  virtual void clone(void *pos)                   = 0;
  virtual std::unique_ptr<ODEntry> clone()        = 0;
  virtual int                      getArraySize() = 0;
};




class ODEntryInt final : public ODEntry {
 public:
  int64_t data = 0;

  ODEntryInt(const ODEntryInt &) = default;
  ODEntryInt(ODEntryInt &&)      = delete;

  ODEntryInt &operator=(const ODEntryInt &) = delete;
  ODEntryInt &operator=(ODEntryInt &&) = delete;

  ODEntryInt(ObjectDataType dt) : ODEntry(ObjectClassType::INTEGER, dt, true) {}
  ODEntryInt() = delete;
  REAL_TYPE getNumericValue(int subIndex = -1) override;
  void setFromString(std::string str, uint8_t subIndex = 0) override;
  std::string toString(int subIndex = -1) override;
  int getArraySize() override;

  friend class ODEntryContainer;

  std::unique_ptr<ODEntry> clone() override;

 private:
  void clone(void *pos) override;
};

class ODEntryUInt final : public ODEntry {
 public:
  uint64_t data = 0;

  ODEntryUInt(const ODEntryUInt &) = default;
  ODEntryUInt(ODEntryUInt &&)      = delete;

  ODEntryUInt &operator=(const ODEntryUInt &) = delete;
  ODEntryUInt &operator=(ODEntryUInt &&) = delete;

  ODEntryUInt(ObjectDataType dt) : ODEntry(ObjectClassType::UNSIGNED, dt, true) {}
  ODEntryUInt() = delete;
  REAL_TYPE getNumericValue(int subIndex = -1) override;
  void setFromString(std::string str, uint8_t subIndex = 0) override;
  std::string toString(int subIndex = -1) override;
  int getArraySize() override;

  friend class ODEntryContainer;

  std::unique_ptr<ODEntry> clone() override;

 private:
  void clone(void *pos) override;
};

class ODEntryBool final : public ODEntry {
 public:
  bool data = false;

  ODEntryBool(const ODEntryBool &) = default;
  ODEntryBool(ODEntryBool &&)      = delete;

  ODEntryBool &operator=(const ODEntryBool &) = delete;
  ODEntryBool &operator=(ODEntryBool &&) = delete;

  ODEntryBool(ObjectDataType dt) : ODEntry(ObjectClassType::BOOL, dt, true) {}
  ODEntryBool() = delete;
  REAL_TYPE getNumericValue(int subIndex = -1) override;
  void setFromString(std::string str, uint8_t subIndex = 0) override;
  std::string toString(int subIndex = -1) override;
  int getArraySize() override;

  friend class ODEntryContainer;

  std::unique_ptr<ODEntry> clone() override;

 private:
  void clone(void *pos) override;
};

class ODEntryReal final : public ODEntry {
 public:
  REAL_TYPE data = 0;

  ODEntryReal(const ODEntryReal &) = default;
  ODEntryReal(ODEntryReal &&)      = delete;

  ODEntryReal &operator=(const ODEntryReal &) = delete;
  ODEntryReal &operator=(ODEntryReal &&) = delete;

  ODEntryReal(ObjectDataType dt) : ODEntry(ObjectClassType::REAL, dt, true) {}
  ODEntryReal() = delete;
  REAL_TYPE getNumericValue(int subIndex = -1) override;
  void setFromString(std::string str, uint8_t subIndex = 0) override;
  std::string toString(int subIndex = -1) override;
  int getArraySize() override;

  friend class ODEntryContainer;

  std::unique_ptr<ODEntry> clone() override;

 private:
  void clone(void *pos) override;
};

class ODEntryString final : public ODEntry {
 public:
  std::string data = "";

  ODEntryString(const ODEntryString &) = default;
  ODEntryString(ODEntryString &&)      = delete;

  ODEntryString &operator=(const ODEntryString &) = delete;
  ODEntryString &operator=(ODEntryString &&) = delete;

  ODEntryString(ObjectDataType dt) : ODEntry(ObjectClassType::STRING, dt, false) {}
  ODEntryString() = delete;
  REAL_TYPE getNumericValue(int subIndex = -1) override;
  void setFromString(std::string str, uint8_t subIndex = 0) override;
  std::string toString(int subIndex = -1) override;
  int getArraySize() override;

  friend class ODEntryContainer;

  std::unique_ptr<ODEntry> clone() override;

 private:
  void clone(void *pos) override;
};

class ODEntryArrayInt final : public ODEntry {
 public:
  std::vector<int64_t> data;

  ODEntryArrayInt(const ODEntryArrayInt &) = default;
  ODEntryArrayInt(ODEntryArrayInt &&)      = delete;

  ODEntryArrayInt &operator=(const ODEntryArrayInt &) = delete;
  ODEntryArrayInt &operator=(ODEntryArrayInt &&) = delete;

  ODEntryArrayInt(ObjectDataType dt) : ODEntry(ObjectClassType::ARRAY_INTEGER, dt, false) { data.resize(0xFF + 1); }
  ODEntryArrayInt() = delete;
  REAL_TYPE getNumericValue(int subIndex = -1) override;
  void setFromString(std::string str, uint8_t subIndex = 0) override;
  std::string toString(int subIndex = -1) override;
  int getArraySize() override;

  friend class ODEntryContainer;

  std::unique_ptr<ODEntry> clone() override;

 private:
  void clone(void *pos) override;
};

class ODEntryArrayUInt final : public ODEntry {
 public:
  std::vector<uint64_t> data;

  ODEntryArrayUInt(const ODEntryArrayUInt &) = default;
  ODEntryArrayUInt(ODEntryArrayUInt &&)      = delete;

  ODEntryArrayUInt &operator=(const ODEntryArrayUInt &) = delete;
  ODEntryArrayUInt &operator=(ODEntryArrayUInt &&) = delete;

  ODEntryArrayUInt(ObjectDataType dt) : ODEntry(ObjectClassType::ARRAY_UNSIGNED, dt, false) { data.resize(0xFF + 1); }
  ODEntryArrayUInt() = delete;
  REAL_TYPE getNumericValue(int subIndex = -1) override;
  void setFromString(std::string str, uint8_t subIndex = 0) override;
  std::string toString(int subIndex = -1) override;
  int getArraySize() override;

  friend class ODEntryContainer;

  std::unique_ptr<ODEntry> clone() override;

 private:
  void clone(void *pos) override;
};

class ODEntryArrayBool final : public ODEntry {
 public:
  std::vector<uint8_t> data;

  ODEntryArrayBool(const ODEntryArrayBool &) = default;
  ODEntryArrayBool(ODEntryArrayBool &&)      = delete;

  ODEntryArrayBool &operator=(const ODEntryArrayBool &) = delete;
  ODEntryArrayBool &operator=(ODEntryArrayBool &&) = delete;

  ODEntryArrayBool(ObjectDataType dt) : ODEntry(ObjectClassType::ARRAY_BOOL, dt, false) { data.resize(0xFF + 1); }
  ODEntryArrayBool() = delete;
  REAL_TYPE getNumericValue(int subIndex = -1) override;
  void setFromString(std::string str, uint8_t subIndex = 0) override;
  std::string toString(int subIndex = -1) override;
  int getArraySize() override;

  friend class ODEntryContainer;

  std::unique_ptr<ODEntry> clone() override;

 private:
  void clone(void *pos) override;
};

class ODEntryArrayReal final : public ODEntry {
 public:
  std::vector<REAL_TYPE> data;

  ODEntryArrayReal(const ODEntryArrayReal &) = default;
  ODEntryArrayReal(ODEntryArrayReal &&)      = delete;

  ODEntryArrayReal &operator=(const ODEntryArrayReal &) = delete;
  ODEntryArrayReal &operator=(ODEntryArrayReal &&) = delete;

  ODEntryArrayReal(ObjectDataType dt) : ODEntry(ObjectClassType::ARRAY_REAL, dt, false) { data.resize(0xFF + 1); }
  ODEntryArrayReal() = delete;
  REAL_TYPE getNumericValue(int subIndex = -1) override;
  void setFromString(std::string str, uint8_t subIndex = 0) override;
  std::string toString(int subIndex = -1) override;
  int getArraySize() override;

  friend class ODEntryContainer;

  std::unique_ptr<ODEntry> clone() override;

 private:
  void clone(void *pos) override;
};
}
