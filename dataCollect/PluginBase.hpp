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
 * \file PluginBase.hpp
 * \brief Contains class PluginBase
 * \todo IMPLEMENT
 */

#pragma once

#include "defines.hpp"

#include "Cycle.hpp"
#include "EventBase.hpp"

namespace EPL_DataCollect {

class CaptureInstance;

/*!
  * class PluginBase
  * \brief Base class for all plugins
  *
  * Also provides an interface for registering cycle storage and generating events.
  */
class PluginBase {
 public:
  // Constructors/Destructors
  //


  /*!
   * Empty Constructor
   */
  PluginBase();

  /*!
   * Empty Destructor
   */
  virtual ~PluginBase();

  PluginBase(const PluginBase &) = default;
  PluginBase(PluginBase &&)      = default;

  PluginBase &operator=(const PluginBase &) = default;
  PluginBase &operator=(PluginBase &&) = default;

  // Static Public attributes
  //

  // Public attributes
  //


  // Public attribute accessor methods
  //


  // Public attribute accessor methods
  //



  /*!
   * \brief Execute the plugin
   * \warning This function must be FAST
   * \param  cycle The cycle to process
   */
  virtual void run(Cycle *cycle) { (void)cycle; }


  /*!
   * \brief initializes the plugin
   * \param ci A pointer to the capture instance
   */
  virtual bool initialize(CaptureInstance *ci) {
    (void)ci;
    return true;
  }


  /*!
   * \brief initializes the plugin
   */
  virtual bool reset(CaptureInstance *ci) {
    (void)ci;
    return true;
  }


  /*!
   * \brief Returns a semicolon seperated list of dependencies
   * \return std::string
   */
  virtual std::string getDependencies() { return ""; }


  /*!
   * \brief Returns an ID std::string
   * \return std::string
   */
  virtual std::string getID() { return ""; }

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
  /*!
   * \brief Adds an event to the event log
   * \return int
   * \param  event The event to add
   */
  int addEvent(EventBase event) {
    (void)event;
    return 0;
  }


  /*!
   * \brief Adds a new cycle storage entry
   * Returns false when the index is already occupied
   * TEMPLATED
   *
   * Wrapper for the CaptureInstance function
   * \return bool
   * \param  index The index to register
   */
  bool registerCycleStorage(std::string index) {
    (void)index;
    return false;
  }

 private:
  // Static Private attributes
  //

  // Private attributes
  //

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
