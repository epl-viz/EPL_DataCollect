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
 * \file EventBase.hpp
 * \brief Contains class EventBase
 */


#pragma once

#include "defines.hpp"
#include "EPLEnums.h"
#include <unordered_map>

namespace EPL_DataCollect {

class Cycle;

/*!
  * \brief Base class for all events
  */
class EventBase {
 public:
  typedef std::unordered_map<int, std::string>         INDEX_MAP;
  typedef std::unordered_map<std::string, std::string> MDATA_MAP;

 private:
  EvType      type        = EVT_UNKNOWN;
  std::string typeSTR     = "<NOT SET>";
  uint64_t    flags       = 0;
  std::string pluginID    = "<NOT SET>";
  std::string name        = "<NOT SET>";
  std::string description = "<NOT SET>";
  uint32_t    firstCycle  = 0;
  uint32_t    lastCycle   = 0;
  INDEX_MAP   affectedIndices;

 protected:
  EventBase(EvType      evType,
            std::string evPluginID,
            std::string evName,
            std::string evDesc,
            uint64_t    evFlags,
            Cycle *     cycle,
            INDEX_MAP   evIndices);

  EventBase(EvType      evType,
            std::string evTypeSTR,
            std::string evPluginID,
            std::string evName,
            std::string evDesc,
            uint64_t    evFlags,
            Cycle *     cycle,
            INDEX_MAP   evIndices);

 public:
  EventBase()          = delete;
  virtual ~EventBase() = default;

  EventBase(const EventBase &) = default;
  EventBase(EventBase &&)      = default;

  EventBase &operator=(const EventBase &) = default;
  EventBase &operator=(EventBase &&) = default;

  mockable EvType getType() const noexcept;
  mockable std::string getTypeAsString() const noexcept;
  mockable std::string getName() const noexcept;
  mockable std::string getDescription() const noexcept;
  mockable std::string getPluginID() const noexcept;
  mockable uint64_t getEventFlags() const noexcept;
  mockable INDEX_MAP getAffectedIndices() const noexcept;

  mockable void getCycleRange(uint32_t *first, uint32_t *last = nullptr) noexcept;
  mockable void updateRange(int first = -1, int last = -1) noexcept;

  virtual MDATA_MAP *getMetadata() noexcept;
};
}
