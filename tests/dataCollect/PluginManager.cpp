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

#include <Cycle.hpp>
#include <PluginManager.hpp>
#include <catch.hpp>
#include <fakeit.hpp>

using namespace EPL_DataCollect;
using namespace fakeit;

TEST_CASE("Testing PluginManager", "[plugins]") {
  PluginManager pm;

  Mock<Cycle> mock;
  Cycle &     c = mock.get();

  Mock<PluginBase> pbMock[16];
  for (uint32_t i = 0; i < 16; i++) {
    When(Method(pbMock[i], run)).AlwaysReturn();
  }

  When(Method(pbMock[0x0], getID)).AlwaysReturn("P0");
  When(Method(pbMock[0x1], getID)).AlwaysReturn("P1");
  When(Method(pbMock[0x2], getID)).AlwaysReturn("P2");
  When(Method(pbMock[0x3], getID)).AlwaysReturn("P3");
  When(Method(pbMock[0x4], getID)).AlwaysReturn("P4");
  When(Method(pbMock[0x5], getID)).AlwaysReturn("P5");
  When(Method(pbMock[0x6], getID)).AlwaysReturn("P6");
  When(Method(pbMock[0x7], getID)).AlwaysReturn("P7");
  When(Method(pbMock[0x8], getID)).AlwaysReturn("P8");
  When(Method(pbMock[0x9], getID)).AlwaysReturn("P9");
  When(Method(pbMock[0xA], getID)).AlwaysReturn("PA");
  When(Method(pbMock[0xB], getID)).AlwaysReturn("PB");
  When(Method(pbMock[0xC], getID)).AlwaysReturn("PC");
  When(Method(pbMock[0xD], getID)).AlwaysReturn("PD");
  When(Method(pbMock[0xE], getID)).AlwaysReturn("PE");
  When(Method(pbMock[0xF], getID)).AlwaysReturn("PF");

  When(Method(pbMock[0x0], getDependencies)).AlwaysReturn("");
  When(Method(pbMock[0x1], getDependencies)).AlwaysReturn("");
  When(Method(pbMock[0x2], getDependencies)).AlwaysReturn("P3");
  When(Method(pbMock[0x3], getDependencies)).AlwaysReturn("");
  When(Method(pbMock[0x4], getDependencies)).AlwaysReturn("");
  When(Method(pbMock[0x5], getDependencies)).AlwaysReturn("");
  When(Method(pbMock[0x6], getDependencies)).AlwaysReturn("P0;P1;P2");
  When(Method(pbMock[0x7], getDependencies)).AlwaysReturn("P0");
  When(Method(pbMock[0x8], getDependencies)).AlwaysReturn("P7");
  When(Method(pbMock[0x9], getDependencies)).AlwaysReturn("P8");
  When(Method(pbMock[0xA], getDependencies)).AlwaysReturn("P9");
  When(Method(pbMock[0xB], getDependencies)).AlwaysReturn("PA");
  When(Method(pbMock[0xC], getDependencies)).AlwaysReturn("NOPE");
  When(Method(pbMock[0xD], getDependencies)).AlwaysReturn("PE"); // Circle
  When(Method(pbMock[0xE], getDependencies)).AlwaysReturn("PF"); // Circle
  When(Method(pbMock[0xF], getDependencies)).AlwaysReturn("PD"); // Circle

  SECTION("Add duplicate") {
    auto tmp = std::shared_ptr<PluginBase>(&pbMock[0x0].get(), [](PluginBase *) {});
    REQUIRE(pm.addPlugin(tmp) == true);
    REQUIRE(pm.addPlugin(tmp) == false);
  }

  SECTION("Adding plugins and init") {
    pm.addPlugin(std::shared_ptr<PluginBase>(&pbMock[0x0].get(), [](PluginBase *) {}));
    pm.addPlugin(std::shared_ptr<PluginBase>(&pbMock[0x1].get(), [](PluginBase *) {}));
    pm.addPlugin(std::shared_ptr<PluginBase>(&pbMock[0x2].get(), [](PluginBase *) {}));
    pm.addPlugin(std::shared_ptr<PluginBase>(&pbMock[0x3].get(), [](PluginBase *) {}));
    pm.addPlugin(std::shared_ptr<PluginBase>(&pbMock[0x4].get(), [](PluginBase *) {}));
    pm.addPlugin(std::shared_ptr<PluginBase>(&pbMock[0x5].get(), [](PluginBase *) {}));
    pm.addPlugin(std::shared_ptr<PluginBase>(&pbMock[0x6].get(), [](PluginBase *) {}));

    REQUIRE(pm.init() == true);
  }

  SECTION("Missing dependency") {
    pm.addPlugin(std::shared_ptr<PluginBase>(&pbMock[0x0].get(), [](PluginBase *) {}));
    pm.addPlugin(std::shared_ptr<PluginBase>(&pbMock[0x7].get(), [](PluginBase *) {}));
    pm.addPlugin(std::shared_ptr<PluginBase>(&pbMock[0x8].get(), [](PluginBase *) {}));
    pm.addPlugin(std::shared_ptr<PluginBase>(&pbMock[0x9].get(), [](PluginBase *) {}));
    pm.addPlugin(std::shared_ptr<PluginBase>(&pbMock[0xA].get(), [](PluginBase *) {}));
    pm.addPlugin(std::shared_ptr<PluginBase>(&pbMock[0xB].get(), [](PluginBase *) {}));
    pm.addPlugin(std::shared_ptr<PluginBase>(&pbMock[0xC].get(), [](PluginBase *) {}));

    REQUIRE(pm.init() == false);
  }

  SECTION("Dependency cycle") {
    pm.addPlugin(std::shared_ptr<PluginBase>(&pbMock[0xD].get(), [](PluginBase *) {}));
    pm.addPlugin(std::shared_ptr<PluginBase>(&pbMock[0xE].get(), [](PluginBase *) {}));
    pm.addPlugin(std::shared_ptr<PluginBase>(&pbMock[0xF].get(), [](PluginBase *) {}));

    REQUIRE(pm.init() == false);
  }

  SECTION("Access after init") {
    pm.addPlugin(std::shared_ptr<PluginBase>(&pbMock[0x0].get(), [](PluginBase *) {}));
    pm.addPlugin(std::shared_ptr<PluginBase>(&pbMock[0x1].get(), [](PluginBase *) {}));
    pm.addPlugin(std::shared_ptr<PluginBase>(&pbMock[0x2].get(), [](PluginBase *) {}));
    pm.addPlugin(std::shared_ptr<PluginBase>(&pbMock[0x3].get(), [](PluginBase *) {}));
    pm.addPlugin(std::shared_ptr<PluginBase>(&pbMock[0x4].get(), [](PluginBase *) {}));
    pm.addPlugin(std::shared_ptr<PluginBase>(&pbMock[0x5].get(), [](PluginBase *) {}));
    pm.addPlugin(std::shared_ptr<PluginBase>(&pbMock[0x6].get(), [](PluginBase *) {}));

    REQUIRE(pm.canEditPlugins() == true);
    REQUIRE(pm.removePlugin("P5") == true);
    REQUIRE(pm.init() == true);
    auto tmp = std::shared_ptr<PluginBase>(&pbMock[0xF].get(), [](PluginBase *) {});
    REQUIRE(pm.addPlugin(tmp) == false);
    REQUIRE(pm.removePlugin("P4") == false);
    REQUIRE(pm.canEditPlugins() == false);
  }

  SECTION("Get List and remove plugins") {
    pm.addPlugin(std::shared_ptr<PluginBase>(&pbMock[0x0].get(), [](PluginBase *) {}));
    pm.addPlugin(std::shared_ptr<PluginBase>(&pbMock[0x1].get(), [](PluginBase *) {}));
    pm.addPlugin(std::shared_ptr<PluginBase>(&pbMock[0x2].get(), [](PluginBase *) {}));
    pm.addPlugin(std::shared_ptr<PluginBase>(&pbMock[0x3].get(), [](PluginBase *) {}));
    pm.addPlugin(std::shared_ptr<PluginBase>(&pbMock[0x4].get(), [](PluginBase *) {}));
    pm.addPlugin(std::shared_ptr<PluginBase>(&pbMock[0x5].get(), [](PluginBase *) {}));
    pm.addPlugin(std::shared_ptr<PluginBase>(&pbMock[0x6].get(), [](PluginBase *) {}));

    std::vector<std::string> toFind = {"P0", "P1", "P2", "P3", "P4", "P5", "P6"};
    auto                     plList = pm.getPluginList();

    for (auto i : toFind) {
      bool found = false;
      for (auto j : plList) {
        if (i == j) {
          found = true;
          break;
        }
      }
      REQUIRE(found == true);
    }

    REQUIRE(pm.canEditPlugins() == true);
    REQUIRE(pm.removePlugin("P5") == true);

    toFind = {"P0", "P1", "P2", "P3", "P4", /*"P5",*/ "P6"};
    plList = pm.getPluginList();

    for (auto i : toFind) {
      bool found = false;
      for (auto j : plList) {
        if (i == j) {
          found = true;
          break;
        }
      }
      REQUIRE(found == true);
    }

    REQUIRE(pm.init() == true);
    REQUIRE(pm.init() == false);

    REQUIRE(pm.removePlugin("PF") == false);
    REQUIRE(pm.canEditPlugins() == false);

    REQUIRE(pm.reset() == true);
    REQUIRE(pm.reset() == false);

    REQUIRE(pm.canEditPlugins() == true);
    REQUIRE(pm.removePlugin("PF") == false); // PF does not exist
    REQUIRE(pm.removePlugin("P0") == true);

    toFind = {/*"P0",*/ "P1", "P2", "P3", "P4", /*"P5",*/ "P6"};
    plList = pm.getPluginList();

    for (auto i : toFind) {
      bool found = false;
      for (auto j : plList) {
        if (i == j) {
          found = true;
          break;
        }
      }
      REQUIRE(found == true);
    }
  }

  SECTION("Run Plugins") {
    pm.addPlugin(std::shared_ptr<PluginBase>(&pbMock[0x0].get(), [](PluginBase *) {}));
    pm.addPlugin(std::shared_ptr<PluginBase>(&pbMock[0x1].get(), [](PluginBase *) {}));
    pm.addPlugin(std::shared_ptr<PluginBase>(&pbMock[0x2].get(), [](PluginBase *) {}));
    pm.addPlugin(std::shared_ptr<PluginBase>(&pbMock[0x3].get(), [](PluginBase *) {}));
    pm.addPlugin(std::shared_ptr<PluginBase>(&pbMock[0x4].get(), [](PluginBase *) {}));
    pm.addPlugin(std::shared_ptr<PluginBase>(&pbMock[0x5].get(), [](PluginBase *) {}));
    pm.addPlugin(std::shared_ptr<PluginBase>(&pbMock[0x6].get(), [](PluginBase *) {}));

    REQUIRE(pm.processCycle(&c) == 1);
    REQUIRE(pm.init() == true);
    REQUIRE(pm.processCycle(&c) == 0);
  }
}
