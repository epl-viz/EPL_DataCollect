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
 * \file TimeSeries.hpp
 * \brief Contains class TimeSeries
 */


#pragma once


#include "defines.hpp"
#include "CycleStorageBase.hpp"
#include "ODEntry.hpp"
#include <string>
#include <vector>

namespace EPL_DataCollect {
namespace plugins {

/*!
  * class TimeSeries
  * \brief Time series for a specific OD (or CycleStorage) entry
  */
class TimeSeries {
 public:
  /*!
   * \brief The type of the TimeSeries
   */
  enum TimeSeriesDataType { OBJECT, CYClE_STORAGE };

 private:
  TimeSeriesDataType type;
  uint16_t           odIndex    = 0;
  uint8_t            odSubIndex = 0;
  uint8_t            nodeID     = 0;

  std::string csID = "";

 public:
  std::vector<double> tsData;

  TimeSeries() = delete;
  TimeSeries(uint8_t nID, uint16_t index, uint8_t subIndex = 0);
  TimeSeries(uint8_t nID, std::string cycleStorageID);
  virtual ~TimeSeries();

  TimeSeries(const TimeSeries &) = delete;
  TimeSeries(TimeSeries &&)      = delete;

  TimeSeries &operator=(const TimeSeries &) = delete;
  TimeSeries &operator=(TimeSeries &&) = delete;

  bool               isCustomEntry() const noexcept;
  TimeSeriesDataType getType() const noexcept;
  uint16_t           getIndex() const noexcept;
  uint8_t            getSubIndex() const noexcept;
  uint8_t            getNodeID() const noexcept;
  std::string        getCSID() const noexcept;

  void addDataPoint(uint32_t cycleNum, ODEntry *data) noexcept;
  void addDataPoint(uint32_t cycleNum, CycleStorageBase *data) noexcept;
};
}
}
