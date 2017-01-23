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
 * \file CycleBuilder.hpp
 * \brief Contains class CycleBuilder
 * \todo IMPLEMENT
 */

#pragma once

#include "Cycle.hpp"

namespace EPL_DataCollect {

/*!
  * class CycleBuilder
  * \brief Updates the cycle
  *
  * This class is responsible for deterministically generating / updating cycle with
  * the provided packet from the InputHandler and the invocation of the
  * PluginManager
  */
class CycleBuilder {
 public:
  // Constructors/Destructors
  //


  /*!
   * Empty Constructor
   */
  CycleBuilder();

  /*!
   * Empty Destructor
   */
  virtual ~CycleBuilder();

  // Static Public attributes
  //

  // Public attributes
  //


  // Public attribute accessor methods
  //


  // Public attribute accessor methods
  //



  /*!
   * \brief Starts the build loop
   * Returns false if a loop is already running
   * \return bool
   */
  bool startLoop() { return false; }


  /*!
   * \brief Stopps the build loop
   * Returns false if no loop is running
   * \return bool
   */
  bool stopLoop() { return false; }


  /*!
   * \brief Returns whether the build loop is running or not
   * \return bool
   */
  bool isRunning() { return false; }


  /*!
   * \brief (Re)constructs a specific cycle
   * This function uses a specific start cycle as a base to compute the targetCycle.
   * \return Cycle
   * \param  targetCycle The target cycle
   * \param  start The base cycle to iterate over
   */
  Cycle seekCycle(unsigned int targetCycle, Cycle start) {
    (void)targetCycle;
    return start;
  }


  /*!
   * \brief Returns the last cycle completely processed by the buildLoop
   * \return Cycle
   */
  Cycle getCurrentCycle() { return Cycle(); }

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

 public:
  // Private attribute accessor methods
  //

 private:
 public:
  // Private attribute accessor methods
  //

 private:
  /*!
   * \brief Main cycle builder loop. Runs in a seperate thread
   */
  void buildLoop() {}
};
}
