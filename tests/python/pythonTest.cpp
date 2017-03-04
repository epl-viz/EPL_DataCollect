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
#include <PythonPlugin.hpp>
#include <catch.hpp>
#include <fakeit.hpp>
#include <iostream>

#pragma clang diagnostic pop

using namespace fakeit;
using namespace EPL_DataCollect;
using namespace EPL_DataCollect::plugins;

TEST_CASE("Loading python files", "[python]") {
  std::cout << "\n";
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
  std::cout << "\n";
}

TEST_CASE("Testing calling cython", "[python]") {

  std::cout << "\n\n";

  PluginManager   pm;
  CaptureInstance ci;
  Cycle           cy;


  auto pyPlugin = std::make_shared<PythonPlugin>("PluginA");
  pm.addPlugin(pyPlugin);
  REQUIRE(pm.init(&ci) == TRUE);
  pm.processCycle(&cy);

  //   bool ret = pyPlugin->initialize(NULL);
  //
  //   if (ret) {
  //     std::cout << "plugin.getID returns \t" << pyPlugin->getID() << "\n";
  //     std::cout << "plugin.getDependencies returns \t" << pyPlugin->getDependencies() << "\n";
  //     std::cout << "plugin.run()..."
  //               << "\n";
  //     pyPlugin->run(new EPL_DataCollect::Cycle());
  //
  //     std::cout << "\n----------------\n\n\t5 runs of Plugin A:";
  //
  //     EPL_DataCollect::Cycle *curCyc = new EPL_DataCollect::Cycle();
  //     for (int i = 0; i < 5; i++) {
  //       pyPlugin->run(curCyc);
  //     }
  //   } else {
  //     std::cout << "INIT FAILED !";
  //   }
  std::cout << "\n\n";
}
