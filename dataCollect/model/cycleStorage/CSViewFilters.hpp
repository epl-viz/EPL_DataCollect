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
 * \file CSViewFilters.hpp
 * \brief Contains class CSViewFilters
 */


#pragma once

#include "defines.hpp"
#include "CycleStorageBase.hpp"
#include <unordered_map>
#include <vector>

namespace EPL_DataCollect {

class CSViewFilters final : public CycleStorageBase {
 public:
  enum FilterType { INCLUDE, EXCLUDE };

  class Filter final {
   private:
    FilterType type;
    std::unordered_map<uint16_t, bool> setMap;
    std::string name;

   public:
    Filter()  = delete;
    ~Filter() = default;

    Filter(FilterType t, std::string n) : type(t), name(n) {}

    Filter(const Filter &) = default;
    Filter(Filter &&)      = default;

    Filter &operator=(const Filter &) = default;
    Filter &operator=(Filter &&) = default;

    inline std::string getName() const noexcept { return name; }
    inline FilterType  getType() const noexcept { return type; }

    bool includeIndex(uint16_t index) noexcept;

    void setIndex(uint16_t index) noexcept;
    void unSetIndex(uint16_t index) noexcept;
    bool isSet(uint16_t index) noexcept;
  };

 private:
  std::vector<Filter> filters;

 public:
  CSViewFilters() = default;
  virtual ~CSViewFilters();

  CSViewFilters(const CSViewFilters &) = default;
  CSViewFilters(CSViewFilters &&)      = default;

  CSViewFilters &operator=(const CSViewFilters &) = default;
  CSViewFilters &operator=(CSViewFilters &&) = default;

  double getNumericValue() override;
  bool   isNumericValue() override;

  std::unique_ptr<CycleStorageBase> clone() override;

  uint32_t newFilter(FilterType type, std::string name) noexcept;
  std::vector<Filter> getFilters() const noexcept;
  Filter *getFilter(uint32_t id) const noexcept;
};
}
