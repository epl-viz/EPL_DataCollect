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
 * \file CycleStorageBase.hpp
 * \brief Contains class CycleStorageBase
 */


#pragma once

#include "defines.hpp"
#include <memory>

namespace EPL_DataCollect {

/*!
  * class CycleStorageBase
  * \brief Base class for OD like storage in a cycle
  *
  * Contains additional generic information that is not in the OD.
  * Should only be used for communication between plugins and the frontend or for storing cycle
  * dependent information.
  */
class CycleStorageBase {
 public:
  CycleStorageBase()          = default;
  virtual ~CycleStorageBase() = default;

  CycleStorageBase(const CycleStorageBase &) = default;
  CycleStorageBase(CycleStorageBase &&)      = default;

  CycleStorageBase &operator=(const CycleStorageBase &) = default;
  CycleStorageBase &operator=(CycleStorageBase &&) = default;

  /*!
   * \brief Returns a numeric Representation of the stored information
   * \return double
   */
  virtual double getNumericValue() = 0;

  /*!
   * \brief Returns whether the Entry can be represented as ONE numeric value
   * \return bool
   */
  virtual bool isNumericValue() = 0;

  virtual std::unique_ptr<CycleStorageBase> clone() = 0;
};
}
