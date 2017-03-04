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
  REQUIRE(ODEntryContainer::getOCTbyODT(ObjectType::VAR, ObjectDataType::BOOLEAN) == ObjectClassType::BOOL);

  REQUIRE(ODEntryContainer::getOCTbyODT(ObjectType::VAR, ObjectDataType::INTEGER8) == ObjectClassType::INTEGER);
  REQUIRE(ODEntryContainer::getOCTbyODT(ObjectType::VAR, ObjectDataType::INTEGER16) == ObjectClassType::INTEGER);
  REQUIRE(ODEntryContainer::getOCTbyODT(ObjectType::VAR, ObjectDataType::INTEGER24) == ObjectClassType::INTEGER);
  REQUIRE(ODEntryContainer::getOCTbyODT(ObjectType::VAR, ObjectDataType::INTEGER32) == ObjectClassType::INTEGER);
  REQUIRE(ODEntryContainer::getOCTbyODT(ObjectType::VAR, ObjectDataType::INTEGER40) == ObjectClassType::INTEGER);
  REQUIRE(ODEntryContainer::getOCTbyODT(ObjectType::VAR, ObjectDataType::INTEGER48) == ObjectClassType::INTEGER);
  REQUIRE(ODEntryContainer::getOCTbyODT(ObjectType::VAR, ObjectDataType::INTEGER56) == ObjectClassType::INTEGER);
  REQUIRE(ODEntryContainer::getOCTbyODT(ObjectType::VAR, ObjectDataType::INTEGER64) == ObjectClassType::INTEGER);

  REQUIRE(ODEntryContainer::getOCTbyODT(ObjectType::VAR, ObjectDataType::UNSIGNED8) == ObjectClassType::UNSIGNED);
  REQUIRE(ODEntryContainer::getOCTbyODT(ObjectType::VAR, ObjectDataType::UNSIGNED16) == ObjectClassType::UNSIGNED);
  REQUIRE(ODEntryContainer::getOCTbyODT(ObjectType::VAR, ObjectDataType::UNSIGNED24) == ObjectClassType::UNSIGNED);
  REQUIRE(ODEntryContainer::getOCTbyODT(ObjectType::VAR, ObjectDataType::UNSIGNED32) == ObjectClassType::UNSIGNED);
  REQUIRE(ODEntryContainer::getOCTbyODT(ObjectType::VAR, ObjectDataType::UNSIGNED40) == ObjectClassType::UNSIGNED);
  REQUIRE(ODEntryContainer::getOCTbyODT(ObjectType::VAR, ObjectDataType::UNSIGNED48) == ObjectClassType::UNSIGNED);
  REQUIRE(ODEntryContainer::getOCTbyODT(ObjectType::VAR, ObjectDataType::UNSIGNED56) == ObjectClassType::UNSIGNED);
  REQUIRE(ODEntryContainer::getOCTbyODT(ObjectType::VAR, ObjectDataType::UNSIGNED64) == ObjectClassType::UNSIGNED);

  REQUIRE(ODEntryContainer::getOCTbyODT(ObjectType::VAR, ObjectDataType::REAL32) == ObjectClassType::REAL);
  REQUIRE(ODEntryContainer::getOCTbyODT(ObjectType::VAR, ObjectDataType::REAL64) == ObjectClassType::REAL);

  REQUIRE(ODEntryContainer::getOCTbyODT(ObjectType::ARRAY, ObjectDataType::BOOLEAN) == ObjectClassType::ARRAY_BOOL);

  REQUIRE(ODEntryContainer::getOCTbyODT(ObjectType::ARRAY, ObjectDataType::INTEGER8) == ObjectClassType::ARRAY_INTEGER);
  REQUIRE(ODEntryContainer::getOCTbyODT(ObjectType::ARRAY, ObjectDataType::INTEGER16) ==
          ObjectClassType::ARRAY_INTEGER);
  REQUIRE(ODEntryContainer::getOCTbyODT(ObjectType::ARRAY, ObjectDataType::INTEGER24) ==
          ObjectClassType::ARRAY_INTEGER);
  REQUIRE(ODEntryContainer::getOCTbyODT(ObjectType::ARRAY, ObjectDataType::INTEGER32) ==
          ObjectClassType::ARRAY_INTEGER);
  REQUIRE(ODEntryContainer::getOCTbyODT(ObjectType::ARRAY, ObjectDataType::INTEGER40) ==
          ObjectClassType::ARRAY_INTEGER);
  REQUIRE(ODEntryContainer::getOCTbyODT(ObjectType::ARRAY, ObjectDataType::INTEGER48) ==
          ObjectClassType::ARRAY_INTEGER);
  REQUIRE(ODEntryContainer::getOCTbyODT(ObjectType::ARRAY, ObjectDataType::INTEGER56) ==
          ObjectClassType::ARRAY_INTEGER);
  REQUIRE(ODEntryContainer::getOCTbyODT(ObjectType::ARRAY, ObjectDataType::INTEGER64) ==
          ObjectClassType::ARRAY_INTEGER);

  REQUIRE(ODEntryContainer::getOCTbyODT(ObjectType::ARRAY, ObjectDataType::UNSIGNED8) ==
          ObjectClassType::ARRAY_UNSIGNED);
  REQUIRE(ODEntryContainer::getOCTbyODT(ObjectType::ARRAY, ObjectDataType::UNSIGNED16) ==
          ObjectClassType::ARRAY_UNSIGNED);
  REQUIRE(ODEntryContainer::getOCTbyODT(ObjectType::ARRAY, ObjectDataType::UNSIGNED24) ==
          ObjectClassType::ARRAY_UNSIGNED);
  REQUIRE(ODEntryContainer::getOCTbyODT(ObjectType::ARRAY, ObjectDataType::UNSIGNED32) ==
          ObjectClassType::ARRAY_UNSIGNED);
  REQUIRE(ODEntryContainer::getOCTbyODT(ObjectType::ARRAY, ObjectDataType::UNSIGNED40) ==
          ObjectClassType::ARRAY_UNSIGNED);
  REQUIRE(ODEntryContainer::getOCTbyODT(ObjectType::ARRAY, ObjectDataType::UNSIGNED48) ==
          ObjectClassType::ARRAY_UNSIGNED);
  REQUIRE(ODEntryContainer::getOCTbyODT(ObjectType::ARRAY, ObjectDataType::UNSIGNED56) ==
          ObjectClassType::ARRAY_UNSIGNED);
  REQUIRE(ODEntryContainer::getOCTbyODT(ObjectType::ARRAY, ObjectDataType::UNSIGNED64) ==
          ObjectClassType::ARRAY_UNSIGNED);

  REQUIRE(ODEntryContainer::getOCTbyODT(ObjectType::ARRAY, ObjectDataType::REAL32) == ObjectClassType::ARRAY_REAL);
  REQUIRE(ODEntryContainer::getOCTbyODT(ObjectType::ARRAY, ObjectDataType::REAL64) == ObjectClassType::ARRAY_REAL);

  REQUIRE(ODEntryContainer::getOCTbyODT(ObjectType::VAR, ObjectDataType::VISIBLE_STRING) == ObjectClassType::STRING);
  REQUIRE(ODEntryContainer::getOCTbyODT(ObjectType::VAR, ObjectDataType::OCTET_STRING) == ObjectClassType::STRING);
  REQUIRE(ODEntryContainer::getOCTbyODT(ObjectType::VAR, ObjectDataType::UNICODE_STRING) == ObjectClassType::STRING);

  REQUIRE(ODEntryContainer::getOCTbyODT(ObjectType::VAR, ObjectDataType::IDENTITY) == ObjectClassType::COMPLEX);
}

