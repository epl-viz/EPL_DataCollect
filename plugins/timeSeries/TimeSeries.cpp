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
 * \file TimeSeries.cpp
 * \brief Contains class TimeSeries
 */

#include "TimeSeries.hpp"
#include <iostream>

namespace EPL_DataCollect {
namespace plugins {

TimeSeries::TimeSeries(uint8_t nID, uint16_t index, uint8_t subIndex)
    : type(OBJECT), odIndex(index), odSubIndex(subIndex), nodeID(nID) {}
TimeSeries::TimeSeries(uint8_t nID, std::string cycleStorageID)
    : type(CYClE_STORAGE), nodeID(nID), csID(cycleStorageID) {}

TimeSeries::~TimeSeries() {}

/*!
 * \brief returns whether the specified OD entry is a custom Entry
 * \return bool
 */
bool TimeSeries::isCustomEntry() const noexcept { return type == CYClE_STORAGE; }


/*!
 * \brief Returns the type of the data to store
 * \return TimeSeriesDataType
 */
TimeSeries::TimeSeriesDataType TimeSeries::getType() const noexcept { return type; }


/*!
 * \brief returns the OD index
 * \return unsigned int
 */
uint16_t TimeSeries::getIndex() const noexcept { return odIndex; }

/*!
 * \brief returns the OD sub index
 * \return unsigned int
 */
uint8_t TimeSeries::getSubIndex() const noexcept { return odSubIndex; }


/*!
 * \brief Returns the ID of the Node
 * \return unsigned int
 */
uint8_t TimeSeries::getNodeID() const noexcept { return nodeID; }


/*!
 * \brief Returns the ID of the cycle storage
 * \return std::string
 */
std::string TimeSeries::getCSID() const noexcept { return csID; }


/*!
 * \brief Adds a new data point to the timeseries
 * \param  data The new data point
 */
void TimeSeries::addDataPoint(uint32_t cycleNum, ODEntry *data) noexcept {
  double val = 0;

  if (data)
    val = data->getNumericValue(odSubIndex);

  if (cycleNum >= tsData.size())
    tsData.resize(cycleNum + 1);

  tsData[cycleNum] = val;
}


/*!
 * \brief Adds a new data point to the timeseries
 * \param  data The new data point
 */
void TimeSeries::addDataPoint(uint32_t cycleNum, CycleStorageBase *data) noexcept {
  double val = 0;

  if (data)
    val = data->getNumericValue();

  if (cycleNum >= tsData.size())
    tsData.resize(cycleNum + 1);

  tsData[cycleNum] = val;
}
}
}
