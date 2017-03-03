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
 * \file ODEntryContainer.cpp
 * \brief Contains class ODEntryContainer
 */

#include "ODEntryContainer.hpp"
#include <iostream>
#include <string.h>

namespace EPL_DataCollect {

ODEntryContainer::~ODEntryContainer() {
  // Manually call the destructor
  if (!movedFrom)
    reinterpret_cast<ODEntry *>(data)->~ODEntry();
}

ODEntryContainer::ODEntryContainer(const ODEntryContainer &c) {
  reinterpret_cast<ODEntry *>(const_cast<ODEntryContainer &>(c).data)->clone(data);
}

ODEntryContainer::ODEntryContainer(ODEntryContainer &&c) {
  reinterpret_cast<ODEntry *>(c.data)->clone(data);
  c.movedFrom = true;
}

ODEntryContainer &ODEntryContainer::operator=(const ODEntryContainer &c) {
  if (this != &c) {
    reinterpret_cast<ODEntry *>(data)->~ODEntry();
    reinterpret_cast<ODEntry *>(const_cast<ODEntryContainer &>(c).data)->clone(data);
  }
  return *this;
}

ODEntryContainer &ODEntryContainer::operator=(ODEntryContainer &&c) {
  if (this != &c) {
    reinterpret_cast<ODEntry *>(data)->~ODEntry();
    reinterpret_cast<ODEntry *>(c.data)->clone(data);
    c.movedFrom = true;
  }
  return *this;
}

ODEntryContainer::ODEntryContainer(ODEntry *entry) { entry->clone(data); }

ODEntryContainer::ODEntryContainer(ObjectClassType type, ObjectDataType dt) {
  switch (type) {
    case ObjectClassType::INTEGER: init<ODEntryInt>(dt); return;
    case ObjectClassType::UNSIGNED: init<ODEntryUInt>(dt); return;
    case ObjectClassType::BOOL: init<ODEntryBool>(dt); return;
    case ObjectClassType::REAL: init<ODEntryReal>(dt); return;
    case ObjectClassType::STRING: init<ODEntryString>(dt); return;
    case ObjectClassType::ARRAY_INTEGER: init<ODEntryArrayInt>(dt); return;
    case ObjectClassType::ARRAY_UNSIGNED: init<ODEntryArrayUInt>(dt); return;
    case ObjectClassType::ARRAY_BOOL: init<ODEntryArrayBool>(dt); return;
    case ObjectClassType::ARRAY_REAL: init<ODEntryArrayReal>(dt); return;
    case ObjectClassType::COMPLEX: init<ODEntryComplex>(dt); return;
  }
}

ODEntryContainer::ODEntryContainer(ObjectDataType type, ObjectType ot)
    : ODEntryContainer(getOCTbyODT(ot, type), type) {}
}
