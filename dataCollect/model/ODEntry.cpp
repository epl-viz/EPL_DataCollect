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



ODEntry::REAL_TYPE ODEntryInt::getNumericValue() { return static_cast<ODEntry::REAL_TYPE>(data); }
ODEntry::REAL_TYPE ODEntryUInt::getNumericValue() { return static_cast<ODEntry::REAL_TYPE>(data); }
ODEntry::REAL_TYPE ODEntryBool::getNumericValue() { return data ? 1 : 0; }
ODEntry::REAL_TYPE ODEntryReal::getNumericValue() { return data; }
ODEntry::REAL_TYPE ODEntryString::getNumericValue() { return 0; }
ODEntry::REAL_TYPE ODEntryArrayInt::getNumericValue() { return 0; }
ODEntry::REAL_TYPE ODEntryArrayUInt::getNumericValue() { return 0; }
ODEntry::REAL_TYPE ODEntryArrayBool::getNumericValue() { return 0; }
ODEntry::REAL_TYPE ODEntryArrayReal::getNumericValue() { return 0; }
ODEntry::REAL_TYPE ODEntryComplex::getNumericValue() { return 0; }

void ODEntryInt::clone(void *pos) { new (pos) ODEntryInt(*this); };
void ODEntryUInt::clone(void *pos) { new (pos) ODEntryUInt(*this); };
void ODEntryBool::clone(void *pos) { new (pos) ODEntryBool(*this); };
void ODEntryReal::clone(void *pos) { new (pos) ODEntryReal(*this); };
void ODEntryString::clone(void *pos) { new (pos) ODEntryString(*this); };
void ODEntryArrayInt::clone(void *pos) { new (pos) ODEntryArrayInt(*this); };
void ODEntryArrayUInt::clone(void *pos) { new (pos) ODEntryArrayUInt(*this); };
void ODEntryArrayBool::clone(void *pos) { new (pos) ODEntryArrayBool(*this); };
void ODEntryArrayReal::clone(void *pos) { new (pos) ODEntryArrayReal(*this); };
void ODEntryComplex::clone(void *pos) { new (pos) ODEntryComplex(*this); };
}
