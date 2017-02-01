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

#include <ODEntryContainer.hpp>
#include <catch.hpp>
#include <fakeit.hpp>

using namespace EPL_DataCollect;
using namespace fakeit;

bool checkDouble(double a, double b);
bool checkDouble(double a, double b) {
  double t = (a > b) ? (a - b) : (b - a);
  return t < 0.0000001;
}

TEST_CASE("Testing ODT 2 OCT", "[ODEntry]") {
  REQUIRE(ODEntryContainer::getOCTbyODT(OT_VAR, ODT_BOOLEAN) == OCT_BOOL);

  REQUIRE(ODEntryContainer::getOCTbyODT(OT_VAR, ODT_INTEGER8) == OCT_INTEGER);
  REQUIRE(ODEntryContainer::getOCTbyODT(OT_VAR, ODT_INTEGER16) == OCT_INTEGER);
  REQUIRE(ODEntryContainer::getOCTbyODT(OT_VAR, ODT_INTEGER24) == OCT_INTEGER);
  REQUIRE(ODEntryContainer::getOCTbyODT(OT_VAR, ODT_INTEGER32) == OCT_INTEGER);
  REQUIRE(ODEntryContainer::getOCTbyODT(OT_VAR, ODT_INTEGER40) == OCT_INTEGER);
  REQUIRE(ODEntryContainer::getOCTbyODT(OT_VAR, ODT_INTEGER48) == OCT_INTEGER);
  REQUIRE(ODEntryContainer::getOCTbyODT(OT_VAR, ODT_INTEGER56) == OCT_INTEGER);
  REQUIRE(ODEntryContainer::getOCTbyODT(OT_VAR, ODT_INTEGER64) == OCT_INTEGER);

  REQUIRE(ODEntryContainer::getOCTbyODT(OT_VAR, ODT_UNSIGNED8) == OCT_UNSIGNED);
  REQUIRE(ODEntryContainer::getOCTbyODT(OT_VAR, ODT_UNSIGNED16) == OCT_UNSIGNED);
  REQUIRE(ODEntryContainer::getOCTbyODT(OT_VAR, ODT_UNSIGNED24) == OCT_UNSIGNED);
  REQUIRE(ODEntryContainer::getOCTbyODT(OT_VAR, ODT_UNSIGNED32) == OCT_UNSIGNED);
  REQUIRE(ODEntryContainer::getOCTbyODT(OT_VAR, ODT_UNSIGNED40) == OCT_UNSIGNED);
  REQUIRE(ODEntryContainer::getOCTbyODT(OT_VAR, ODT_UNSIGNED48) == OCT_UNSIGNED);
  REQUIRE(ODEntryContainer::getOCTbyODT(OT_VAR, ODT_UNSIGNED56) == OCT_UNSIGNED);
  REQUIRE(ODEntryContainer::getOCTbyODT(OT_VAR, ODT_UNSIGNED64) == OCT_UNSIGNED);

  REQUIRE(ODEntryContainer::getOCTbyODT(OT_VAR, ODT_REAL32) == OCT_REAL);
  REQUIRE(ODEntryContainer::getOCTbyODT(OT_VAR, ODT_REAL64) == OCT_REAL);

  REQUIRE(ODEntryContainer::getOCTbyODT(OT_ARRAY, ODT_BOOLEAN) == OCT_ARRAY_BOOL);

  REQUIRE(ODEntryContainer::getOCTbyODT(OT_ARRAY, ODT_INTEGER8) == OCT_ARRAY_INTEGER);
  REQUIRE(ODEntryContainer::getOCTbyODT(OT_ARRAY, ODT_INTEGER16) == OCT_ARRAY_INTEGER);
  REQUIRE(ODEntryContainer::getOCTbyODT(OT_ARRAY, ODT_INTEGER24) == OCT_ARRAY_INTEGER);
  REQUIRE(ODEntryContainer::getOCTbyODT(OT_ARRAY, ODT_INTEGER32) == OCT_ARRAY_INTEGER);
  REQUIRE(ODEntryContainer::getOCTbyODT(OT_ARRAY, ODT_INTEGER40) == OCT_ARRAY_INTEGER);
  REQUIRE(ODEntryContainer::getOCTbyODT(OT_ARRAY, ODT_INTEGER48) == OCT_ARRAY_INTEGER);
  REQUIRE(ODEntryContainer::getOCTbyODT(OT_ARRAY, ODT_INTEGER56) == OCT_ARRAY_INTEGER);
  REQUIRE(ODEntryContainer::getOCTbyODT(OT_ARRAY, ODT_INTEGER64) == OCT_ARRAY_INTEGER);

  REQUIRE(ODEntryContainer::getOCTbyODT(OT_ARRAY, ODT_UNSIGNED8) == OCT_ARRAY_UNSIGNED);
  REQUIRE(ODEntryContainer::getOCTbyODT(OT_ARRAY, ODT_UNSIGNED16) == OCT_ARRAY_UNSIGNED);
  REQUIRE(ODEntryContainer::getOCTbyODT(OT_ARRAY, ODT_UNSIGNED24) == OCT_ARRAY_UNSIGNED);
  REQUIRE(ODEntryContainer::getOCTbyODT(OT_ARRAY, ODT_UNSIGNED32) == OCT_ARRAY_UNSIGNED);
  REQUIRE(ODEntryContainer::getOCTbyODT(OT_ARRAY, ODT_UNSIGNED40) == OCT_ARRAY_UNSIGNED);
  REQUIRE(ODEntryContainer::getOCTbyODT(OT_ARRAY, ODT_UNSIGNED48) == OCT_ARRAY_UNSIGNED);
  REQUIRE(ODEntryContainer::getOCTbyODT(OT_ARRAY, ODT_UNSIGNED56) == OCT_ARRAY_UNSIGNED);
  REQUIRE(ODEntryContainer::getOCTbyODT(OT_ARRAY, ODT_UNSIGNED64) == OCT_ARRAY_UNSIGNED);

  REQUIRE(ODEntryContainer::getOCTbyODT(OT_ARRAY, ODT_REAL32) == OCT_ARRAY_REAL);
  REQUIRE(ODEntryContainer::getOCTbyODT(OT_ARRAY, ODT_REAL64) == OCT_ARRAY_REAL);

  REQUIRE(ODEntryContainer::getOCTbyODT(OT_VAR, ODT_VISIBLE_STRING) == OCT_STRING);
  REQUIRE(ODEntryContainer::getOCTbyODT(OT_VAR, ODT_OCTET_STRING) == OCT_STRING);
  REQUIRE(ODEntryContainer::getOCTbyODT(OT_VAR, ODT_UNICODE_STRING) == OCT_STRING);

  REQUIRE(ODEntryContainer::getOCTbyODT(OT_VAR, ODT_IDENTITY) == OCT_COMPLEX);
}

