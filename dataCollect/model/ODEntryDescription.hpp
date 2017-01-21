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
 * \file ODEntryDescription.hpp
 * \brief Contains class ODEntryDescription
 * \todo IMPLEMENT
 */


#pragma once



#include "defines.hpp"
#include "ODEntry.hpp"

namespace EPL_DataCollect {

/*!
  * class ODEntryDescription
  * \brief Description of a specific OD entry
  */
class ODEntryDescription {
 public:
  // Constructors/Destructors
  //


  /*!
   * Empty Constructor
   */
  ODEntryDescription();

  /*!
   * Empty Destructor
   */
  virtual ~ODEntryDescription();

  ODEntryDescription( const ODEntryDescription & ) = default;
  ODEntryDescription( ODEntryDescription && )      = default;

  ODEntryDescription &operator=( const ODEntryDescription & ) = default;
  ODEntryDescription &operator=( ODEntryDescription && ) = default;

  // Static Public attributes
  //

  // Public attributes
  //

  std::string name;
  ODEntryType type;
  int         defaultValue;

  // Public attribute accessor methods
  //


  // Public attribute accessor methods
  //


  /*!
   * Set the value of name
   * \param new_var the new value of name
   */
  void setName( std::string new_var ) { name = new_var; }

  /*!
   * Get the value of name
   * \return the value of name
   */
  std::string getName() { return name; }

  /*!
   * Set the value of type
   * \param new_var the new value of type
   */
  void setType( ODEntryType new_var ) { type = new_var; }

  /*!
   * Get the value of type
   * \return the value of type
   */
  ODEntryType getType() { return type; }

  /*!
   * Set the value of defaultValue
   * \param new_var the new value of defaultValue
   */
  void setDefaultValue( int new_var ) { defaultValue = new_var; }

  /*!
   * Get the value of defaultValue
   * \return the value of defaultValue
   */
  int getDefaultValue() { return defaultValue; }

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
