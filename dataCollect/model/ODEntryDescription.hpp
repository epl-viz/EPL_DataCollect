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
 * \file ODEntryDescription.hpp
 * \brief Contains struct ODEntryDescription
 */


#pragma once

#include "defines.hpp"
#include "ODEntryContainer.hpp"
#include "EPLEnums.h"

namespace EPL_DataCollect {

/*!
  * \brief Description of a specific OD entry
  */
struct ODEntryDescription final {
  uint16_t         index    = 0;                   //!< \brief The 16bit Index of the OD Entry
  ObjectType       type     = ObjectType::NULL_OT; //!< \brief The Object type (see EPSG DS 301 v1.3.0 Section 6.2.1)
  std::string      name     = "<UNDEFINED>";
  ObjectDataType   dataType = ObjectDataType::BOOLEAN;
  ObjectCategory   category = ObjectCategory::NOT_RELEVANT;
  ObjectAccess     access   = ObjectAccess::CONST;
  ODEntryContainer valueMin;
  ODEntryContainer valueMax;
  ODEntryContainer defaultValue;

  ODEntryDescription() = delete;
  ODEntryDescription(ObjectType t, ObjectDataType dt)
      : type(t), dataType(dt), valueMin(dataType), valueMax(dataType), defaultValue(dataType) {}
  ~ODEntryDescription() = default;

  ODEntryDescription(const ODEntryDescription &) = default;
  ODEntryDescription(ODEntryDescription &&)      = default;

  ODEntryDescription &operator=(const ODEntryDescription &) = default;
  ODEntryDescription &operator=(ODEntryDescription &&) = default;
};
}
