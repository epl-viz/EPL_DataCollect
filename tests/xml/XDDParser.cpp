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

#include <OD.hpp>
#include <XDDParser.hpp>
#include <catch.hpp>
#include <fakeit.hpp>

using namespace EPL_DataCollect;
using namespace fakeit;

const std::string DATA_ROOT = constants::EPL_DC_BUILD_DIR_ROOT + "/external/resources";

TEST_CASE("Testing XDD parser", "[xml]") {
  OD od;

  std::string f1 = DATA_ROOT + "/pcaps/EPL_Exa";
  std::string f2 = DATA_ROOT + "/pcaps";
  std::string f3 = DATA_ROOT + "/pcaps/EPL_Example.cap";
  std::string f4 = DATA_ROOT + "/profiles/00000000_POWERLINK_CiA302-4_RMN.xdd";

  REQUIRE(XDDParser::parseXDD(nullptr, f1) == XDDParser::FILE_DOES_NOT_EXIST);
  REQUIRE(XDDParser::parseXDD(nullptr, f2) == XDDParser::FILE_IS_NOT_A_REGUAR_FILE);
  REQUIRE(XDDParser::parseXDD(nullptr, f3) == XDDParser::FAILED_TO_PARSE_XML);
  REQUIRE(XDDParser::parseXDD(&od, f4) == XDDParser::SUCCESS);

  REQUIRE(od.getODDesc()->exists(0x1008) == true);
  REQUIRE(od.getODDesc()->exists(0x1030) == true);
  REQUIRE(od.getODDesc()->exists(0x1050) == true);

  ODEntryDescription *d1 = od.getODDesc()->getEntry(0x1008);
  ODEntryDescription *d2 = od.getODDesc()->getEntry(0x1030);
  ODEntryDescription *d3 = od.getODDesc()->getEntry(0x1050);

  REQUIRE(d1 != nullptr);
  REQUIRE(d2 != nullptr);
  REQUIRE(d3 != nullptr);

  REQUIRE(d1->index == 0x1008);
  REQUIRE(d1->name == "NMT_ManufactDevName_VS");
  REQUIRE(d1->type == ObjectType::VAR);
  REQUIRE(d1->dataType == ObjectDataType::VISIBLE_STRING);
  REQUIRE(d1->defaultValue->toString() == "openPOWERLINK device");
  REQUIRE(d1->access == ObjectAccess::CONST);
  REQUIRE(d1->pdo == ObjectPDOMapping::NO);

  REQUIRE(d2->index == 0x1030);
  REQUIRE(d2->name == "NMT_InterfaceGroup_0h_REC");
  REQUIRE(d2->type == ObjectType::RECORD);
  REQUIRE(d2->subEntries.size() == 10);
  REQUIRE(d2->subEntries[0].name == "NumberOfEntries");
  REQUIRE(d2->subEntries[1].name == "InterfaceIndex_U16");
  REQUIRE(d2->subEntries[2].name == "InterfaceDescription_VSTR");
  REQUIRE(d2->subEntries[3].name == "InterfaceType_U8");
  REQUIRE(d2->subEntries[4].name == "InterfaceMtu_U16");
  REQUIRE(d2->subEntries[5].name == "InterfacePhysAddress_OSTR");
  REQUIRE(d2->subEntries[6].name == "InterfaceName_VSTR");
  REQUIRE(d2->subEntries[7].name == "InterfaceOperStatus_U8");
  REQUIRE(d2->subEntries[8].name == "InterfaceAdminState_U8");
  REQUIRE(d2->subEntries[9].name == "Valid_BOOL");

  REQUIRE(d2->subEntries[0].type == ObjectType::VAR);
  REQUIRE(d2->subEntries[0].dataType == ObjectDataType::UNSIGNED8);

  REQUIRE(d2->subEntries[1].dataType == ObjectDataType::UNSIGNED16);
  REQUIRE(d2->subEntries[1].valueMin->toString() == "0");
  REQUIRE(d2->subEntries[1].valueMax->toString() == "10");
  REQUIRE(d2->subEntries[1].defaultValue->toString() == "1");

  REQUIRE(d3->index == 0x1050);
  REQUIRE(d3->type == ObjectType::ARRAY);
}
