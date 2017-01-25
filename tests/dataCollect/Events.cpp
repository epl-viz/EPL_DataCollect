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
#include <EvError.hpp>
#include <EvInfo.hpp>
#include <EvPluginText.hpp>
#include <EvProtoError.hpp>
#include <EvWarning.hpp>
#include <EventBase.hpp>
#include <catch.hpp>

using namespace EPL_DataCollect;

TEST_CASE("Test all event classes", "[Events]") {
  Cycle                c;
  EventBase::INDEX_MAP map;
  map[1]    = "asdf";
  map[1000] = "dfsfgh";

  std::string strings[] = {"TestPlugin",
                           "ASDG",
                           "Something",
                           "Something rather long",
                           "Hello World",
                           "StrStrStrStr",
                           "\x1b[31;1mThis should be bold red\x1b[0m",
                           "This String has a\n newline",
                           "This string \0has \0null bytes"};

  SECTION("Test setting the cycle range") {
    uint32_t     first = 0xDEADBEAF, last = 0xDEADBEAF;
    EvProtoError ev(strings[3], strings[4], strings[5], 0, &c, map);

    ev.getCycleRange(&first, &last);
    REQUIRE(first == c.getCycleNum());
    REQUIRE(last == c.getCycleNum());

    ev.updateRange(0xDEAD, 0xBADA55);

    ev.getCycleRange(&first, &last);
    REQUIRE(first == 0xDEAD);
    REQUIRE(last == 0xBADA55);

    ev.updateRange(-1, -10);

    ev.getCycleRange(&first, &last);
    REQUIRE(first == 0xDEAD);
    REQUIRE(last == 0xBADA55);
  }

  SECTION("Test EvProtoError") {
    uint32_t     first = 0xDEADBEAF, last = 0xDEADBEAF;
    EvProtoError ev(strings[0], strings[1], strings[2], 0, &c, map);
    REQUIRE(ev.getTypeAsString() == "EVT_PROTO_ERROR");
    REQUIRE(ev.getType() == EVT_PROTO_ERROR);
    REQUIRE(ev.getPluginID() == strings[0]);
    REQUIRE(ev.getName() == strings[1]);
    REQUIRE(ev.getDescription() == strings[2]);
    REQUIRE(ev.getEventFlags() == 0);
    REQUIRE(ev.getAffectedIndices() == map);

    ev.getCycleRange(&first, &last);
    REQUIRE(first == c.getCycleNum());
    REQUIRE(last == c.getCycleNum());
  }

  SECTION("Test EvError") {
    uint32_t first = 0xDEADBEAF, last = 0xDEADBEAF;
    EvError  ev(strings[6], strings[7], strings[8], 0, nullptr, map);
    REQUIRE(ev.getTypeAsString() == "EVT_ERROR");
    REQUIRE(ev.getType() == EVT_ERROR);
    REQUIRE(ev.getPluginID() == strings[6]);
    REQUIRE(ev.getName() == strings[7]);
    REQUIRE(ev.getDescription() == strings[8]);
    REQUIRE(ev.getEventFlags() == 0);
    REQUIRE(ev.getAffectedIndices() == map);

    ev.getCycleRange(&first, &last);
    REQUIRE(first == UINT32_MAX);
    REQUIRE(last == UINT32_MAX);
  }

  SECTION("Test EvWarning") {
    uint32_t  first = 0xDEADBEAF, last = 0xDEADBEAF;
    EvWarning ev(strings[0], strings[1], strings[2], 0, &c, map);
    REQUIRE(ev.getTypeAsString() == "EVT_WARNING");
    REQUIRE(ev.getType() == EVT_WARNING);
  }

  SECTION("Test EvInfo") {
    uint32_t first = 0xDEADBEAF, last = 0xDEADBEAF;
    EvInfo   ev(strings[0], strings[1], strings[2], 0, &c, map);
    REQUIRE(ev.getTypeAsString() == "EVT_INFO");
    REQUIRE(ev.getType() == EVT_INFO);
  }

  SECTION("Test EvDebug") {
    uint32_t first = 0xDEADBEAF, last = 0xDEADBEAF;
    EvDebug  ev(strings[0], strings[1], strings[2], 0, &c, map);
    REQUIRE(ev.getTypeAsString() == "EVT_DEBUG");
    REQUIRE(ev.getType() == EVT_DEBUG);
  }

  SECTION("Test EvPluginText") {
    uint32_t     first = 0xDEADBEAF, last = 0xDEADBEAF;
    EvPluginText ev(strings[0], strings[1], strings[2], 0, &c, map);
    REQUIRE(ev.getTypeAsString() == "EVT_PLUGIN_EV_TEXT");
    REQUIRE(ev.getType() == EVT_PLUGIN_EV_TEXT);
  }
}
