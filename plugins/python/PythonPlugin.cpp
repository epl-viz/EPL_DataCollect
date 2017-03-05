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
 * \file PythonPlugin.cpp
 * \brief Contains class PythonPlugin
 */

#include "PythonPlugin.hpp"
#include "Cycle.hpp"
#include "EvPluginText.hpp"
#include "EvView.hpp"
#include "EventBase.hpp"
#include "PyStorage.hpp"
#include "EPLEnums.h"
#include "Python.h"
#include "iostream"
#include <memory>
#include <string>

#define PLUGIN_ID "getID"
#define PLUGIN_RUN "run"
#define PLUGIN_DEP "getDependencies"
#define PLUGIN_INIT "initialize"
#define PLUGIN_PARENT "Plugin.Plugin"
#define PYTHON_STR "str"
// #define PLUGIN_EV "PluginEvent" // clang: macro not in use

namespace EPL_DataCollect {
namespace plugins {

// static stuff
std::unordered_map<std::string, PythonPlugin *> PythonPlugin::plugins;

/**
 * @brief Getting a pointer to the python plugin by id.
 *
 * @param name p_name: name of plugin
 * @return EPL_DataCollect::plugins::PythonPlugin* pointer to plugin or null
 */
PythonPlugin *PythonPlugin::getPythonPlugin(std::string name) { return plugins[name]; }


// const / dest
PythonPlugin::PythonPlugin() {}

PythonPlugin::~PythonPlugin() {}


/**
 * @brief Reserving a plugin
 *
 * @param pluginName p_pluginName:...
 */
PythonPlugin::PythonPlugin(std::string pluginName) { plugID = pluginName; };

/**
 * @brief Getting Plugin ID
 *
 * @return std::__cxx11::string the plugin ID
 */
std::string PythonPlugin::getID() { return plugID; };

/**
 * @brief Getting Plugin Dependencies
 *
 * @return std::__cxx11::string plugin dependencies
 */
std::string PythonPlugin::getDependencies() { return plugDeps; };

/**
 * @brief Getting the currently processing cycle
 *
 * @return EPL_DataCollect::Cycle* current cycle to be processed
 */
Cycle *PythonPlugin::getCurrentCycle() { return currentCycle; }

/**
 * @brief Getting a cycle by cycle number. Saving this cycle for use.
 *
 * @param number p_number: the cycle number
 * @return EPL_DataCollect::Cycle* pointer to cycle with number, or latest available if number invalid
 */
Cycle *PythonPlugin::getCycleByNum(unsigned int number) {
  if (workingCycle.getCycleNum() != static_cast<uint32_t>(number) && getCI() != nullptr) {
    workingCycle = getCI()->getCycleContainer()->getCycle(static_cast<uint32_t>(number));
  }
  return &workingCycle;
}


// storage specific methods
/**
 * @brief Getting the storage item string at index. This data is plugin specific !
 *
 * @param index p_index: index at where to get data
 * @return std::__cxx11::string data as string, empty string if no ci found
 */
std::string PythonPlugin::getStorage(std::string index) {
  if (getCurrentCycle()->getCycleStorage(plugID) == nullptr) // that should actually not happen in the first place
    return "";

  auto *map = dynamic_cast<CSPythonPluginStorage *>(getCurrentCycle()->getCycleStorage(plugID))->getMap();
  return (*map)[index];
};

/**
 * @brief Setting storage at specific index for later usage. Storage is plugin specific !
 *
 * @param index p_index: where to store
 * @param var p_var: what to store
 * @return bool storage op successful or not
 */
bool PythonPlugin::setStorage(std::string index, std::string var) {
  if (getCurrentCycle()->getCycleStorage(plugID) == nullptr) // that should actually not happen in the first place
    return false;

  auto *map     = dynamic_cast<CSPythonPluginStorage *>(getCurrentCycle()->getCycleStorage(plugID))->getMap();
  (*map)[index] = var;
  return true;
};

// data specific methods
/**
 * @brief Getting data (that is plugin wide) as string
 *
 * @param index p_index: data from where
 * @return std::__cxx11::string data as string, empty string if not there
 */
std::string PythonPlugin::getData(std::string index) {
  if (getCurrentCycle()->getCycleStorage(index) == nullptr)
    return "";
  return getCurrentCycle()->getCycleStorage(index)->getStringRepresentation();
};

/**
 * @brief Setting data string at index, adding a PyStorageStr for access from all plugins
 *
 * @param index p_index: index
 * @param var p_var: what to be stored
 * @return bool success or not
 */
bool PythonPlugin::setDataStr(std::string index, std::string var) {
  if (getCurrentCycle()->getCycleStorage(index) == nullptr)
    return false;

  auto *data = dynamic_cast<PyStorageStr *>(getCurrentCycle()->getCycleStorage(index));

  if (data == nullptr)
    return false;

  data->data = var;
  return true;
};

/**
 * @brief Setting data int at index, adding a PyStorageStr for access from all plugins
 *
 * @param index p_index: index
 * @param var p_var: what to be stored
 * @return bool success or not
 */
bool PythonPlugin::setDataInt(std::string index, int var) {
  if (getCurrentCycle()->getCycleStorage(index) == nullptr)
    return false;

  auto *data = dynamic_cast<PyStorageInt *>(getCurrentCycle()->getCycleStorage(index));

  if (data == nullptr)
    return false;

  data->data = var;
  return true;
};

/**
 * @brief Registering an appropriate Cycle storage
 *
 * @param index p_index: where to store
 * @param typeAsInt p_typeAsInt: either 1 (int) or 2 (string)
 * @return bool success
 */
bool PythonPlugin::registerPyCycleStorage(std::string index, int typeAsInt) {
  switch (typeAsInt) {
    case 1: // adding int
      return registerCycleStorage<PyStorageInt>(index);
    case 2: // adding string
      return registerCycleStorage<PyStorageStr>(index);
  }
  return false;
};


// event specific methods
/**
 * @brief Events to be added, coded as key (ev type), value and argument as strings
 *
 * @param key p_key: EvType enum
 * @param value p_value: value to be added (cn, od entry, text etc)
 * @param argument p_argument: additional arguments (level of highlighting etc)
 * @return bool success or not
 */
bool PythonPlugin::addPyEvent(int key, std::string value, std::string argument) {
  int val;
  int arg;
  switch (static_cast<EvType>(key)) {
    case EvType::VIEW_STARTCAP: // add event STARTCAP
      return addEvent(std::make_unique<EvView>(EvType::VIEW_STARTCAP,
                                               getID(),
                                               std::string("PluginEvent"),
                                               std::string("Starting live view..."),
                                               0,
                                               getCurrentCycle(),
                                               EventBase::INDEX_MAP()));
    case EvType::VIEW_ENDCAP: // add event ENDCAP
      return addEvent(std::make_unique<EvView>(EvType::VIEW_ENDCAP,
                                               getID(),
                                               std::string("PluginEvent"),
                                               std::string("Stopping live view..."),
                                               0,
                                               getCurrentCycle(),
                                               EventBase::INDEX_MAP()));
    case EvType::VIEW_EV_HIGHLIGHT_MN: // add event highlight MN
      return addEvent(std::make_unique<EvView>(EvType::VIEW_EV_HIGHLIGHT_MN,
                                               getID(),
                                               std::string("PluginEvent"),
                                               std::string("Highlighting MN..."),
                                               0,
                                               getCurrentCycle(),
                                               EventBase::INDEX_MAP()));
    case EvType::VIEW_EV_HIGHLIGHT_CN: // add event highlight CN x (given by value)
      try {
        val = std::stoi(value);
        if (val < 0)
          return false;
      } catch (std::invalid_argument ia) { return false; }
      return addEvent(std::make_unique<EvView>(EvType::VIEW_EV_HIGHLIGHT_CN,
                                               getID(),
                                               std::string("PluginEvent"),
                                               std::string("Highlighting CN\t" + value),
                                               val,
                                               getCurrentCycle(),
                                               EventBase::INDEX_MAP()));
    case EvType::VIEW_EV_JUMPTOTIME: // add event jump to time (given by value)
      try {
        val = std::stoi(value);
        if (val < 0)
          return false;
      } catch (std::invalid_argument ia) {
        std::cerr << "Invalid event argument" << std::endl;
        return false;
      }
      return addEvent(std::make_unique<EvView>(EvType::VIEW_EV_JUMPTOTIME,
                                               getID(),
                                               std::string("PluginEvent"),
                                               std::string("Jump to time\t" + value),
                                               val,
                                               getCurrentCycle(),
                                               EventBase::INDEX_MAP()));
    case EvType::VIEW_EV_HIGHLIGHT_OD_ENTRY: // add event highlight od entry (given by value)
      try {
        val = std::stoi(value);
        arg = std::stoi(argument);
        if (val < 0x0000 || val > 0xFFFF)
          return false;
        if (arg < 0 || val > 100)
          return false;
      } catch (std::invalid_argument ia) {
        std::cerr << "Invalid event argument" << std::endl;
        return false;
      }
      return addEvent(std::make_unique<EvView>(EvType::VIEW_EV_HIGHLIGHT_OD_ENTRY,
                                               getID(),
                                               std::string("PluginEvent"),
                                               argument,
                                               val,
                                               getCurrentCycle(),
                                               EventBase::INDEX_MAP()));
    case EvType::VIEW_EV_TEXT: // add event text
      return addEvent(std::make_unique<EvPluginText>(
            getID(), std::string("PluginEvent"), value, 0, getCurrentCycle(), EventBase::INDEX_MAP()));
    default: return false;
  }
};


// plugin main methods
/**
 * @brief Loads and checks the current plugin. Make sure that pythons sys.path is set to
 *        cython libs and plugin folder !
 *
 * @param ci p_ci: ci the plugin should be run with
 * @return bool whether successful, also prints on std::cerr on error
 */
bool PythonPlugin::initialize(CaptureInstance *ci) {
  (void)ci; // has already been inited by runInitialize(...) before

  //** Python Initialization
  pName = PyUnicode_DecodeFSDefault(plugID.c_str());

  // check if module is present
  pModule = PyImport_Import(pName);
  Py_DECREF(pName);
  if (pModule == NULL) {
    std::cerr << "Module\t'" << plugID << "'\t could not be loaded" << std::endl;
    return false;
  }

  // check if correct class is available
  pDict  = PyModule_GetDict(pModule);
  pClass = PyDict_GetItemString(pDict, plugID.c_str());
  if (pClass == NULL) {
    std::cerr << "Class of Plugin\t'" << plugID << "'\t could not be created" << std::endl;
    return false;
  }

  if (!PyCallable_Check(pClass)) {
    std::cerr << "Class\t'" << plugID << "'\t not callable" << std::endl;
    return false;
  }
  pInstance = PyObject_CallObject(pClass, NULL);

  // check if instance could be created, and correct instance of super class Plugin
  if (pInstance == NULL) {
    std::cerr << "Class instance of\t'" << plugID << "'\t could not be created" << std::endl;
    return false;
  }

  if (strcmp(pInstance->ob_type->tp_base->tp_name, PLUGIN_PARENT) != 0) {
    std::cerr << "Plugin class\t'" << plugID << "'\t does not derive from correct parent class\t" << PLUGIN_PARENT
              << std::endl;
    return false;
  }

  // check if (correct) getID() method is implemented
  pValue = PyObject_CallMethod(pInstance, reinterpret_cast<const char *>(PLUGIN_ID), NULL);
  if (pValue == NULL || strcmp(pValue->ob_type->tp_name, PYTHON_STR) != 0) {
    std::cerr << "Plugin method\t" << PLUGIN_ID << "\tof plugin\t'" << plugID << "'\t is not or incorrectly implemented"
              << std::endl;
    return false;
  }

  pValue         = PyUnicode_AsUTF8String(pValue);
  std::string id = PyBytes_AsString(pValue);
  Py_DECREF(pValue);
  if (id.compare(plugID) != 0) {
    std::cerr << "Plugin method" << PLUGIN_ID << "\tof plugin\t'" << plugID << "'\t does not return class name"
              << std::endl;
    return false;
  }

  // retrieving dependencies
  pValue = PyObject_CallMethod(pInstance, reinterpret_cast<const char *>(PLUGIN_DEP), NULL); // can't return NULL
  Py_DECREF(pValue);

  if (strcmp(pValue->ob_type->tp_name, "str") != 0) {
    std::cerr << "Plugin method\t" << PLUGIN_DEP << "\tof plugin\t'" << plugID << "'\t has incorrect return type"
              << std::endl;
    return false;
  }
  pValue   = PyUnicode_AsUTF8String(pValue);
  plugDeps = PyBytes_AsString(pValue);

  //** instancing was successful, initing needed C++ stuff...
  if (!registerCycleStorage<CSPythonPluginStorage>(plugID)) {
    std::cerr << "Plugin \t'" << plugID << "'\t cycle storage not available" << std::endl;
    return false;
  }

  plugins[plugID] = this;

  // now check if the initialization of the plugin is successful in python
  pValue = PyObject_CallMethod(pInstance, reinterpret_cast<const char *>(PLUGIN_INIT), NULL); // can't return NULL
  if (pValue != Py_True) {
    std::cerr << "Plugin \t'" << plugID << "'\t Python initialization failed" << std::endl;
    return false;
  }
  Py_DECREF(pValue);
  return true;
};

/**
 * @brief Calling the plugins run method and updating the cycle. The python plugin may
 *        use the updated cycle
 *
 * @param cycle p_cycle: to cycle to be used from python plugin
 */
void PythonPlugin::run(Cycle *cycle) {
  if (!running)
    return;
  if (cycle != nullptr)
    currentCycle = cycle;
  PyObject_CallMethod(pInstance, reinterpret_cast<const char *>(PLUGIN_RUN), NULL); // can't fail so no checking
};

/**
 * @brief Set running, for python unload
 *
 * @param newRunning p_newRunning: new value of running
 */
void PythonPlugin::setRunning(bool newRunning) { running = newRunning; }

bool PythonPlugin::reset(CaptureInstance *ci) {
  (void)ci;
  return true; // nothing to do here
};
}
}