TEST_CASE("Testing ODEntryInt", "[ODEntry]") {
  std::cout << "ODEntry container size: \x1b[33;1m" << EPL_DataCollect::internal::calcSize() << "\x1b[0m Byte"
            << std::endl;

  ODEntryContainer ct(ObjectDataType::INTEGER32);
  auto             ptr   = *ct;
  ODEntryInt *     t     = ct.getData<ODEntryInt>();
  ODEntry *        entry = *ct;
  t->setFromString("42");
  REQUIRE(t->data == 42);
  REQUIRE(t->toString() == "42");
  REQUIRE(t->getType() == ObjectClassType::INTEGER);
  REQUIRE(t->getType() == ct->getType());
  REQUIRE(t->getDataType() == ObjectDataType::INTEGER32);
  REQUIRE(t->getDataType() == entry->getDataType());
  REQUIRE(t->isNumericValue() == true);
  REQUIRE(checkDouble(t->getNumericValue(), 42) == true);
  REQUIRE(ptr == entry);
  REQUIRE(t->clone().get() != nullptr);

  // Test copying of complex stuff
  ODEntryContainer ct2(ct);
  ODEntryContainer ct3(std::move(ct));
  ct2 = ct3;
  ct3 = std::move(ct2);
}

TEST_CASE("Testing ODEntryUInt", "[ODEntry]") {
  ODEntryContainer ct(ObjectDataType::UNSIGNED16);
  ODEntryUInt &    t = *ct.getData<ODEntryUInt>();
  t.setFromString("0x2A");
  REQUIRE(t.data == 42);
  REQUIRE(t.toString() == "42");
  REQUIRE(t.getType() == ObjectClassType::UNSIGNED);
  REQUIRE(t.getDataType() == ObjectDataType::UNSIGNED16);
  REQUIRE(t.isNumericValue() == true);
  REQUIRE(checkDouble(t.getNumericValue(), 42) == true);
  REQUIRE(t.clone().get() != nullptr);

  ODEntryContainer ct2(ct);
  ODEntryContainer ct3(std::move(ct));

  // Test copying of complex stuff
  ct2 = ct3;
  ct3 = std::move(ct2);
}

