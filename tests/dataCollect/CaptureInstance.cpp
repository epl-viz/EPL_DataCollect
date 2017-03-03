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
#include <PluginBase.hpp>
#include <catch.hpp>
#include <fakeit.hpp>

using namespace EPL_DataCollect;
using namespace fakeit;

class CSTest1 : public CycleStorageBase {
 public:
  virtual bool                              isNumericValue() override;
  virtual double                            getNumericValue() override;
  virtual std::unique_ptr<CycleStorageBase> clone() override;
};

bool                              CSTest1::isNumericValue() { return false; }
double                            CSTest1::getNumericValue() { return 0; }
std::unique_ptr<CycleStorageBase> CSTest1::clone() { return std::make_unique<CSTest1>(*this); }

TEST_CASE("Testing CaptureInstance", "[CaptureInstance]") {
  CaptureInstance ci;
  PluginManager & pm = *ci.getPluginManager();

  Mock<PluginBase> pbMock[16];
  for (uint32_t i = 0; i < 16; i++) {
    When(Method(pbMock[i], run)).AlwaysReturn();
    When(Method(pbMock[i], initialize)).AlwaysReturn(true);
    When(Method(pbMock[i], reset)).AlwaysReturn(true);
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

  SECTION("Adding plugins -- live capture") {
    pm.addPlugin(std::shared_ptr<PluginBase>(&pbMock[0x0].get(), [](PluginBase *) {}));
    pm.addPlugin(std::shared_ptr<PluginBase>(&pbMock[0x1].get(), [](PluginBase *) {}));
    pm.addPlugin(std::shared_ptr<PluginBase>(&pbMock[0x2].get(), [](PluginBase *) {}));
    pm.addPlugin(std::shared_ptr<PluginBase>(&pbMock[0x3].get(), [](PluginBase *) {}));
    pm.addPlugin(std::shared_ptr<PluginBase>(&pbMock[0x4].get(), [](PluginBase *) {}));
    pm.addPlugin(std::shared_ptr<PluginBase>(&pbMock[0x5].get(), [](PluginBase *) {}));
    pm.addPlugin(std::shared_ptr<PluginBase>(&pbMock[0x6].get(), [](PluginBase *) {}));

#if 0
    REQUIRE(ci.getState() == CaptureInstance::SETUP);
    REQUIRE(ci.startRecording("") == 0);
    REQUIRE(ci.startRecording("") == -1);
    REQUIRE(ci.getState() == CaptureInstance::RUNNING);
    REQUIRE(ci.stopRecording() == 0);
    REQUIRE(ci.stopRecording() == -1);
    REQUIRE(ci.getState() == CaptureInstance::DONE);
#else
    REQUIRE(ci.startRecording("") == -2);
    REQUIRE(ci.getState() == CaptureInstance::ERRORED);
#endif
  }

  SECTION("Adding plugins -- PCAP") {
    pm.addPlugin(std::shared_ptr<PluginBase>(&pbMock[0x0].get(), [](PluginBase *) {}));
    pm.addPlugin(std::shared_ptr<PluginBase>(&pbMock[0x1].get(), [](PluginBase *) {}));
    pm.addPlugin(std::shared_ptr<PluginBase>(&pbMock[0x2].get(), [](PluginBase *) {}));
    pm.addPlugin(std::shared_ptr<PluginBase>(&pbMock[0x3].get(), [](PluginBase *) {}));
    pm.addPlugin(std::shared_ptr<PluginBase>(&pbMock[0x4].get(), [](PluginBase *) {}));
    pm.addPlugin(std::shared_ptr<PluginBase>(&pbMock[0x5].get(), [](PluginBase *) {}));
    pm.addPlugin(std::shared_ptr<PluginBase>(&pbMock[0x6].get(), [](PluginBase *) {}));

    REQUIRE(ci.getState() == CaptureInstance::SETUP);
    REQUIRE(ci.loadPCAP("") == 10);
    REQUIRE(ci.loadPCAP(constants::EPL_DC_BUILD_DIR_ROOT) == 11);
    REQUIRE(ci.loadPCAP(constants::EPL_DC_BUILD_DIR_ROOT + "/external/resources/pcaps/EPL_Example.cap") == 0);
    REQUIRE(ci.loadPCAP("") == -1);
    REQUIRE(ci.getState() == CaptureInstance::RUNNING);
    REQUIRE(ci.stopRecording() == 0);
    REQUIRE(ci.stopRecording() == -1);
    REQUIRE(ci.getState() == CaptureInstance::DONE);
  }

  SECTION("Missing dependency") {
    pm.addPlugin(std::shared_ptr<PluginBase>(&pbMock[0x0].get(), [](PluginBase *) {}));
    pm.addPlugin(std::shared_ptr<PluginBase>(&pbMock[0x7].get(), [](PluginBase *) {}));
    pm.addPlugin(std::shared_ptr<PluginBase>(&pbMock[0x8].get(), [](PluginBase *) {}));
    pm.addPlugin(std::shared_ptr<PluginBase>(&pbMock[0x9].get(), [](PluginBase *) {}));
    pm.addPlugin(std::shared_ptr<PluginBase>(&pbMock[0xA].get(), [](PluginBase *) {}));
    pm.addPlugin(std::shared_ptr<PluginBase>(&pbMock[0xB].get(), [](PluginBase *) {}));
    pm.addPlugin(std::shared_ptr<PluginBase>(&pbMock[0xC].get(), [](PluginBase *) {}));

    REQUIRE(ci.getState() == CaptureInstance::SETUP);
    REQUIRE(ci.startRecording("") == 1);
    REQUIRE(ci.startRecording("") == -1);
    REQUIRE(ci.getState() == CaptureInstance::ERRORED);
  }

  SECTION("Testing adding CycleStorage") {
    REQUIRE(ci.registerCycleStorage<CSTest1>("A") == true);
    REQUIRE(ci.registerCycleStorage<CSTest1>("B") == true);
    REQUIRE(ci.registerCycleStorage<CSTest1>("A") == false);
    REQUIRE(ci.loadPCAP(constants::EPL_DC_BUILD_DIR_ROOT + "/external/resources/pcaps/EPL_Example.cap") == 0);
    REQUIRE(ci.stopRecording() == 0);
    REQUIRE(ci.registerCycleStorage<CSTest1>("C") == false);
  }

  SECTION("Testing the stubs") {
    REQUIRE(ci.getEventLog() != nullptr);
    REQUIRE(ci.getPluginManager() != nullptr);
    REQUIRE(ci.getCycleContainer() != nullptr);
    REQUIRE(ci.getDevices() == std::vector<std::string>());
    ci.loadXDD("");
  }
}

TEST_CASE("Testing CaptureInstance -- plugin init error", "[CaptureInstance]") {
  CaptureInstance ci;
  PluginManager & pm = *ci.getPluginManager();

  Mock<PluginBase> pbMock[4];
  for (uint32_t i = 0; i < 4; i++) {
    When(Method(pbMock[i], run)).AlwaysReturn();
    When(Method(pbMock[i], initialize)).AlwaysReturn(false);
    When(Method(pbMock[i], reset)).AlwaysReturn(true);
  }

  When(Method(pbMock[0x0], getID)).AlwaysReturn("P0");
  When(Method(pbMock[0x1], getID)).AlwaysReturn("P1");
  When(Method(pbMock[0x2], getID)).AlwaysReturn("P2");
  When(Method(pbMock[0x3], getID)).AlwaysReturn("P3");

  When(Method(pbMock[0x0], getDependencies)).AlwaysReturn("");
  When(Method(pbMock[0x1], getDependencies)).AlwaysReturn("");
  When(Method(pbMock[0x2], getDependencies)).AlwaysReturn("P3");
  When(Method(pbMock[0x3], getDependencies)).AlwaysReturn("");

  pm.addPlugin(std::shared_ptr<PluginBase>(&pbMock[0x0].get(), [](PluginBase *) {}));
  pm.addPlugin(std::shared_ptr<PluginBase>(&pbMock[0x1].get(), [](PluginBase *) {}));
  pm.addPlugin(std::shared_ptr<PluginBase>(&pbMock[0x2].get(), [](PluginBase *) {}));
  pm.addPlugin(std::shared_ptr<PluginBase>(&pbMock[0x3].get(), [](PluginBase *) {}));

  REQUIRE(ci.getState() == CaptureInstance::SETUP);
  REQUIRE(ci.startRecording("") == 1);
  REQUIRE(ci.getState() == CaptureInstance::ERRORED);
}

TEST_CASE("Testing CaptureInstance -- plugin reset error", "[CaptureInstance]") {
  CaptureInstance ci;
  PluginManager & pm = *ci.getPluginManager();

  Mock<PluginBase> pbMock[4];
  for (uint32_t i = 0; i < 4; i++) {
    When(Method(pbMock[i], run)).AlwaysReturn();
    When(Method(pbMock[i], initialize)).AlwaysReturn(true);
    When(Method(pbMock[i], reset)).AlwaysReturn(false);
  }

  When(Method(pbMock[0x0], getID)).AlwaysReturn("P0");
  When(Method(pbMock[0x1], getID)).AlwaysReturn("P1");
  When(Method(pbMock[0x2], getID)).AlwaysReturn("P2");
  When(Method(pbMock[0x3], getID)).AlwaysReturn("P3");

  When(Method(pbMock[0x0], getDependencies)).AlwaysReturn("");
  When(Method(pbMock[0x1], getDependencies)).AlwaysReturn("");
  When(Method(pbMock[0x2], getDependencies)).AlwaysReturn("P3");
  When(Method(pbMock[0x3], getDependencies)).AlwaysReturn("");

  pm.addPlugin(std::shared_ptr<PluginBase>(&pbMock[0x0].get(), [](PluginBase *) {}));
  pm.addPlugin(std::shared_ptr<PluginBase>(&pbMock[0x1].get(), [](PluginBase *) {}));
  pm.addPlugin(std::shared_ptr<PluginBase>(&pbMock[0x2].get(), [](PluginBase *) {}));
  pm.addPlugin(std::shared_ptr<PluginBase>(&pbMock[0x3].get(), [](PluginBase *) {}));

  REQUIRE(ci.getState() == CaptureInstance::SETUP);
  REQUIRE(ci.loadPCAP(constants::EPL_DC_BUILD_DIR_ROOT + "/external/resources/pcaps/EPL_Example.cap") == 0);
  REQUIRE(ci.getState() == CaptureInstance::RUNNING);
  REQUIRE(ci.stopRecording() == 1);
  REQUIRE(ci.getState() == CaptureInstance::ERRORED);
}

TEST_CASE("Testing CaptureInstance -- config", "[CaptureInstance]") {
  CaptureInstance ci;

  auto cfg                  = ci.getConfig();
  cfg.ihConfig.eplFrameName = "TEST";
  cfg.smConfig.saveInterval = 0xDEADBEAF;
  ci.setConfig(cfg);

  auto ihConfig = ci.getInputHandler()->getConfig();
  auto smConfig = ci.getSnapshotManager()->getConfig();

  REQUIRE(ihConfig.eplFrameName == "TEST");
  REQUIRE(smConfig.saveInterval == 0xDEADBEAF);

  auto def        = ci.getDefaultNodeConfig();
  def.baseProfile = "1001";
  ci.setDefaultNodeConfig(def);

  auto prof1        = def;
  prof1.baseProfile = "000";

  ci.setNodeConfig(0, prof1);
  auto prof2 = ci.getNodeConfig(0);
  auto prof3 = ci.getNodeConfig(4);

  REQUIRE(prof2.baseProfile == prof1.baseProfile);
  REQUIRE(prof3.baseProfile == def.baseProfile);
}
