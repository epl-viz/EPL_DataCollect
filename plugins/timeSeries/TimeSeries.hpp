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
 * \todo IMPLEMENT
 */


#pragma once


#include "defines.hpp"
#include "CycleStorageBase.hpp"
#include "ODEntry.hpp"
#include <string>

namespace EPL_DataCollect {
namespace plugins {

/*!
 * \brief The type of the TimeSeries
 * \todo IMPLEMENT
 */
enum TimeSeriesDataType { TS_DT_INT };

/*!
  * class TimeSeries
  * \brief Time series for a specific OD (or CycleStorage) entry
  */
class TimeSeries {
 public:
  // Constructors/Destructors
  //


  /*!
   * Empty Constructor
   */
  TimeSeries();

  /*!
   * Empty Destructor
   */
  virtual ~TimeSeries();

  // Static Public attributes
  //

  // Public attributes
  //


  // Public attribute accessor methods
  //


  // Public attribute accessor methods
  //



  /*!
   * \brief returns whether the specified OD entry is a custom Entry
   * \return bool
   */
  bool isCustomEntry() { return false; }


  /*!
   * \brief Returns the type of the data to store
   * \return TimeSeriesDataType
   */
  TimeSeriesDataType getType() { return type; }


  /*!
   * \brief returns the OD / custom entry index for this time series
   * \return unsigned int
   */
  unsigned int getIndex() { return 0; }


  /*!
   * \brief Returns the ID of the Node
   * \return unsigned int
   */
  unsigned int getNodeID() { return 0; }


  /*!
   * \brief Returns the ID of the cycle storage
   * \return std::string
   */
  std::string getCSID() { return ""; }


  /*!
   * \brief Adds a new data point to the timeseries
   * \param  data The new data point
   */
  void addDataPoint(ODEntry *data) { (void)data; }


  /*!
   * \brief Adds a new data point to the timeseries
   * \param  data The new data point
   */
  void addDataPoint(CycleStorageBase *data) { (void)data; }

 protected:
  // Static Protected attributes
  //

  // Protected attributes
  //

 public:
  // Protected attribute accessor methods
  //

 protected:
 public:
  // Protected attribute accessor methods
  //

 protected:
 private:
  // Static Private attributes
  //

  // Private attributes
  //

  // The type of the data
  TimeSeriesDataType type;

 public:
  // Private attribute accessor methods
  //

 private:
 public:
  // Private attribute accessor methods
  //


 private:
};
}
}