TEST_CASE("Testing ODEntryBool", "[ODEntry]") {
  ODEntryContainer ct(ObjectDataType::BOOLEAN);
  ODEntryBool &    t = *ct.getData<ODEntryBool>();
  t.setFromString("True");
  REQUIRE(t.data == true);
  REQUIRE(t.toString() == "true");
  REQUIRE(t.getType() == ObjectClassType::BOOL);
  REQUIRE(t.getDataType() == ObjectDataType::BOOLEAN);
  REQUIRE(t.isNumericValue() == true);
  REQUIRE(checkDouble(t.getNumericValue(), 1) == true);
  REQUIRE(t.clone().get() != nullptr);

  // Test copying of complex stuff
  ODEntryContainer ct2(ct);
  ODEntryContainer ct3(std::move(ct));

  ct2 = ct3;
  ct3 = std::move(ct2);
}

TEST_CASE("Testing ODEntryReal", "[ODEntry]") {
  ODEntryContainer ct(ObjectDataType::REAL32);
  ODEntryReal &    t = *ct.getData<ODEntryReal>();
  t.setFromString("42");
  REQUIRE(checkDouble(t.data, 42) == true);
  REQUIRE(t.toString().substr(0, 2) == "42");
  REQUIRE(t.getType() == ObjectClassType::REAL);
  REQUIRE(t.getDataType() == ObjectDataType::REAL32);
  REQUIRE(t.isNumericValue() == true);
  REQUIRE(checkDouble(t.getNumericValue(), 42) == true);
  REQUIRE(t.clone().get() != nullptr);

  // Test copying of complex stuff
  ODEntryContainer ct2(ct);
  ODEntryContainer ct3(std::move(ct));

  ct2 = ct3;
  ct3 = std::move(ct2);
}

TEST_CASE("Testing ODEntryString", "[ODEntry]") {
  ODEntryContainer ct(ObjectDataType::VISIBLE_STRING);
  ODEntryString &  t = *ct.getData<ODEntryString>();
  t.setFromString("Hello world");
  REQUIRE(t.data == "Hello world");
  REQUIRE(t.toString() == "Hello world");
  REQUIRE(t.getType() == ObjectClassType::STRING);
  REQUIRE(t.getDataType() == ObjectDataType::VISIBLE_STRING);
  REQUIRE(t.isNumericValue() == false);
  REQUIRE(checkDouble(t.getNumericValue(), 0) == true);
  REQUIRE(t.clone().get() != nullptr);

  ODEntryString *t2;

  // Test copying of complex stuff
  ODEntryContainer ct2(ct);
  t2 = ct2.getData<ODEntryString>();
  REQUIRE(t2->data == "Hello world");
  ODEntryContainer ct3(std::move(ct));
  t2 = ct3.getData<ODEntryString>();
  REQUIRE(t2->data == "Hello world");

  ct2 = ct3;
  t2  = ct2.getData<ODEntryString>();
  REQUIRE(t2->data == "Hello world");
  ct3 = std::move(ct2);
  t2  = ct3.getData<ODEntryString>();
  REQUIRE(t2->data == "Hello world");
}

TEST_CASE("Testing ODEntryArrayInt", "[ODEntry]") {
  ODEntryContainer ct(ObjectDataType::INTEGER40, ObjectType::ARRAY);
  ODEntryArrayInt &t = *ct.getData<ODEntryArrayInt>();
  t.setFromString("11", 5);
  REQUIRE(t.data[5] == 11);
  REQUIRE(t.toString() != "");
  REQUIRE(t.getType() == ObjectClassType::ARRAY_INTEGER);
  REQUIRE(t.getDataType() == ObjectDataType::INTEGER40);
  REQUIRE(t.isNumericValue() == false);
  REQUIRE(checkDouble(t.getNumericValue(), 0) == true);
  REQUIRE(t.clone().get() != nullptr);

  // Test copying of complex stuff
  ODEntryContainer ct2(ct);
  ODEntryContainer ct3(std::move(ct));

  ct2 = ct3;
  ct3 = std::move(ct2);
}

