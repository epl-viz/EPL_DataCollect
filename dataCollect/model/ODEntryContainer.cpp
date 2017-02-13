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
#include <string.h>

namespace EPL_DataCollect {

ODEntryContainer::~ODEntryContainer() {
  // Manually call the destructor
  reinterpret_cast<ODEntry *>(data)->~ODEntry();
}

ODEntryContainer::ODEntryContainer(const ODEntryContainer &c) {
  memset(data, 0, internal::calcSize());
  reinterpret_cast<ODEntry *>(const_cast<ODEntryContainer &>(c).data)->clone(data);
}
ODEntryContainer::ODEntryContainer(ODEntryContainer &&c) {
  memset(data, 0, internal::calcSize());
  reinterpret_cast<ODEntry *>(c.data)->clone(data);
}

ODEntryContainer &ODEntryContainer::operator=(const ODEntryContainer &c) {
  memset(data, 0, internal::calcSize());
  reinterpret_cast<ODEntry *>(const_cast<ODEntryContainer &>(c).data)->clone(data);
  return *this;
}

ODEntryContainer &ODEntryContainer::operator=(ODEntryContainer &&c) {
  memset(data, 0, internal::calcSize());
  reinterpret_cast<ODEntry *>(c.data)->clone(data);
  return *this;
}

ODEntryContainer::ODEntryContainer(ObjectClassType type, ObjectDataType dt) {
  memset(data, 0, internal::calcSize());

  switch (type) {
    case OCT_INTEGER: init<ODEntryInt>(dt); return;
    case OCT_UNSIGNED: init<ODEntryUInt>(dt); return;
    case OCT_BOOL: init<ODEntryBool>(dt); return;
    case OCT_REAL: init<ODEntryReal>(dt); return;
    case OCT_STRING: init<ODEntryString>(dt); return;
    case OCT_ARRAY_INTEGER: init<ODEntryArrayInt>(dt); return;
    case OCT_ARRAY_UNSIGNED: init<ODEntryArrayUInt>(dt); return;
    case OCT_ARRAY_BOOL: init<ODEntryArrayBool>(dt); return;
    case OCT_ARRAY_REAL: init<ODEntryArrayReal>(dt); return;
    case OCT_COMPLEX: init<ODEntryComplex>(dt); return;
  }
}

ODEntryContainer::ODEntryContainer(ObjectDataType type, ObjectType ot)
    : ODEntryContainer(getOCTbyODT(ot, type), type) {}

ODEntry *ODEntryContainer::operator*() noexcept { return getData<ODEntry>(); }
ODEntry *ODEntryContainer::operator->() noexcept { return getData<ODEntry>(); }
}
