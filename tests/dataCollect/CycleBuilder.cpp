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

#include <CaptureInstance.hpp>
#include <CycleBuilder.hpp>
#include <InputHandler.hpp>
#include <catch.hpp>
#include <fakeit.hpp>

#if __cplusplus <= 201402L
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#else
#include <filesystem>
namespace fs = std::filesystem;
#endif

using namespace EPL_DataCollect;
using namespace fakeit;
using namespace constants;

TEST_CASE("Testing load of a 100mb file", "[CycleBuilder][bigFile]") {
  CaptureInstance inst;

  auto instCfg                  = inst.getConfig();
  auto cfg                      = inst.getDefaultNodeConfig();
  cfg.autoDeduceSpecificProfile = false;

  instCfg.xddDir = EPL_DC_BUILD_DIR_ROOT + "/external/resources/profiles/printer";

  cfg.baseProfile = "steppercn4cn_1.xdc";
  inst.setNodeConfig(1, cfg);

  cfg.baseProfile = "steppercn4cn_2.xdc";
  inst.setNodeConfig(2, cfg);

  cfg.baseProfile = "steppercn4cn_3.xdc";
  inst.setNodeConfig(3, cfg);

  cfg.baseProfile = "steppercn4cn_4.xdc";
  inst.setNodeConfig(4, cfg);

  cfg.baseProfile = "00000000_POWERLINK_CiA302-4_MN.xdc";
  inst.setNodeConfig(240, cfg);
  inst.setConfig(instCfg);

  std::string file = constants::EPL_DC_BUILD_DIR_ROOT + "/external/resources/pcaps/170119_wall_complete.pcapng";
  fs::path    filePath(file);
  REQUIRE(fs::exists(filePath));
  REQUIRE(fs::is_regular_file(filePath));

  REQUIRE(inst.loadPCAP(file) == 0);
  REQUIRE(inst.getCycleBuilder()->isRunning() == true);

  inst.getCycleBuilder()->waitForLoopToFinish();
}

TEST_CASE("Testing loading withCollisions", "[CycleBuilder][CN0]") {
  CaptureInstance inst;

  std::string file =
        constants::EPL_DC_BUILD_DIR_ROOT + "/external/resources/pcaps/1CN-SomeCollisions-ThenMapping.pcapng";
  fs::path filePath(file);
  REQUIRE(fs::exists(filePath));
  REQUIRE(fs::is_regular_file(filePath));

  REQUIRE(inst.loadPCAP(file) == 0);

  inst.getCycleBuilder()->waitForLoopToFinish();
}

TEST_CASE("Testing loading a EPL_Example", "[CycleBuilder]") {
  CaptureInstance inst;

  std::string file = constants::EPL_DC_BUILD_DIR_ROOT + "/external/resources/pcaps/EPL_Example.cap";
  fs::path    filePath(file);
  REQUIRE(fs::exists(filePath));
  REQUIRE(fs::is_regular_file(filePath));

  REQUIRE(inst.loadPCAP(file) == 0);

  inst.getCycleBuilder()->waitForLoopToFinish();
}

TEST_CASE("Testing loading 1CN", "[CycleBuilder]") {
  CaptureInstance inst;

  std::string file = constants::EPL_DC_BUILD_DIR_ROOT + "/external/resources/pcaps/1CN-with-ObjectMapping-PDO.pcapng";
  fs::path    filePath(file);
  REQUIRE(fs::exists(filePath));
  REQUIRE(fs::is_regular_file(filePath));

  REQUIRE(inst.loadPCAP(file) == 0);

  {
    SLEEP(milliseconds, 2000);
    auto ptr = inst.getCycleContainer()->pollCyclePTR();
    REQUIRE(*ptr != nullptr);
    std::cout << "Random polled Cycle: " << ptr->getCycleNum() << std::endl;
  }

  inst.getCycleBuilder()->waitForLoopToFinish();
}
