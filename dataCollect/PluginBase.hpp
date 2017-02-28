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
 */

#pragma once

#include "defines.hpp"
#include "CaptureInstance.hpp"
#include "Cycle.hpp"
#include "EventBase.hpp"

namespace EPL_DataCollect {

class PluginManager;

/*!
  * class PluginBase
  * \brief Base class for all plugins
  *
  * Also provides an interface for registering cycle storage and generating events.
  */
class PluginBase {
 private:
  CaptureInstance *ciPTR = nullptr;

  bool runInitialize(CaptureInstance *ci);
  bool runReset(CaptureInstance *ci);

 public:
  PluginBase() = default;
  virtual ~PluginBase();

  PluginBase(const PluginBase &) = default;
  PluginBase(PluginBase &&)      = default;

  PluginBase &operator=(const PluginBase &) = default;
  PluginBase &operator=(PluginBase &&) = default;

  virtual void run(Cycle *cycle)        = 0;
  virtual std::string getDependencies() = 0;
  virtual std::string getID()           = 0;

  virtual bool initialize(CaptureInstance *ci) = 0;
  virtual bool reset(CaptureInstance *ci)      = 0;

  friend class PluginManager;

  bool addEvent(std::unique_ptr<EventBase> event) noexcept;
  CaptureInstance* getCI(){ return ciPTR; }

  template <class C, class... ARGS>
  inline bool registerCycleStorage(std::string index, ARGS &&... args);
};


/*!
 * \brief Adds a new cycle storage entry
 * Returns false when the index is already occupied
 *
 * \note this is a wrapper for the CaptureInstance function
 * \warning This function may only be called after or during initialize
 * \return bool
 * \param  index The index to register
 */
template <class C, class... ARGS>
inline bool PluginBase::registerCycleStorage(std::string index, ARGS &&... args) {
  if (ciPTR == nullptr) {
    return false;
  }

  return ciPTR->registerCycleStorage<C>(index, std::forward<ARGS>(args)...);
}


/*!
 * \fn void PluginBase::run(Cycle *cycle)
 * \brief Execute the plugin
 * \warning This function must be FAST
 * \note This function MUST be implemented by a plugin
 * \param  cycle The cycle to process
 */

/*!
 * \fn bool PluginBase::initialize(CaptureInstance *ci)
 * \brief initializes the plugin
 * \note This function MUST be implemented by a plugin
 * \param ci A pointer to the capture instance
 */

/*!
 * \fn bool PluginBase::reset(CaptureInstance *ci)
 * \brief resets the plugin
 * \note This function MUST be implemented by a plugin
 * \param ci A pointer to the capture instance
 */

/*!
 * \fn std::string PluginBase::getDependencies(CaptureInstance *ci)
 * \brief Returns a semicolon seperated list of dependencies
 * \note This function MUST be implemented by a plugin
 * \return the list as a std::string
 */

/*!
 * \fn std::string PluginBase::getID(CaptureInstance *ci)
 * \brief Returns an ID std::string
 * \note This function MUST be implemented by a plugin
 * \return std::string
 */
}
