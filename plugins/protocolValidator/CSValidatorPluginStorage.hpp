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
 * \file CSValidatorPluginStorage.hpp
 * \brief Contains class CSValidatorPluginStorage
 */


#pragma once

#include "defines.hpp"
#include "CycleStorageBase.hpp"
#include "EPLEnums.h"
#include <unordered_map>

namespace EPL_DataCollect {
namespace plugins {

/*!
  * class CSValidatorPluginStorage
  * \brief Structure used to store data describing the state of a plugin (validator plugin)
  */
class CSValidatorPluginStorage : public CycleStorageBase {
 public:
  CSValidatorPluginStorage();
  virtual ~CSValidatorPluginStorage();

  CSValidatorPluginStorage(const CSValidatorPluginStorage &) = default;
  CSValidatorPluginStorage(CSValidatorPluginStorage &&)      = default;

  CSValidatorPluginStorage &operator=(const CSValidatorPluginStorage &) = default;
  CSValidatorPluginStorage &operator=(CSValidatorPluginStorage &&) = default;

  std::unordered_map<uint8_t, NMTState> nodeStatus;
  std::unordered_map<uint8_t, int64_t>  pReqTimes; // mapping the last poll request

  double getNumericValue() override { return 0; }

  bool isNumericValue() override { return false; }


  /*!
   * Set the value of map
   * \param new_nodeStatus the new value of map
   */
  void setMap(std::unordered_map<uint8_t, NMTState> new_nodeStatus) { nodeStatus = new_nodeStatus; }

  /*!
   * Get the value of map
   * \return the value of map
   */
  std::unordered_map<uint8_t, NMTState> *getNodeStatus() { return &nodeStatus; }

  std::unordered_map<uint8_t, int64_t> *getpReqTimes() { return &pReqTimes; }



  std::unique_ptr<CycleStorageBase> clone() override { return std::make_unique<CSValidatorPluginStorage>(*this); }
};
}
}
