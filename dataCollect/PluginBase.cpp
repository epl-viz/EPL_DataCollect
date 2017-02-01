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
 * \file PluginBase.cpp
 * \brief Contains class PluginBase
 * \todo IMPLEMENT
 */

#include "PluginBase.hpp"
#include <iostream>

namespace EPL_DataCollect {

PluginBase::~PluginBase() {}

/*!
 * \brief Adds an event to the event log
 * \return true on success
 * \warning This function may only be called after or during initialize
 * \param  event The event to add
 */
bool PluginBase::addEvent(std::unique_ptr<EventBase> event) noexcept {
  if (ciPTR == nullptr)
    return false;

  ciPTR->getEventLog()->addEvent(std::move(event));
  return true;
}

bool PluginBase::runInitialize(CaptureInstance *ci) {
  ciPTR    = ci;
  bool ret = initialize(ci);

  if (!ret) {
    std::cerr << "[PluginBase] Failed to initialize plugin " << getID() << std::endl;
  }

  return ret;
}

bool PluginBase::runReset(CaptureInstance *ci) {
  bool ret = reset(ci);

  if (!ret) {
    std::cerr << "[PluginBase] Failed to reset plugin " << getID() << std::endl;
  }

  ciPTR = nullptr;
  return ret;
}
}