TEST_CASE("Testing ODEntryInt", "[ODEntry]") {
  std::cout << "ODEntry container size: \x1b[33;1m" << EPL_DataCollect::internal::calcSize() << "\x1b[0m Byte"
            << std::endl;

  ODEntryContainer ct(ODT_INTEGER32);
  auto             ptr   = *ct;
  ODEntryInt *     t     = ct.getData<ODEntryInt>();
  ODEntry *        entry = *ct;
  t->data                = 42;
  REQUIRE(t->getType() == OCT_INTEGER);
  REQUIRE(t->getType() == ct->getType());
  REQUIRE(t->getDataType() == ODT_INTEGER32);
  REQUIRE(t->getDataType() == entry->getDataType());
  REQUIRE(t->isNumericValue() == true);
  REQUIRE(checkDouble(t->getNumericValue(), 42) == true);
  REQUIRE(ptr == entry);

  // Test copying of complex stuff
  ODEntryContainer ct2(ct);
  ODEntryContainer ct3(std::move(ct));
  ct2 = ct3;
  ct3 = std::move(ct2);
}

TEST_CASE("Testing ODEntryUInt", "[ODEntry]") {
  ODEntryContainer ct(ODT_UNSIGNED16);
  ODEntryUInt &    t = *ct.getData<ODEntryUInt>();
  t.data             = 42;
  REQUIRE(t.getType() == OCT_UNSIGNED);
  REQUIRE(t.getDataType() == ODT_UNSIGNED16);
  REQUIRE(t.isNumericValue() == true);
  REQUIRE(checkDouble(t.getNumericValue(), 42) == true);

  ODEntryContainer ct2(ct);
  ODEntryContainer ct3(std::move(ct));

  // Test copying of complex stuff
  ct2 = ct3;
  ct3 = std::move(ct2);
}

TEST_CASE("Testing ODEntryBool", "[ODEntry]") {
  ODEntryContainer ct(ODT_BOOLEAN);
  ODEntryBool &    t = *ct.getData<ODEntryBool>();
  t.data             = true;
  REQUIRE(t.getType() == OCT_BOOL);
  REQUIRE(t.getDataType() == ODT_BOOLEAN);
  REQUIRE(t.isNumericValue() == true);
  REQUIRE(checkDouble(t.getNumericValue(), 1) == true);

  // Test copying of complex stuff
  ODEntryContainer ct2(ct);
  ODEntryContainer ct3(std::move(ct));

  ct2 = ct3;
  ct3 = std::move(ct2);
}

TEST_CASE("Testing ODEntryReal", "[ODEntry]") {
  ODEntryContainer ct(ODT_REAL32);
  ODEntryReal &    t = *ct.getData<ODEntryReal>();
  t.data             = 42;
  REQUIRE(t.getType() == OCT_REAL);
  REQUIRE(t.getDataType() == ODT_REAL32);
  REQUIRE(t.isNumericValue() == true);
  REQUIRE(checkDouble(t.getNumericValue(), 42) == true);

  // Test copying of complex stuff
  ODEntryContainer ct2(ct);
  ODEntryContainer ct3(std::move(ct));

  ct2 = ct3;
  ct3 = std::move(ct2);
}

