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
 * \todo IMPLEMENT
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

namespace EPL_DataCollect {
namespace plugins {

// Constructors/Destructors
//
std::unordered_map<std::string, PythonPlugin *> PythonPlugin::plugins;

PythonPlugin::PythonPlugin() {}


// BEFORE CALLING MAKE SURE PYTHONPATH IS SET TO PLUGIN DIRECTIONARY !!!
PythonPlugin::PythonPlugin(std::string pluginName) { plugID = pluginName; };

PythonPlugin::~PythonPlugin() {}

std::string PythonPlugin::getID() { return plugID; };

std::string PythonPlugin::getDependencies() { return plugDeps; };

bool PythonPlugin::initialize(CaptureInstance *ci) {
  (void)ci;

  //** Python Initialization
  pName = PyUnicode_DecodeFSDefault(plugID.c_str());

  // check if module is present
  pModule = PyImport_Import(pName);
  Py_DECREF(pName);
  if (pModule == NULL) {
    std::cerr << "Module\t'" + plugID + "'\t could not be loaded\n";
    return false;
  }

  // check if correct class is available
  pDict  = PyModule_GetDict(pModule);
  pClass = PyDict_GetItemString(pDict, plugID.c_str());
  if (pClass == NULL) {
    std::cerr << "Class of Plugin\t'" + plugID + "'\t could not be created\n";
    return false;
  }

  if (!PyCallable_Check(pClass)) {
    std::cerr << "Class\t'" + plugID + "'\t not callable\n";
    return false;
  }
  pInstance = PyObject_CallObject(pClass, NULL);

  // check if instance could be created, and correct instance of super class Plugin
  if (pInstance == NULL) {
    std::cerr << "Class instance of\t'" + plugID + "'\t could not be created\n";
    return false;
  }

  if (strcmp(pInstance->ob_type->tp_base->tp_name, "Plugin.Plugin") != 0) {
    std::cerr << "Plugin class\t'" + plugID + "'\t does not derive from correct parent class\n";
    return false;
  }

  // check if (correct) getID() method is implemented
  pValue = PyObject_CallMethod(pInstance, reinterpret_cast<const char *>("getID"), NULL);
  if (pValue == NULL || strcmp(pValue->ob_type->tp_name, "str") != 0) {
    std::cerr << "Plugin method getID() of plugin\t'" + plugID + "'\t is not or incorrectly implemented\n";
    return false;
  }

  pValue         = PyUnicode_AsUTF8String(pValue);
  std::string id = PyBytes_AsString(pValue);
  Py_DECREF(pValue);
  if (id.compare(plugID) != 0) {
    std::cerr << "Plugin method getID() of plugin\t'" + plugID + "'\t does not return class name\n";
    return false;
  }

  // retrieving dependencies
  pValue = PyObject_CallMethod(pInstance, reinterpret_cast<const char *>("getDependencies"), NULL); // can't return NULL
  Py_DECREF(pValue);

  if (strcmp(pValue->ob_type->tp_name, "str") != 0) {
    std::cerr << "Plugin method getDependencies() of plugin\t'" + plugID + "'\t has incorrect return type\n";
    return false;
  }
  pValue   = PyUnicode_AsUTF8String(pValue);
  plugDeps = PyBytes_AsString(pValue);

  // check if pluginID is already in storage, in this case no addition possible
  if (plugins.find(plugID) != plugins.end()) {
    std::cerr << "Plugin with ID\t'" + plugID + "'\t already in database\n";
    return false;
  }

  // now check if the initialization of the plugin is successful in python
  pValue = PyObject_CallMethod(pInstance, reinterpret_cast<const char *>("initialize"), NULL); // can't return NULL
  if (pValue != Py_True) {
    std::cerr << "Plugin \t'" + plugID + "'\t Python initialization failed\n";
    return false;
  }
  Py_DECREF(pValue);


  //** instancing was successful, initing needed C++ stuff...

  if (!registerCycleStorage<CSPythonPluginStorage>(plugID)) {
    std::cerr << "Plugin \t'" + plugID + "'\t cycle storage not available\n";
    return false;
  }

  plugins[plugID] = this;
  return true;
};

void PythonPlugin::run(Cycle *cycle) {
  currentCycle = cycle;
  PyObject_CallMethod(pInstance, reinterpret_cast<const char *>("run"), NULL); // can't fail so no checking
};

bool PythonPlugin::reset(CaptureInstance *ci) {
  (void)ci;
  return false;
};

PythonPlugin *PythonPlugin::getPythonPlugin(std::string name) { return plugins[name]; }

Cycle *PythonPlugin::getCurrentCycle() { return currentCycle; }

Cycle *PythonPlugin::getCycleByNum(int number) {
  if (workingCycle.getCycleNum() != static_cast<uint32_t>(number) && getCI() != nullptr) {
    workingCycle = getCI()->getCycleContainer()->getCycle(static_cast<uint32_t>(number));
  }
  return &workingCycle;
}



bool PythonPlugin::addPyEvent(int key, const char *value) {
  char *   end_ptr;
  uint64_t var = strtoull(value, &end_ptr, 0);

  switch (key) {
    case 0: // add event STARTCAP
      return addEvent(std::make_unique<EvView>(EvType::VIEW_STARTCAP,
                                               getID(),
                                               std::string("PluginEvent"),
                                               std::string("Starting live view..."),
                                               0,
                                               getCurrentCycle(),
                                               EventBase::INDEX_MAP()));
    case 1: // add event ENDCAP
      return addEvent(std::make_unique<EvView>(EvType::VIEW_ENDCAP,
                                               getID(),
                                               std::string("PluginEvent"),
                                               std::string("Stopping live view..."),
                                               0,
                                               getCurrentCycle(),
                                               EventBase::INDEX_MAP()));
    case 2: // add event highlight MN
      if (value == end_ptr)
        return false;
      return addEvent(std::make_unique<EvView>(EvType::VIEW_EV_HIGHLIGHT_MN,
                                               getID(),
                                               std::string("PluginEvent"),
                                               std::string("Starting live capture..."),
                                               var,
                                               getCurrentCycle(),
                                               EventBase::INDEX_MAP()));
    case 3: // add event highlight CN x (given by value)
      if (value == end_ptr)
        return false;
      return addEvent(std::make_unique<EvView>(EvType::VIEW_EV_HIGHLIGHT_CN,
                                               getID(),
                                               std::string("PluginEvent"),
                                               std::string("Starting live capture..."),
                                               var,
                                               getCurrentCycle(),
                                               EventBase::INDEX_MAP()));
    case 4: // add event jump to time (given by value)
      if (value == end_ptr)
        return false;
      return addEvent(std::make_unique<EvView>(EvType::VIEW_EV_JUMPTOTIME,
                                               getID(),
                                               std::string("PluginEvent"),
                                               std::string("Starting live capture..."),
                                               var,
                                               getCurrentCycle(),
                                               EventBase::INDEX_MAP()));
    case 5: // add event highlight od entry (given by value)
      if (value == end_ptr)
        return false;
      return addEvent(std::make_unique<EvView>(EvType::VIEW_EV_HIGHLIGHT_OD_ENTRY,
                                               getID(),
                                               std::string("PluginEvent"),
                                               std::string("Starting live capture..."),
                                               var,
                                               getCurrentCycle(),
                                               EventBase::INDEX_MAP()));
    case 6: // add event text
      return addEvent(std::make_unique<EvPluginText>(
            getID(), std::string("PluginEvent"), std::string(value), 0, getCurrentCycle(), EventBase::INDEX_MAP()));
  }
  return false;
};




bool PythonPlugin::registerPyCycleStorage(std::string index, int typeAsInt) {
  // create IntStorage / str storage / boolStorage class and then call registerCycleStorage<IntStorage>("stringindex!")
  // !!!
  // typeAsStr is either "bool", "int", or "str" -> register appropriately
  switch (typeAsInt) {
    case 1: // adding int
      break;
    case 2: // adding string
      break;
  }
  std::cout << "\nregging\t" + index + ":Type" + std::to_string(typeAsInt) + "\n";
  return true;
};

bool PythonPlugin::setStorage(std::string index, std::string var) {
  (void)index;
  (void)var;
  return false;
};

std::string PythonPlugin::getStorage(std::string index) {
  (void)index;
  return std::string("IT IS false") + index;
};

std::string PythonPlugin::getData(std::string index) {
  (void)index;
  return ""; // have to return empty string !!! otherwise seg
};

bool PythonPlugin::setDataStr(std::string index, std::string var) {
  (void)index;
  (void)var;
  return true;
};

bool PythonPlugin::setDataInt(std::string index, int var) {
  (void)index;
  (void)var;
  return true;
};
}
}
