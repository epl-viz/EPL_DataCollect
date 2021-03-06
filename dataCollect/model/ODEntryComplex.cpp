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

#include "ODEntryComplex.hpp"
#include "EPLEnum2Str.hpp"
#include <iostream>

namespace EPL_DataCollect {


ODEntry::REAL_TYPE ODEntryComplex::getNumericValue(int si) {
  if (si < 0 || si >= static_cast<int>(data.size()))
    return 0;

  if (*data[static_cast<size_t>(si)] == nullptr)
    return 0;

  return data[static_cast<size_t>(si)]->getNumericValue();
}

void ODEntryComplex::setFromString(std::string str, uint8_t subIndex) {
  if (subIndex >= data.size())
    return;

  if (*data[subIndex] == nullptr)
    data[subIndex].init(ObjectDataType::INTEGER64);

  data[subIndex]->setFromString(str);
}

int         ODEntryComplex::getArraySize() { return static_cast<int>(data.size()); }
std::string ODEntryComplex::toString(int si) {
  if (si < 0 || si >= static_cast<int>(data.size()))
    return "<N/A>";

  if (*data[static_cast<size_t>(si)]) {
    return data[static_cast<size_t>(si)]->toString();
  } else {
    return "<N/A>";
  }
}
void ODEntryComplex::clone(void *pos) { new (pos) ODEntryComplex(*this); };
std::unique_ptr<ODEntry>         ODEntryComplex::clone() { return std::make_unique<ODEntryComplex>(*this); };

ODEntryComplex::ODEntryComplex(ObjectDataType dt) : ODEntry(ObjectClassType::COMPLEX, dt, false) {
  data.resize(0xFF + 1);
}
}
