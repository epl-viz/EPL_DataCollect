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
 * \file ODEntry.cpp
 * \brief Contains class ODEntry
 */

#include "ODEntry.hpp"
#include "ODEntryContainer.hpp"
#include <iostream>

namespace EPL_DataCollect {

/*!
 * \brief The constructor
 * \param t   the class type of the Entry (the ID of the child class)
 * \param dt  the ethernetPOWERLINK data type
 * \param num Whether the type can be represented as a number
 */
ODEntry::ODEntry(ObjectClassType t, ObjectDataType dt, bool num) : type(t), dataType(dt), isNumerical(num) {}

ODEntry::~ODEntry() {}

/*!
 * \brief Returns the class type of the entry
 * \return ObjectClassType
 */
ObjectClassType ODEntry::getType() const noexcept { return type; }

/*!
 * \brief Returns the class type of the entry
 * \return ObjectClassType
 */
ObjectDataType ODEntry::getDataType() const noexcept { return dataType; }

/*!
 * \brief Returns whether the Entry can be represented as ONE numeric value
 * \return bool
 */
bool ODEntry::isNumericValue() const noexcept { return isNumerical; }



ODEntry::REAL_TYPE ODEntryInt::getNumericValue(uint8_t) { return static_cast<ODEntry::REAL_TYPE>(data); }
ODEntry::REAL_TYPE ODEntryUInt::getNumericValue(uint8_t) { return static_cast<ODEntry::REAL_TYPE>(data); }
ODEntry::REAL_TYPE ODEntryBool::getNumericValue(uint8_t) { return data ? 1 : 0; }
ODEntry::REAL_TYPE ODEntryReal::getNumericValue(uint8_t) { return data; }
ODEntry::REAL_TYPE ODEntryString::getNumericValue(uint8_t) { return 0; }
ODEntry::REAL_TYPE ODEntryArrayInt::getNumericValue(uint8_t si) { return static_cast<ODEntry::REAL_TYPE>(data[si]); }
ODEntry::REAL_TYPE ODEntryArrayUInt::getNumericValue(uint8_t si) { return static_cast<ODEntry::REAL_TYPE>(data[si]); }
ODEntry::REAL_TYPE ODEntryArrayBool::getNumericValue(uint8_t si) { return data[si] ? 1 : 0; }
ODEntry::REAL_TYPE ODEntryArrayReal::getNumericValue(uint8_t si) { return data[si]; }


inline bool strToBool(std::string str) {
  return str == "true" || str == "True" || str == "TRUE" || str == "1" || str == "ON" || str == "on";
}

void ODEntryInt::setFromString(std::string str, uint8_t) { data = std::stol(str, nullptr, 0); }
void ODEntryUInt::setFromString(std::string str, uint8_t) { data = std::stoul(str, nullptr, 0); }
void ODEntryBool::setFromString(std::string str, uint8_t) { data = strToBool(str); }
void ODEntryReal::setFromString(std::string str, uint8_t) { data = std::stod(str); }
void ODEntryString::setFromString(std::string str, uint8_t) { data = str; }
void ODEntryArrayInt::setFromString(std::string str, uint8_t subIndex) { data[subIndex] = std::stol(str); }
void ODEntryArrayUInt::setFromString(std::string str, uint8_t subIndex) { data[subIndex] = std::stoul(str); }
void ODEntryArrayBool::setFromString(std::string str, uint8_t subIndex) { data[subIndex] = strToBool(str) ? 1 : 0; }
void ODEntryArrayReal::setFromString(std::string str, uint8_t subIndex) { data[subIndex] = std::stod(str); }


std::string ODEntryInt::toString(uint8_t) { return std::to_string(data); }
std::string ODEntryUInt::toString(uint8_t) { return std::to_string(data); }
std::string ODEntryBool::toString(uint8_t) { return data ? "true" : "false"; }
std::string ODEntryReal::toString(uint8_t) { return std::to_string(data); }
std::string ODEntryString::toString(uint8_t) { return data; }
std::string ODEntryArrayInt::toString(uint8_t si) { return std::to_string(data[si]); }
std::string ODEntryArrayUInt::toString(uint8_t si) { return std::to_string(data[si]); }
std::string ODEntryArrayBool::toString(uint8_t si) { return data[si] ? "true" : "false"; }
std::string ODEntryArrayReal::toString(uint8_t si) { return std::to_string(data[si]); }


void ODEntryInt::clone(void *pos) { new (pos) ODEntryInt(*this); }
void ODEntryUInt::clone(void *pos) { new (pos) ODEntryUInt(*this); }
void ODEntryBool::clone(void *pos) { new (pos) ODEntryBool(*this); }
void ODEntryReal::clone(void *pos) { new (pos) ODEntryReal(*this); }
void ODEntryString::clone(void *pos) { new (pos) ODEntryString(*this); }
void ODEntryArrayInt::clone(void *pos) { new (pos) ODEntryArrayInt(*this); }
void ODEntryArrayUInt::clone(void *pos) { new (pos) ODEntryArrayUInt(*this); }
void ODEntryArrayBool::clone(void *pos) { new (pos) ODEntryArrayBool(*this); }
void ODEntryArrayReal::clone(void *pos) { new (pos) ODEntryArrayReal(*this); }

std::unique_ptr<ODEntry> ODEntryInt::clone() { return std::make_unique<ODEntryInt>(*this); }
std::unique_ptr<ODEntry> ODEntryUInt::clone() { return std::make_unique<ODEntryUInt>(*this); }
std::unique_ptr<ODEntry> ODEntryBool::clone() { return std::make_unique<ODEntryBool>(*this); }
std::unique_ptr<ODEntry> ODEntryReal::clone() { return std::make_unique<ODEntryReal>(*this); }
std::unique_ptr<ODEntry> ODEntryString::clone() { return std::make_unique<ODEntryString>(*this); }
std::unique_ptr<ODEntry> ODEntryArrayInt::clone() { return std::make_unique<ODEntryArrayInt>(*this); }
std::unique_ptr<ODEntry> ODEntryArrayUInt::clone() { return std::make_unique<ODEntryArrayUInt>(*this); }
std::unique_ptr<ODEntry> ODEntryArrayBool::clone() { return std::make_unique<ODEntryArrayBool>(*this); }
std::unique_ptr<ODEntry> ODEntryArrayReal::clone() { return std::make_unique<ODEntryArrayReal>(*this); }

int ODEntryInt::getArraySize() { return -1; }
int ODEntryUInt::getArraySize() { return -1; }
int ODEntryBool::getArraySize() { return -1; }
int ODEntryReal::getArraySize() { return -1; }
int ODEntryString::getArraySize() { return -1; }
int ODEntryArrayInt::getArraySize() { return static_cast<int>(data.size()); }
int ODEntryArrayUInt::getArraySize() { return static_cast<int>(data.size()); }
int ODEntryArrayBool::getArraySize() { return static_cast<int>(data.size()); }
int ODEntryArrayReal::getArraySize() { return static_cast<int>(data.size()); }
}
