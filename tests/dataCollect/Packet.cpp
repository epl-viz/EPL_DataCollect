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

#include <Packet.hpp>
#include <catch.hpp>
#include <fakeit.hpp>

using namespace EPL_DataCollect;
using namespace fakeit;

TEST_CASE("Testing Packet", "[Packet]") {
  plf::colony<std::shared_ptr<ODEntry>> ods;
  ods.emplace(std::make_shared<ODEntry>());
  ods.emplace(std::make_shared<ODEntry>());
  ods.emplace(std::make_shared<ODEntry>());
  ods.emplace(std::make_shared<ODEntry>());

  std::string wire  = "assdagagdagaerg";
  std::string other = "affbytsysrtharthathaeheeh";

  auto tp = std::chrono::system_clock::now();

  Packet p(PT_PDO_REQ, CMD_ID_WRITE_BY_NAME, nullptr, wire, other, 10, 20, tp, 1, 2);

  for (auto &i : ods) {
    p.addDiff(1, i);
  }

  REQUIRE(p.getODDesc() == nullptr);
  REQUIRE(p.getOtherData() == other);
  REQUIRE(p.getWiresharkString() == wire);
  REQUIRE(p.getDestNode() == 20);
  REQUIRE(p.getSrcNode() == 10);
  REQUIRE(p.getTimeStamp() == tp);
  REQUIRE(p.getTransactionID() == 1);
  REQUIRE(p.getNumSegments() == 2);

  auto diffs = p.getDiffs();
  for (auto &it : ods) {
    bool found = false;
    for (auto &i : *diffs) {
      REQUIRE(i.getIndex() == 1);
      if (i.getEntry() == it.get()) {
        found = true;
        break;
      }
    }

    REQUIRE(found == true);
  }
}
