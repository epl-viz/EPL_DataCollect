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
 * \file PluginManager.hpp
 * \brief Contains class PluginManager
 */

#pragma once

#include "defines.hpp"
#include "Cycle.hpp"
#include <memory>
#include <mutex>
#include <string>
#include <vector>

namespace EPL_DataCollect {

class CaptureInstance;
class PluginBase;

/*!
  * class PluginManager
  * \brief Central mechanism for handling all plugins
  */
class PluginManager {
 public:
  enum pmStatus { EDIT, INIT };

 private:
  pmStatus             status = EDIT;
  std::recursive_mutex accessMutex;

  std::vector<std::shared_ptr<PluginBase>> plugins;
  std::vector<PluginBase *>                pluginsOrdered;

  std::vector<std::string> splitString(std::string const &str, char c) const noexcept;

 public:
  PluginManager() = default;
  virtual ~PluginManager();

  PluginManager(const PluginManager &) = delete;
  PluginManager(PluginManager &&)      = delete;

  PluginManager &operator=(const PluginManager &) = delete;
  PluginManager &operator=(PluginManager &&) = delete;

  mockable int processCycle(Cycle *cycle) noexcept;

  mockable bool init(CaptureInstance *ci) noexcept;
  mockable bool reset(CaptureInstance *ci) noexcept;
  mockable bool canEditPlugins() noexcept;

  mockable bool addPlugin(std::shared_ptr<PluginBase> cmd) noexcept;
  mockable std::vector<std::string> getPluginList() noexcept;
  mockable bool removePlugin(std::string pluginID) noexcept;
};
}
