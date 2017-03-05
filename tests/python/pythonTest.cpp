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

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wold-style-cast"

#include "PluginManager.hpp"
#include <CaptureInstance.hpp>
#include <Cycle.hpp>
#include <EvView.hpp>
#include <PythonPlugin.hpp>
#include <catch.hpp>
#include <fakeit.hpp>
#include <iostream>

#pragma clang diagnostic pop

#if __cplusplus <= 201402L
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#else
#include <filesystem>
namespace fs = std::filesystem;
#endif

using namespace fakeit;
using namespace EPL_DataCollect;
using namespace EPL_DataCollect::plugins;

TEST_CASE("Loading python files", "[python]") {
  std::cout << std::endl;
  auto pyPlugin_noFile             = std::make_shared<PythonPlugin>("Test_NOFILE");
  auto pyPlugin_emptyFile          = std::make_shared<PythonPlugin>("Test_EmptyFile");
  auto pyPlugin_classNotAvailable  = std::make_shared<PythonPlugin>("Test_ClassNotAvailable");
  auto pyPlugin_classIllegalParent = std::make_shared<PythonPlugin>("Test_ClassIllegalParent");
  auto pyPlugin_noIDMethod         = std::make_shared<PythonPlugin>("Test_NoIDMethod");
  auto pyPlugin_illegalIDMethod    = std::make_shared<PythonPlugin>("Test_IllegalIDMethod");

  auto            pyPlugin_minimalPlugin = std::make_shared<PythonPlugin>("Test_MinimalPlugin");
  PluginManager   pm;
  CaptureInstance ci;
  pm.addPlugin(pyPlugin_minimalPlugin);


  REQUIRE(pyPlugin_noFile->initialize(NULL) == FALSE);
  REQUIRE(pyPlugin_emptyFile->initialize(NULL) == FALSE);
  REQUIRE(pyPlugin_classNotAvailable->initialize(NULL) == FALSE);
  REQUIRE(pyPlugin_classIllegalParent->initialize(NULL) == FALSE);
  REQUIRE(pyPlugin_noIDMethod->initialize(NULL) == FALSE);
  REQUIRE(pyPlugin_illegalIDMethod->initialize(NULL) == FALSE);
  REQUIRE(pm.init(&ci) == TRUE);
  std::cout << std::endl;
}

TEST_CASE("Testing calling cython", "[python]") {
  std::cout << std::endl;

  PluginManager   pm;
  CaptureInstance ci;

  auto pyPlugin = std::make_shared<PythonPlugin>("PluginA");
  pm.addPlugin(pyPlugin);
  REQUIRE(pm.init(&ci) == TRUE);

  Cycle cy = *ci.getStartCycle();

  pm.processCycle(&cy);
  std::cout << std::endl;
}

TEST_CASE("Test loading with plugin", "[python]") {
  CaptureInstance inst;
  auto            id       = inst.getEventLog()->getAppID();
  auto            pyPlugin = std::make_shared<PythonPlugin>("SimplePlugin");
  inst.getPluginManager()->addPlugin(pyPlugin);

  std::string file = constants::EPL_DC_BUILD_DIR_ROOT + "/external/resources/pcaps/1CN-with-ObjectMapping-PDO.pcapng";
  fs::path    filePath(file);
  REQUIRE(fs::exists(filePath));
  REQUIRE(fs::is_regular_file(filePath));

  REQUIRE(inst.loadPCAP(file) == 0);

  inst.getCycleBuilder()->waitForLoopToFinish();
  auto events = inst.getEventLog()->pollEvents(id);
  std::cout << "EVT SIZE:" << events.size() << std::endl;
}

TEST_CASE("GUI API calls exec", "[python]") {
  CaptureInstance inst;
  auto            id       = inst.getEventLog()->getAppID();
  auto            pyPluginValidCalls = std::make_shared<PythonPlugin>("Test_VALIDGUIAPICalls");
  auto            pyPluginInvalidCalls = std::make_shared<PythonPlugin>("Test_INVALIDGUIAPICalls");
  inst.getPluginManager()->addPlugin(pyPluginValidCalls);
  inst.getPluginManager()->addPlugin(pyPluginInvalidCalls);

  std::string file = constants::EPL_DC_BUILD_DIR_ROOT + "/external/resources/pcaps/1CN-with-ObjectMapping-PDO.pcapng";
  fs::path    filePath(file);
  REQUIRE(fs::exists(filePath));
  REQUIRE(fs::is_regular_file(filePath));

  REQUIRE(inst.loadPCAP(file) == 0);

  inst.getCycleBuilder()->waitForLoopToFinish();
  auto events = inst.getEventLog()->pollEvents(id);

  int evTypeStartCap = 0;
  int evTypeEndCap = 0;
  int evTypeMN = 0;
  int evTypeCN = 0;
  int evTypeOD = 0;

  for(auto ev : events) {
    switch (ev->getType()) {
      case EvType::VIEW_STARTCAP:
        evTypeStartCap++;
        break;
      case EvType::VIEW_ENDCAP:
        evTypeEndCap++;
        break;
      case EvType::VIEW_EV_HIGHLIGHT_MN:
        evTypeMN++;
        break;
      case EvType::VIEW_EV_HIGHLIGHT_CN:
        evTypeCN++;
        if(ev->getEventFlags() != 1) {
          FAIL("invalid flag in cn higlight event");
        }
        break;
      case EvType::VIEW_EV_HIGHLIGHT_OD_ENTRY:
        evTypeOD++;
        if(ev->getEventFlags() != 0x1000 || ev->getDescription().compare(std::string("10"))) {
          FAIL("illegal event flags / description in highlight od entry event");
        }
        break;
      default:
        break;
    }
  }

  auto *firstEv = events.at(0);

  REQUIRE(firstEv->getType() == EvType::VIEW_EV_HIGHLIGHT_MN);
  REQUIRE(events.size() == 577);
  REQUIRE(evTypeStartCap == 144);
  REQUIRE(evTypeEndCap == 144);
  REQUIRE(evTypeCN == 144);
  REQUIRE(evTypeOD == 144);
  REQUIRE(evTypeMN == 1); // added only once cause of cycle range
}
