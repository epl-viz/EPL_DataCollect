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
 * \file CSPythonPluginStorage.hpp
 * \brief Contains class CSPythonPluginStorage
 * \todo IMPLEMENT
 */


#pragma once

#include "defines.hpp"
#include "CycleStorageBase.hpp"
#include <unordered_map>

namespace EPL_DataCollect {
namespace plugins {

/*!
  * class CSPythonPluginStorage
  * \brief Structure used to store data describing the state of a plugin
  */
class CSPythonPluginStorage : public CycleStorageBase {
 public:
  // Constructors/Destructors
  //


  /*!
   * Empty Constructor
   */
  CSPythonPluginStorage();

  /*!
   * Empty Destructor
   */
  virtual ~CSPythonPluginStorage();

  CSPythonPluginStorage(const CSPythonPluginStorage &) = default;
  CSPythonPluginStorage(CSPythonPluginStorage &&)      = default;

  CSPythonPluginStorage &operator=(const CSPythonPluginStorage &) = default;
  CSPythonPluginStorage &operator=(CSPythonPluginStorage &&) = default;

  // Static Public attributes
  //

  // Public attributes
  //

  std::unordered_map<std::string, std::string> map;

  // Public attribute accessor methods
  //


  // Public attribute accessor methods
  //


  /*!
   * Set the value of map
   * \param new_var the new value of map
   */
  void setMap(std::unordered_map<std::string, std::string> new_var) { map = new_var; }

  /*!
   * Get the value of map
   * \return the value of map
   */
  std::unordered_map<std::string, std::string> *getMap() { return &map; }

  std::unique_ptr<CycleStorageBase> clone() override { return std::make_unique<CSPythonPluginStorage>(*this); }

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
};
}
}
