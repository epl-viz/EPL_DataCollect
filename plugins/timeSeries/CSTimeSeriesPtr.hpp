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
 * \file CSTimeSeriesPtr.hpp
 * \brief Contains class CSTimeSeriesPtr
 */


#pragma once

#include "defines.hpp"
#include "CycleStorageBase.hpp"
#include "TimeSeries.hpp"
#include <memory>
#include <plf_colony.h>

namespace EPL_DataCollect {
namespace plugins {

/*!
  * class CSTimeSeriesPtr
  * \brief Contains all pointers to TimeSeries instances
  *
  * \note This class is part of the TimeSeries plugin
  */
class CSTimeSeriesPtr final : public CycleStorageBase {
 public:
  typedef plf::colony<std::shared_ptr<TimeSeries>> CONTAINER;

 private:
  CONTAINER timeSeries;

 public:
  CSTimeSeriesPtr() = default;
  virtual ~CSTimeSeriesPtr();

  CSTimeSeriesPtr(const CSTimeSeriesPtr &) = default;
  CSTimeSeriesPtr(CSTimeSeriesPtr &&)      = default;

  CSTimeSeriesPtr &operator=(const CSTimeSeriesPtr &) = default;
  CSTimeSeriesPtr &operator=(CSTimeSeriesPtr &&) = default;

  CONTAINER *getTsPTRs() noexcept;
  void addTS(std::shared_ptr<TimeSeries> newTS) noexcept;

  double getNumericValue() override { return 0; }
  bool   isNumericValue() override { return false; }

  std::unique_ptr<CycleStorageBase> clone() override { return std::make_unique<CSTimeSeriesPtr>(*this); }
};
}
}
