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

#include <ODDescription.hpp>
#include <catch.hpp>

using namespace EPL_DataCollect;

TEST_CASE("Test ODDescription", "[Model]") {
  ODEntryDescription entries[16];
  for (int i = 0; i < 16; i++) {
    entries[i].name = "Entry " + std::to_string(i);
  }

  ODDescription d1, d2;

  SECTION("Sanity check generated entries") {
    REQUIRE(entries[0].name == "Entry 0");
    REQUIRE(entries[1].name == "Entry 1");
    REQUIRE(entries[4].name == "Entry 4");
    REQUIRE(entries[14].name == "Entry 14");
    REQUIRE(entries[15].name == "Entry 15");
  }

  SECTION("Test simple adding and getting") {
    REQUIRE(d1.setEntry(100, entries[0]) == true);
    REQUIRE(d1.setEntry(101, entries[2]) == true);
    REQUIRE(d1.exists(100) == true);
    REQUIRE(d1.exists(500) == false);
    REQUIRE(d1.setEntry(100, entries[14]) == false);
    REQUIRE(d1.setEntry(101, entries[15]) == false);
    REQUIRE(d1.getEntry(100)->name == "Entry 0");
    REQUIRE(d1.getEntry(101)->name == "Entry 2");
  }

  SECTION("Override Entries") {
    REQUIRE(d1.setEntry(100, entries[0]) == true);
    REQUIRE(d1.overrideEntry(103, entries[5]) == false);
    REQUIRE(d1.exists(100) == true);
    REQUIRE(d1.exists(103) == false);
    REQUIRE(d1.setEntry(100, entries[14]) == false);
    REQUIRE(d1.getEntry(100)->name == "Entry 0");
    REQUIRE(d1.overrideEntry(100, entries[7]) == true);
    REQUIRE(d1.getEntry(100)->name == "Entry 7");
  }

  SECTION("Merging") {
    REQUIRE(d1.setEntry(100, entries[0]) == true);
    REQUIRE(d1.setEntry(101, entries[1]) == true);
    REQUIRE(d1.setEntry(102, entries[2]) == true);
    REQUIRE(d1.setEntry(103, entries[3]) == true);
    REQUIRE(d2.setEntry(103, entries[10]) == true);
    REQUIRE(d2.setEntry(104, entries[11]) == true);
    REQUIRE(d2.setEntry(105, entries[12]) == true);
    REQUIRE(d2.setEntry(106, entries[13]) == true);
    d1.applyDesc(d2);
    REQUIRE(d1.getEntry(100)->name == "Entry 0");
    REQUIRE(d1.getEntry(101)->name == "Entry 1");
    REQUIRE(d1.getEntry(102)->name == "Entry 2");
    REQUIRE(d1.getEntry(103)->name == "Entry 10");
    REQUIRE(d1.getEntry(104)->name == "Entry 11");
    REQUIRE(d1.getEntry(105)->name == "Entry 12");
    REQUIRE(d1.getEntry(106)->name == "Entry 13");
  }
}
