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
#include <EvDebug.hpp>
#include <EventBase.hpp>
#include <EventLog.hpp>
#include <catch.hpp>
#include <fakeit.hpp>
#include <iostream>

using namespace EPL_DataCollect;
using namespace fakeit;

TEST_CASE("Test the event log class", "[Events][EventLog]") {
  EventLog             evLog;
  EventBase::INDEX_MAP map;
  map[1]    = "asdf";
  map[1000] = "dfsfgh";

  Mock<Cycle> mock;

  std::string strings[] = {"TestPlugin",
                           "ASDG",
                           "Something",
                           "Something rather long",
                           "Hello World",
                           "StrStrStrStr",
                           "\x1b[31;1mThis should be bold red\x1b[0m",
                           "This String has a\n newline",
                           "This string \0has \0null bytes"};

  SECTION("Test merging events") {
    When(Method(mock, getCycleNum)).Return(1, 1, 10, 10, 10, 11, 12, 13, 14, 17, 17);
    Cycle &c = mock.get();

    evLog.addEvent(std::make_unique<EvDebug>(strings[0], strings[1], strings[2], 0, &c, map)); // 1
    evLog.addEvent(std::make_unique<EvDebug>(strings[3], strings[4], strings[5], 7, &c, map)); // 1
    evLog.addEvent(std::make_unique<EvDebug>(strings[0], strings[1], strings[2], 2, &c, map)); // 10 <-- merge
    evLog.addEvent(std::make_unique<EvDebug>(strings[0], strings[1], strings[2], 3, &c, map)); // 10
    evLog.addEvent(std::make_unique<EvDebug>(strings[0], strings[1], strings[2], 4, &c, map)); // 10
    evLog.addEvent(std::make_unique<EvDebug>(strings[0], strings[1], strings[2], 2, &c, map)); // 11 <-- merge
    evLog.addEvent(std::make_unique<EvDebug>(strings[0], strings[1], strings[2], 2, &c, map)); // 12 <-- merge
    evLog.addEvent(std::make_unique<EvDebug>(strings[0], strings[1], strings[2], 2, &c, map)); // 13 <-- merge
    evLog.addEvent(std::make_unique<EvDebug>(strings[0], strings[1], strings[2], 2, &c, map)); // 14 <-- merge
    evLog.addEvent(std::make_unique<EvDebug>(strings[3], strings[1], strings[2], 2, &c, map)); // 17
    evLog.addEvent(std::make_unique<EvDebug>(strings[4], strings[1], strings[2], 6, &c, map)); // 17

    auto evList = evLog.getAllEvents();
    REQUIRE(evList.size() == 7);

    uint32_t begin, end;
    evList[2]->getCycleRange(&begin, &end);
    REQUIRE(begin == 10);
    REQUIRE(end == 14);
  }


  SECTION("Test polling") {
    When(Method(mock, getCycleNum)).Return(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11);
    Cycle &c = mock.get();

    uint32_t app1 = evLog.getAppID();
    uint32_t app2 = evLog.getAppID();
    REQUIRE(app1 != app2);

    auto pollA1V1 = evLog.pollEvents(app1);

    evLog.addEvent(std::make_unique<EvDebug>(strings[0], strings[1], strings[2], 0x0, &c, map));
    evLog.addEvent(std::make_unique<EvDebug>(strings[3], strings[4], strings[5], 0x1, &c, map));
    evLog.addEvent(std::make_unique<EvDebug>(strings[6], strings[7], strings[8], 0x2, &c, map));
    evLog.addEvent(std::make_unique<EvDebug>(strings[0], strings[1], strings[2], 0x3, &c, map));
    auto pollA1V2 = evLog.pollEvents(app1);
    auto pollA2V1 = evLog.pollEvents(app2);
    evLog.addEvent(std::make_unique<EvDebug>(strings[0], strings[1], strings[2], 0x4, &c, map));
    evLog.addEvent(std::make_unique<EvDebug>(strings[0], strings[1], strings[2], 0x5, &c, map));
    evLog.addEvent(std::make_unique<EvDebug>(strings[0], strings[1], strings[2], 0x6, &c, map));
    auto pollA2V2 = evLog.pollEvents(app2);
    evLog.addEvent(std::make_unique<EvDebug>(strings[0], strings[1], strings[2], 0x7, &c, map));
    evLog.addEvent(std::make_unique<EvDebug>(strings[0], strings[1], strings[2], 0x8, &c, map));
    auto pollA1V3 = evLog.pollEvents(app1);
    evLog.addEvent(std::make_unique<EvDebug>(strings[3], strings[1], strings[2], 0x9, &c, map));
    evLog.addEvent(std::make_unique<EvDebug>(strings[4], strings[1], strings[3], 0xA, &c, map));
    auto pollA2V3 = evLog.pollEvents(app2);

    REQUIRE(pollA1V1.size() == 0);
    REQUIRE(pollA1V2.size() == 4);
    REQUIRE(pollA1V3.size() == 5);
    REQUIRE(pollA2V1.size() == 4);
    REQUIRE(pollA2V2.size() == 3);
    REQUIRE(pollA2V3.size() == 4);

    // Test some (random) events
    REQUIRE(pollA1V2[2]->getEventFlags() == 0x2);
    REQUIRE(pollA2V1[3]->getEventFlags() == 0x3);
    REQUIRE(pollA1V3[4]->getEventFlags() == 0x8);
    REQUIRE(pollA2V2[1]->getEventFlags() == 0x5);
  }


  SECTION("Test Cycle range") {
    When(Method(mock, getCycleNum)).Return(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11);
    Cycle &c = mock.get();

    evLog.addEvent(std::make_unique<EvDebug>(strings[0], strings[1], strings[2], 0x0, &c, map));
    evLog.addEvent(std::make_unique<EvDebug>(strings[3], strings[4], strings[5], 0x1, &c, map));
    evLog.addEvent(std::make_unique<EvDebug>(strings[6], strings[7], strings[8], 0x2, &c, map));
    evLog.addEvent(std::make_unique<EvDebug>(strings[0], strings[1], strings[2], 0x3, &c, map));
    evLog.addEvent(std::make_unique<EvDebug>(strings[0], strings[1], strings[2], 0x4, &c, map));
    evLog.addEvent(std::make_unique<EvDebug>(strings[0], strings[1], strings[2], 0x5, &c, map));
    evLog.addEvent(std::make_unique<EvDebug>(strings[0], strings[1], strings[2], 0x6, &c, map));
    evLog.addEvent(std::make_unique<EvDebug>(strings[0], strings[1], strings[2], 0x7, &c, map));
    evLog.addEvent(std::make_unique<EvDebug>(strings[0], strings[1], strings[2], 0x8, &c, map));
    evLog.addEvent(std::make_unique<EvDebug>(strings[3], strings[1], strings[2], 0x9, &c, map));
    evLog.addEvent(std::make_unique<EvDebug>(strings[4], strings[1], strings[3], 0xA, &c, map));

    auto range1 = evLog.getEventsInRange(2, 6);
    auto range2 = evLog.getEventsInRange(1, 11);
    auto range3 = evLog.getEventsInRange(-1, 8);
    auto range4 = evLog.getEventsInRange(5, -1);
    auto range5 = evLog.getEventsInRange(-1, -1);

    REQUIRE(range1.size() == 5);
    REQUIRE(range2.size() == 11);
    REQUIRE(range3.size() == 8);
    REQUIRE(range4.size() == 7);
    REQUIRE(range5.size() == 11);

    // Test some (random) events
    REQUIRE(range1[2]->getEventFlags() == 0x3);
    REQUIRE(range2[3]->getEventFlags() == 0x3);
    REQUIRE(range3[4]->getEventFlags() == 0x4);
    REQUIRE(range4[1]->getEventFlags() == 0x5);
    REQUIRE(range5[8]->getEventFlags() == 0x8);
    REQUIRE(range2 == range5);
  }
}
