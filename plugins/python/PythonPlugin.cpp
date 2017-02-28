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
#include "EvView.hpp"
#include "EventBase.hpp"
#include "EPLEnums.h"
#include "Python.h"
#include "iostream"
#include <memory>
#include <string>
#include "EvPluginText.hpp"

namespace EPL_DataCollect {
namespace plugins {

// Constructors/Destructors
//
std::unordered_map<std::string, PythonPlugin *> PythonPlugin::plugins;
Cycle *PythonPlugin::currentCycle;

PythonPlugin::PythonPlugin() {}


// BEFORE CALLING MAKE SURE PYTHONPATH IS SET TO PLUGIN DIRECTIONARY !!!
PythonPlugin::PythonPlugin(std::string pluginName) {
  plugName = pluginName;

  plugins[plugName] = this;

  // Python INIT
  pName   = PyUnicode_DecodeFSDefault(plugName.c_str());
  pModule = PyImport_Import(pName);
  Py_DECREF(pName);

  if (pModule != NULL) {
    pDict  = PyModule_GetDict(pModule);
    pClass = PyDict_GetItemString(pDict, plugName.c_str());

    if (PyCallable_Check(pClass)) {
      pInstance = PyObject_CallObject(pClass, NULL);
    }
  }

  registerCycleStorage<CSPythonPluginStorage>(pluginName);
  // initing
};

PythonPlugin::~PythonPlugin() {}

bool PythonPlugin::initialize(CaptureInstance *ci) {
  (void)ci;
  pValue = PyObject_CallMethod(pInstance, reinterpret_cast<const char *>("initialize"), NULL);
  if (pValue != NULL) {
    Py_DECREF(pValue);
    return true;
  }
  return false;
};

void PythonPlugin::run(Cycle *cycle) {
  currentCycle = cycle;
  PyObject_CallMethod(pInstance, reinterpret_cast<const char *>("run"), NULL);
};

bool PythonPlugin::reset(CaptureInstance *ci) {
  (void)ci;
  return false;
};

Cycle *PythonPlugin::getCurrentCycle() { return currentCycle; }

PythonPlugin *PythonPlugin::getPythonPlugin(const char *name) { return plugins[std::string(name)]; }

std::string PythonPlugin::getDependencies() {
  std::string ret_val = "";
  pValue              = PyObject_CallMethod(pInstance, reinterpret_cast<const char *>("getDependencies"), NULL);
  if (pValue != NULL) {
    pValue  = PyUnicode_AsUTF8String(pValue);
    ret_val = PyBytes_AsString(pValue);
    Py_DECREF(pValue);
  }
  return ret_val; // return empty if no function is given, this is OK !
};

std::string PythonPlugin::getID() {
  std::string ret_val = "";
  pValue              = PyObject_CallMethod(pInstance, reinterpret_cast<const char *>("getID"), NULL);
  if (pValue != NULL) {
    pValue  = PyUnicode_AsUTF8String(pValue);
    ret_val = PyBytes_AsString(pValue);
    Py_DECREF(pValue);
  }
  return ret_val;
};

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
      return addEvent(std::make_unique<EvPluginText>(getID(),
                                                     std::string("PluginEvent"),
                                                     std::string(value),
                                                     0,
                                                     getCurrentCycle(),
                                                     EventBase::INDEX_MAP()));
  }
  return false;
};

Cycle* PythonPlugin::getCycleWithNum(const char* name, int number) {
  (void) name;
  (void) number;
  return NULL;
}

bool PythonPlugin::registerPyCycleStorage(const char *index, int typeAsInt) {
  (void)index;
  (void)typeAsInt;

  // TODO: IMPLEMENT THIS METHOD IS KEY
  // create IntStorage / str storage / boolStorage class and then call registerCycleStorage<IntStorage>("stringindex!")
  // !!!
  // typeAsStr is either "bool", "int", or "str" -> register appropriately
  switch (typeAsInt) {
    case 1:
      break; // adding int
    case 2:
      break; // adding string
  }
  std::cout << "\nregging\t" + std::string(index) + ":";
  return true;
};

bool PythonPlugin::setStorage(const char *index, const char *var) {
  (void)index;
  (void)var;
  return false;
};

std::string PythonPlugin::getStorage(const char *index) {
  (void)index;
  return std::string("IT IS false") + std::string(index);
};

std::string PythonPlugin::getData(const char *index) {
  (void)index;
  return "";
};

bool PythonPlugin::setDataStr(const char *index, const char *var) {
  (void)index;
  (void)var;
  return true;
};

bool PythonPlugin::setDataInt(const char *index, int var) {
  (void)index;
  (void)var;
  return true;
};
}
}
