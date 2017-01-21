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
 * \file CaptureInstance.hpp
 * \brief Contains class CaptureInstance
 * \todo IMPLEMENT
 */

#pragma once

#include "defines.hpp"

#include <vector>
#include "CycleContainer.hpp"
#include "EventLog.hpp"
#include "PluginManager.hpp"

namespace EPL_DataCollect {

/*!
  * class CaptureInstance
  * \brief Root class for the EPL-Viz backend
  *
  * This class handles all relevant data necessary to process the ePL Packets
  */
class CaptureInstance {
 public:
  // Constructors/Destructors
  //


  /*!
   * Empty Constructor
   */
  CaptureInstance();

  /*!
   * Empty Destructor
   */
  virtual ~CaptureInstance();

  // Static Public attributes
  //

  // Public attributes
  //


  // Public attribute accessor methods
  //


  // Public attribute accessor methods
  //



  /*!
   * \brief Loads the specified XDD file
   * \param  path the xdd file to load
   */
  void loadXDD( std::string path ) { (void)path; }


  /*!
   * \brief starts recording on the specified ethernet device
   *
   * A list of available devices can be obtained with getDevices()
   * \param  interface The network device to use for the live capture
   */
  void startRecording( std::string interface ) { (void)interface; }


  /*!
   * \brief ends the live capture
   */
  void stopRecording() {}


  /*!
   * \brief loads a previously captured PCAP
   * \param  file The file to load
   */
  void loadPCAP( std::string file ) { (void)file; }


  /*!
   * \brief Adds a new cycle storage entry
   * Returns false when the index is already occupied
   * TEMPLATED
   * \return bool
   * \param  index The new index to register
   */
  bool registerCycleStorage( std::string index ) {
    (void)index;
    return false;
  }


  /*!
   * \brief Returns a list of available network devices
   * \return std::vector<std::string>
   */
  std::vector<std::string> getDevices() { return std::vector<std::string>(); }


  /*!
   * \brief Returns a pointer to the EventLog
   * \note The pointer is valid for the entire lifetime of the CaptureInstance
   * instance
   * \return EventLog
   */
  EventLog *getEventLog() { return &eventLog; }


  /*!
   * \brief Returns a pointer to the PluginManager
   * \note The pointer is valid for the entire lifetime of the CaptureInstance
   * instance
   * \return PluginManager
   */
  PluginManager *getPluginManager() { return &pluginManager; }


  /*!
   * \brief Returns a pointer to the CycleContainer
   * \note The pointer is valid for the entire lifetime of the CaptureInstance
   * instance
   * \return CycleContainer
   */
  CycleContainer *getCycleContainer() { return &cycleContainer; }

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

  // The PluginManager for this capture instance
  PluginManager pluginManager;
  // The EventLog of this capture instance
  EventLog eventLog;
  // The CycleContainer of this capture instance
  CycleContainer cycleContainer;

 public:
  // Private attribute accessor methods
  //

 private:
 public:
 private:
};
}
