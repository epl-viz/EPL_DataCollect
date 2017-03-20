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

#include <Node.hpp>
#include <EPLEnums.h>
#include <catch.hpp>

using namespace EPL_DataCollect;

TEST_CASE("Initialization succeeds", "[Node]") {
  Node n(1);

  REQUIRE(n.getXDDFiles().empty());
  REQUIRE(n.getIdentity().Profile == UINT16_MAX);

  SECTION("Check for correct node ID") { REQUIRE(n.getID() == 1); }
  SECTION("Test node status initialized to unknown") { REQUIRE(n.getStatus() == NMTState::OFF); }
  SECTION("Test OD Initialization") {
    REQUIRE(n.getOD() != nullptr);
    REQUIRE(n.getODDesc() != nullptr);
  }
}

TEST_CASE("Changing Status works") {
  Node n(1);

  SECTION("Test setting to INITIALISING") {
    n.setStatus(NMTState::INITIALISING);
    REQUIRE(n.getStatus() == NMTState::INITIALISING);
    REQUIRE(n.getStatusStr() == "INITIALISING");
  }
  SECTION("Test setting to OPERATIONAL") {
    n.setStatus(NMTState::OPERATIONAL);
    REQUIRE(n.getStatus() == NMTState::OPERATIONAL);
  }
  SECTION("Test setting to STOPPED") {
    n.setStatus(NMTState::STOPPED);
    REQUIRE(n.getStatus() == NMTState::STOPPED);
  }
}