TEST_CASE("Testing ODEntryString", "[ODEntry]") {
  ODEntryContainer ct(ODT_VISIBLE_STRING);
  ODEntryString &  t = *ct.getData<ODEntryString>();
  t.data             = "Hello world";
  REQUIRE(t.getType() == OCT_STRING);
  REQUIRE(t.getDataType() == ODT_VISIBLE_STRING);
  REQUIRE(t.isNumericValue() == false);
  REQUIRE(checkDouble(t.getNumericValue(), 0) == true);

  // Test copying of complex stuff
  ODEntryContainer ct2(ct);
  t = *ct2.getData<ODEntryString>();
  REQUIRE(t.data == "Hello world");
  ODEntryContainer ct3(std::move(ct));
  t = *ct3.getData<ODEntryString>();
  REQUIRE(t.data == "Hello world");

  ct2 = ct3;
  t   = *ct2.getData<ODEntryString>();
  REQUIRE(t.data == "Hello world");
  ct3 = std::move(ct2);
  t   = *ct3.getData<ODEntryString>();
  REQUIRE(t.data == "Hello world");
}

TEST_CASE("Testing ODEntryArrayInt", "[ODEntry]") {
  ODEntryContainer ct(ODT_INTEGER40, OT_ARRAY);
  ODEntryArrayInt &t = *ct.getData<ODEntryArrayInt>();
  REQUIRE(t.getType() == OCT_ARRAY_INTEGER);
  REQUIRE(t.getDataType() == ODT_INTEGER40);
  REQUIRE(t.isNumericValue() == false);
  REQUIRE(checkDouble(t.getNumericValue(), 0) == true);

  // Test copying of complex stuff
  ODEntryContainer ct2(ct);
  ODEntryContainer ct3(std::move(ct));

  ct2 = ct3;
  ct3 = std::move(ct2);
}

TEST_CASE("Testing ODEntryArrayUInt", "[ODEntry]") {
  ODEntryContainer  ct(ODT_UNSIGNED56, OT_ARRAY);
  ODEntryArrayUInt &t = *ct.getData<ODEntryArrayUInt>();
  REQUIRE(t.getType() == OCT_ARRAY_UNSIGNED);
  REQUIRE(t.getDataType() == ODT_UNSIGNED56);
  REQUIRE(t.isNumericValue() == false);
  REQUIRE(checkDouble(t.getNumericValue(), 0) == true);

  // Test copying of complex stuff
  ODEntryContainer ct2(ct);
  ODEntryContainer ct3(std::move(ct));

  ct2 = ct3;
  ct3 = std::move(ct2);
}

TEST_CASE("Testing ODEntryArrayBool", "[ODEntry]") {
  ODEntryContainer  ct(ODT_BOOLEAN, OT_ARRAY);
  ODEntryArrayBool &t = *ct.getData<ODEntryArrayBool>();
  REQUIRE(t.getType() == OCT_ARRAY_BOOL);
  REQUIRE(t.getDataType() == ODT_BOOLEAN);
  REQUIRE(t.isNumericValue() == false);
  REQUIRE(checkDouble(t.getNumericValue(), 0) == true);

  // Test copying of complex stuff
  ODEntryContainer ct2(ct);
  ODEntryContainer ct3(std::move(ct));

  ct2 = ct3;
  ct3 = std::move(ct2);
}

TEST_CASE("Testing ODEntryArrayReal", "[ODEntry]") {
  ODEntryContainer  ct(OCT_ARRAY_REAL, ODT_VISIBLE_STRING);
  ODEntryArrayReal &t = *ct.getData<ODEntryArrayReal>();
  REQUIRE(t.getType() == OCT_ARRAY_REAL);
  REQUIRE(t.getDataType() == ODT_VISIBLE_STRING);
  REQUIRE(t.isNumericValue() == false);
  REQUIRE(checkDouble(t.getNumericValue(), 0) == true);

  // Test copying of complex stuff
  ODEntryContainer ct2(ct);
  ODEntryContainer ct3(std::move(ct));

  ct2 = ct3;
  ct3 = std::move(ct2);
}

TEST_CASE("Testing ODEntryComplex", "[ODEntry]") {
  ODEntryContainer ct(OCT_COMPLEX, ODT_VISIBLE_STRING);
  ODEntryComplex & t = *ct.getData<ODEntryComplex>();
  REQUIRE(t.getType() == OCT_COMPLEX);
  REQUIRE(t.getDataType() == ODT_VISIBLE_STRING);
  REQUIRE(t.isNumericValue() == false);
  REQUIRE(checkDouble(t.getNumericValue(), 0) == true);

  // Test copying of complex stuff
  ODEntryContainer ct2(ct);
  ODEntryContainer ct3(std::move(ct));

  ct2 = ct3;
  ct3 = std::move(ct2);
}
