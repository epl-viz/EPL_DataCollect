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
 * \file EvPluginText.hpp
 * \brief Contains class EvPluginText
 */


#pragma once

#include "EventBase.hpp"

namespace EPL_DataCollect {

/*!
  * \brief Event that contains a text message from a Plugin
  *
  * \note This event type should only be used when a plugin would write to the
  *   standard output. Always consider the other event types first (EvProtoError,
  *   EvError, EvWarning EvInfo, EvDebug)
  *
  * This class does not do much. Most of the logic is handled in EventBase
  */
class EvPluginText : public EventBase {
 public:
  EvPluginText() = delete;
  virtual ~EvPluginText();

  EvPluginText(std::string          evPluginID,
               std::string          evName,
               std::string          evDesc,
               uint64_t             evFlags,
               Cycle *              cycle,
               EventBase::INDEX_MAP evIndices);

  EvPluginText(const EvPluginText &) = default;
  EvPluginText(EvPluginText &&)      = default;

  EvPluginText &operator=(const EvPluginText &) = default;
  EvPluginText &operator=(EvPluginText &&) = default;
};
}
