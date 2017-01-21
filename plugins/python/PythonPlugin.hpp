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
 * \file PythonPlugin.hpp
 * \brief Contains class PythonPlugin
 * \todo IMPLEMENT
 */


#pragma once

#include "defines.hpp"
#include "CSPythonPluginStorage.hpp"
#include "PluginBase.hpp"

namespace EPL_DataCollect {
namespace plugins {

/*!
  * class PythonPlugin
  * \brief Base class for all Python based plugins
  */
class PythonPlugin : public PluginBase {
 public:
  // Constructors/Destructors
  //


  /*!
   * Empty Constructor
   */
  PythonPlugin();

  /*!
   * Empty Destructor
   */
  virtual ~PythonPlugin();

  // Static Public attributes
  //

  // Public attributes
  //


  // Public attribute accessor methods
  //


  // Public attribute accessor methods
  //


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

  CSPythonPluginStorage state;

 public:
  // Private attribute accessor methods
  //

 private:
 public:
  // Private attribute accessor methods
  //



  /*!
   * Get the value of state
   * \return the value of state
   */
  CSPythonPluginStorage *getState() { return &state; }

 private:
};
}
}
