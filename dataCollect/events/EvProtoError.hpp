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
 * \file EvProtoError.hpp
 * \brief Contains class EvProtoError
 */


#pragma once

#include "defines.hpp"
#include "EventBase.hpp"

namespace EPL_DataCollect {

/*!
  * \brief This event should only be triggered when a ethernetPOWERLINK protocol error occurred
  *
  * This class does not do much. Most of the logic is handled in EventBase
  */
class EvProtoError : public EventBase {
 public:
  EvProtoError() = delete;
  virtual ~EvProtoError();

  EvProtoError(std::string          evPluginID,
               std::string          evName,
               std::string          evDesc,
               uint64_t             evFlags,
               Cycle *              cycle,
               EventBase::INDEX_MAP evIndices);

  EvProtoError(const EvProtoError &) = default;
  EvProtoError(EvProtoError &&)      = default;

  EvProtoError &operator=(const EvProtoError &) = default;
  EvProtoError &operator=(EvProtoError &&) = default;
};
}
