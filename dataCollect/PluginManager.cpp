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
 * \file PluginManager.cpp
 * \brief Contains class PluginManager
 */

#include "PluginManager.hpp"
#include <iostream>
#include <plf_colony.h>

namespace EPL_DataCollect {

PluginManager::~PluginManager() {}


/*!
 * \brief adds the plugin
 * Does nothing if the plugin is already added
 * \param plugin The plugin to add
 * \returns true on success and false if the plugin already exists
 */
bool PluginManager::addPlugin(std::shared_ptr<PluginBase> plugin) noexcept {
  std::lock_guard<std::recursive_mutex> lock(accessMutex);
  if (!canEditPlugins())
    return false;

  for (auto i : plugins) {
    if (i->getID() == plugin->getID())
      return false;
  }

  plugins.emplace_back(plugin);
  return true;
}

bool PluginManager::removePlugin(std::string pluginID) noexcept {
  std::lock_guard<std::recursive_mutex> lock(accessMutex);
  if (!canEditPlugins())
    return false;

  auto it = plugins.begin();
  while (it != plugins.end()) {
    auto i = it->get();
    if (i->getID() == pluginID) {
      plugins.erase(it);
      return true;
    }
    ++it;
  }

  return false;
}

/*!
 * \brief Processes all plugins
 * Can only be called when all plugins are initialized
 * \returns 0 on success
 * \returns 1 if not init
 * \param  cycle The cycle to process
 */
int PluginManager::processCycle(Cycle *cycle) noexcept {
  std::lock_guard<std::recursive_mutex> lock(accessMutex);
  if (status != INIT)
    return 1;

  for (auto i : pluginsOrdered)
    i->run(cycle);

  return 0;
}

/*!
 * \brief Splits a string by c into a vector of strings
 */
std::vector<std::string> PluginManager::splitString(std::string const &str, char c) const noexcept {
  std::vector<std::string> res;
  for (auto i : str) {
    if (res.empty())
      res.emplace_back();

    if (i == c) {
      res.emplace_back();
      continue;
    }

    res.back() += i;
  }
  return res;
}

/*!
 * \brief Initializes all added plugins
 * Plugins can now no longer be added or deleted
 * \returns false if dependencies could not be resolved
 */
bool PluginManager::init(CaptureInstance *ci) noexcept {
  std::lock_guard<std::recursive_mutex> lock(accessMutex);
  if (status == INIT)
    return false;

  plf::colony<PluginBase *> todo;
  for (auto i : plugins) {
    todo.insert(i.get());
  }

  plf::colony<std::string> depDone;

  pluginsOrdered.clear();

  // Add plugins
  while (!todo.empty()) {
    size_t oldSize = todo.size();

    auto it = todo.begin();
    while (it != todo.end()) {
      auto                     i      = *it;
      std::string              depStr = i->getDependencies();
      std::vector<std::string> deps   = splitString(depStr, ';');

      bool missingDep = false;

      // Checks if plugin can be added now
      for (auto const &j : deps) {
        bool foundDep = false;
        for (auto const &k : depDone) {
          if (j == k) {
            foundDep = true;
            break;
          }
        }

        // Can not add plugin (yet)
        if (!foundDep) {
          missingDep = true;
          break;
        }
      }

      if (missingDep) {
        ++it;
        continue;
      }

      depDone.insert(i->getID());
      pluginsOrdered.emplace_back(i);
      it = todo.erase(it);
    }

    // We are stuck in a dependency cycle or a dependnecy is missing
    if (todo.size() >= oldSize) {
      return false;
    }
  }

  for (auto i : pluginsOrdered) {
    i->initialize(ci);
  }

  status = INIT;
  return true;
}


/*!
 * \brief Resets all plugins but does not delete them.
 * Plugins may be added and deleted now
 */
bool PluginManager::reset(CaptureInstance *ci) noexcept {
  std::lock_guard<std::recursive_mutex> lock(accessMutex);
  if (status == EDIT)
    return false;

  for (auto i : pluginsOrdered) {
    i->reset(ci);
  }

  pluginsOrdered.clear();

  status = EDIT;
  return true;
}

/*!
 * \brief Returns whether or not the plugin list can be edited
 */
bool PluginManager::canEditPlugins() noexcept {
  std::lock_guard<std::recursive_mutex> lock(accessMutex);
  return status == EDIT;
}

/*!
 * \brief Returns a list of Plugin IDS
 */
std::vector<std::string> PluginManager::getPluginList() noexcept {
  std::lock_guard<std::recursive_mutex> lock(accessMutex);
  std::vector<std::string>              plList;

  plList.resize(plugins.size());

  for (auto i : plugins) {
    plList.emplace_back(i->getID());
  }

  return plList;
}
}
