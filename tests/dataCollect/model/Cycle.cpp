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

#include "InputParser.hpp"
#include <Cycle.hpp>
#include <catch.hpp>
#include <cmath>
#include <fakeit.hpp>
#include <limits>

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

  REQUIRE(c.getNumNodes() == 0);
  REQUIRE(c.getCycleNum() == UINT32_MAX);
  REQUIRE(c.getPackets().empty() == true);
  REQUIRE(c.getActiveEvents().empty() == true);
  REQUIRE(c.getAllCycleStorage()->size() == 0);
  REQUIRE(c == c);
}

TEST_CASE("Fetching non existant values fails", "[Cycle]") {
  Cycle c;

  REQUIRE(c.getNode(200) == nullptr);
  REQUIRE(c.getCycleStorage("Non_Existant_Plugin") == nullptr);
  REQUIRE(c.getODEntry(201, 0) == nullptr);
  REQUIRE(c.addNode(201) == true);
  REQUIRE(c.addNode(201) == false);
  REQUIRE(c.getODEntry(201, 1) == nullptr);
}

TEST_CASE("OD Entries are fetched correctly") {
  Cycle c;
  REQUIRE(c.getODEntry(200, 0) == nullptr);
  c.addNode(200);
  REQUIRE(c.getODEntry(200, 0) == nullptr);
  REQUIRE(c.getNodeList().size() == 1);

  Node *         n      = c.getNode(200);
  OD *           od     = n->getOD();
  ODDescription *odDesc = od->getODDesc();

  ODEntryDescription entryDesc(ObjectType::VAR, ObjectDataType::INTEGER16);

  odDesc->setEntry(0, entryDesc);

  ODEntry *entry = c.getODEntry(200, 0);

  SECTION("Test getODEntry() correctly retrieving valid values") {
    REQUIRE(entry != nullptr);
    REQUIRE(entry->getType() == ObjectClassType::INTEGER);
    REQUIRE(entry->getDataType() == ObjectDataType::INTEGER16);
    REQUIRE(entry->isNumericValue() == true);
    REQUIRE(std::fabs(entry->getNumericValue() - entryDesc.defaultValue->getNumericValue()) <
            std::numeric_limits<double>::epsilon());
  }
}

TEST_CASE("Packets are updated correctly") {
  Cycle               c;
  std::vector<Packet> newPackets;
  uint32_t            oldCycleNum = c.getCycleNum();

  WiresharkParser::parserData d;

  // Create sample packets
  newPackets.emplace_back(&d, 0);
  newPackets.emplace_back(&d, 10);
  newPackets.emplace_back(&d, 20);
  newPackets.emplace_back(&d, 30);

  SECTION("Updating Packets") {
    c.updatePackets(newPackets);

    REQUIRE(c.getPackets() == newPackets);
    REQUIRE(c.getCycleNum() == oldCycleNum);
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
