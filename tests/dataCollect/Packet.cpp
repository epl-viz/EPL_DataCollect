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
  plf::colony<PacketDiff> ods;
  ods.emplace(1, 0, UINT64_C(10));
  ods.emplace(2, 0, 10.0);
  ods.emplace(3, 1, UINT64_C(10));
  ods.emplace(4, 0, UINT64_C(10));

  std::string wire  = "assdagagdagaerg";
  std::string other = "affbytsysrtharthathaeheeh";

  auto tp = std::chrono::system_clock::now();

  WiresharkParser::parserData d;
  d.dst      = 20;
  d.src      = 10;
  d.wsString = wire;
  d.wsOther  = other;
  d.tp       = tp;
  d.pType    = PacketType::POLL_REQUEST;
  Packet p(&d, 123, 12, 1234);

  for (auto &i : ods) {
    p.addDiff(i);
  }

  REQUIRE(p.getMiscData() == other);
  REQUIRE(p.getWiresharkString() == wire);
  REQUIRE(p.getDestNode() == 20);
  REQUIRE(p.getSrcNode() == 10);
  REQUIRE(p.getTimeStamp() == tp);
  REQUIRE(p.getType() == PacketType::POLL_REQUEST);
  REQUIRE(p.getOffset() == 123);
  REQUIRE(p.getPhysicalFileOffset() == 12);
  REQUIRE(p.getPacketIndex() == 1234);

  auto diffs = p.getDiffs();
  for (auto &it : ods) {
    (void)it;
    bool found = true;
    for (auto &i : *diffs) {
      REQUIRE(i.getIndex() <= 4);
      REQUIRE(i.getSubIndex() <= 1);
      //       if (i.getEntry()->getDataType() == it->getDataType()) {
      //         found = true;
      //         break;
      //       }
    }

    REQUIRE(found == true);
  }
}