TEST_CASE("Testing ODEntryArrayUInt", "[ODEntry]") {
  ODEntryContainer  ct(ObjectDataType::UNSIGNED56, ObjectType::ARRAY);
  ODEntryArrayUInt &t = *ct.getData<ODEntryArrayUInt>();
  t.setFromString("11", 5);
  REQUIRE(t.data[5] == 11);
  REQUIRE(t.toString() != "");
  REQUIRE(t.getType() == ObjectClassType::ARRAY_UNSIGNED);
  REQUIRE(t.getDataType() == ObjectDataType::UNSIGNED56);
  REQUIRE(t.isNumericValue() == false);
  REQUIRE(checkDouble(t.getNumericValue(), 0) == true);
  REQUIRE(t.clone().get() != nullptr);

  // Test copying of complex stuff
  ODEntryContainer ct2(ct);
  ODEntryContainer ct3(std::move(ct));

  ct2 = ct3;
  ct3 = std::move(ct2);
}

TEST_CASE("Testing ODEntryArrayBool", "[ODEntry]") {
  ODEntryContainer  ct(ObjectDataType::BOOLEAN, ObjectType::ARRAY);
  ODEntryArrayBool &t = *ct.getData<ODEntryArrayBool>();
  t.setFromString("TRUE", 5);
  REQUIRE(t.data[5] == true);
  REQUIRE(t.toString() != "");
  REQUIRE(t.getType() == ObjectClassType::ARRAY_BOOL);
  REQUIRE(t.getDataType() == ObjectDataType::BOOLEAN);
  REQUIRE(t.isNumericValue() == false);
  REQUIRE(checkDouble(t.getNumericValue(), 0) == true);
  REQUIRE(t.clone().get() != nullptr);

  // Test copying of complex stuff
  ODEntryContainer ct2(ct);
  ODEntryContainer ct3(std::move(ct));

  ct2 = ct3;
  ct3 = std::move(ct2);
}

TEST_CASE("Testing ODEntryArrayReal", "[ODEntry]") {
  ODEntryContainer  ct(ObjectClassType::ARRAY_REAL, ObjectDataType::VISIBLE_STRING);
  ODEntryArrayReal &t = *ct.getData<ODEntryArrayReal>();
  t.setFromString("11", 5);
  REQUIRE(checkDouble(t.data[5], 11) == true);
  REQUIRE(t.toString() != "");
  REQUIRE(t.getType() == ObjectClassType::ARRAY_REAL);
  REQUIRE(t.getDataType() == ObjectDataType::VISIBLE_STRING);
  REQUIRE(t.isNumericValue() == false);
  REQUIRE(checkDouble(t.getNumericValue(), 0) == true);
  REQUIRE(t.clone().get() != nullptr);

  // Test copying of complex stuff
  ODEntryContainer ct2(ct);
  ODEntryContainer ct3(std::move(ct));

  ct2 = ct3;
  ct3 = std::move(ct2);
}

TEST_CASE("Testing ODEntryComplex", "[ODEntry]") {
  ODEntryContainer ct(ObjectClassType::COMPLEX, ObjectDataType::VISIBLE_STRING);
  ODEntryComplex & t = *ct.getData<ODEntryComplex>();
  t.data[0].init(ObjectDataType::INTEGER64);
  t.setFromString("1", 11);
  t.setFromString("33", 0);
  REQUIRE(reinterpret_cast<ODEntryInt *>(*t.data[0])->data == 33);
  REQUIRE(t.toString() != "");
  REQUIRE(t.getType() == ObjectClassType::COMPLEX);
  REQUIRE(t.getDataType() == ObjectDataType::VISIBLE_STRING);
  REQUIRE(t.isNumericValue() == false);
  REQUIRE(checkDouble(t.getNumericValue(0), 33) == true);
  REQUIRE(checkDouble(t.getNumericValue(11), 1) == true);
  REQUIRE(checkDouble(t.getNumericValue(5), 0) == true);
  REQUIRE(t.clone().get() != nullptr);

  // Test copying of complex stuff
  ODEntryContainer ct2(ct);
  ODEntryContainer ct3(std::move(ct));

  ct2 = ct3;
  ct3 = std::move(ct2);
}
