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
#include <catch.hpp>
#include <fakeit.hpp>

using namespace EPL_DataCollect;
using namespace fakeit;


/*!
  * class TestStorage
  * \brief Stub for testing the CycleStorage system
  *
  * Implements CycleStorageBase in order to provide a target for the test case of the CycleStorage testcase.
  *
  */
class TestStorage : public CycleStorageBase {
 public:
  TestStorage() = default;
  virtual ~TestStorage();

  TestStorage(const TestStorage &) = default;
  TestStorage(TestStorage &&)      = default;

  TestStorage &operator=(const TestStorage &) = default;
  TestStorage &operator=(TestStorage &&) = default;

  virtual bool                              isNumericValue() override;
  virtual double                            getNumericValue() override;
  virtual std::unique_ptr<CycleStorageBase> clone() override;
};

bool                              TestStorage::isNumericValue() { return false; }
double                            TestStorage::getNumericValue() { return 0; }
std::unique_ptr<CycleStorageBase> TestStorage::clone() { return std::make_unique<TestStorage>(*this); }


TestStorage::~TestStorage() {}

TEST_CASE("Standard return values work", "[Cycle]") {
  Cycle c;

  SECTION("Test getNumNodes()") { REQUIRE(c.getNumNodes() == 0); }
  SECTION("Test getCycleNum()") { REQUIRE(c.getCycleNum() == 0); }
  SECTION("Test getPackets()") { REQUIRE(c.getPackets().empty() == true); }
  SECTION("Test getActiveEvents()") { REQUIRE(c.getActiveEvents().empty() == true); }
}

TEST_CASE("Fetching non existant values fails", "[Cycle]") {
  Cycle c;

  SECTION("Test getNode() throwing an exception") { REQUIRE_THROWS(c.getNode(200)); }
  SECTION("Test getCycleStorage() returning a nullptr") {
    REQUIRE(c.getCycleStorage("Non_Existant_Plugin") == nullptr);
  }
}

TEST_CASE("Packets are updated correctly") {
  Cycle               c;
  std::vector<Packet> newPackets;
  uint32_t            oldCycleNum = c.getCycleNum();

  // Create sample packets
  newPackets.emplace_back(PT_UNDEF, CMD_ID_NIL, nullptr, "", "", 1, 10, std::chrono::system_clock::now());
  newPackets.emplace_back(PT_UNDEF, CMD_ID_NIL, nullptr, "", "", 10, 1, std::chrono::system_clock::now());
  newPackets.emplace_back(PT_UNDEF, CMD_ID_NIL, nullptr, "", "", 100, 50, std::chrono::system_clock::now());
  newPackets.emplace_back(PT_UNDEF, CMD_ID_NIL, nullptr, "", "", 30, 80, std::chrono::system_clock::now());
  newPackets.emplace_back(PT_UNDEF, CMD_ID_NIL, nullptr, "", "", 90, 10, std::chrono::system_clock::now());


  SECTION("Updating Packets") {
    c.updatePackets(newPackets);

    REQUIRE(c.getPackets() == newPackets);
    REQUIRE(c.getCycleNum() == oldCycleNum + 1);
  }
}

TEST_CASE("Cycle Storage system works") {
  Cycle c;

  SECTION("Test registering a new CycleStorage") {
    REQUIRE(c.registerCycleStorage("TestPlugin", std::make_unique<TestStorage>()) == true);
  }
  SECTION("Test registering a duplicate CycleStorage") {
    REQUIRE(c.registerCycleStorage("TestPlugin", std::make_unique<TestStorage>()) == true);
    REQUIRE(c.registerCycleStorage("TestPlugin", std::make_unique<TestStorage>()) == false);
  }
  SECTION("Test retrieving a registered CycleStorage") {
    auto              ptr = std::make_unique<TestStorage>();
    CycleStorageBase *p   = ptr.get();

    REQUIRE(c.registerCycleStorage("TestPlugin", std::move(ptr)) == true);
    REQUIRE(c.getCycleStorage("TestPlugin") == p);
  }
}
