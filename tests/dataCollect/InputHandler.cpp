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

#include "defines.hpp"
#include "EPLEnum2Str.hpp"
#include <InputHandler.hpp>
#include <catch.hpp>
#include <epan/print.h>
#include <epan/proto.h>
#include <iostream>
#include <ws_capture.h>
#include <ws_dissect.h>

#if __cplusplus <= 201402L
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#else
#include <filesystem>
namespace fs = std::filesystem;
#endif

using namespace EPL_DataCollect;


TEST_CASE("InputHandler parsing EPL_Example", "[InputHandler]") {
  InputHandler handler;
  std::string  file = constants::EPL_DC_BUILD_DIR_ROOT + "/external/resources/pcaps/EPL_Example.cap";

  auto cfg = handler.getConfig();
  handler.setConfig(cfg);

  fs::path filePath(file);
  REQUIRE(fs::exists(filePath));
  REQUIRE(fs::is_regular_file(filePath));

  ws_capture_t *capture = ws_capture_open_offline(file.c_str(), 0, nullptr, nullptr);
  REQUIRE(capture != nullptr);

  ws_dissect_t *dissect = ws_dissect_capture(capture);
  REQUIRE(dissect != nullptr);

  handler.setDissector(dissect);

  uint32_t      counter = 0;
  ws_dissection diss;
  while (ws_dissect_next(dissect, &diss, nullptr, nullptr)) {
    Packet packet = handler.parsePacket(&diss);

#if ENABLE_DEBUG_PRINT
    std::cout << "\x1b[32;1mCOUNTER:\x1b[34;1m " << counter << "\x1b[m" << std::endl << packet.getMiscData();
#endif

    counter++;
  }

  handler.setDissector(nullptr);
  ws_dissect_free(dissect);
  ws_capture_close(capture);
}

TEST_CASE("InputHandler parsing epl_sdo_udp", "[InputHandler]") {
  InputHandler handler;
  std::string  file = constants::EPL_DC_BUILD_DIR_ROOT + "/external/resources/pcaps/epl_sdo_udp.cap";

  fs::path filePath(file);
  REQUIRE(fs::exists(filePath));
  REQUIRE(fs::is_regular_file(filePath));

  ws_capture_t *capture = ws_capture_open_offline(file.c_str(), 0, nullptr, nullptr);
  REQUIRE(capture != nullptr);

  ws_dissect_t *dissect = ws_dissect_capture(capture);
  REQUIRE(dissect != nullptr);

  handler.setDissector(dissect);

  uint32_t      counter = 0;
  ws_dissection diss;
  while (ws_dissect_next(dissect, &diss, nullptr, nullptr)) {
    Packet packet = handler.parsePacket(&diss);

#if ENABLE_DEBUG_PRINT
    std::cout << "\x1b[32;1mCOUNTER:\x1b[34;1m " << counter << "\x1b[m" << std::endl << packet.getMiscData();
#endif

    counter++;
  }

  handler.setDissector(nullptr);
  ws_dissect_free(dissect);
  ws_capture_close(capture);
}

TEST_CASE("InputHandler parsing 1CN", "[InputHandler]") {
  InputHandler handler;
  std::string  file = constants::EPL_DC_BUILD_DIR_ROOT + "/external/resources/pcaps/1CN.pcapng";

  fs::path filePath(file);
  REQUIRE(fs::exists(filePath));
  REQUIRE(fs::is_regular_file(filePath));

  ws_capture_t *capture = ws_capture_open_offline(file.c_str(), 0, nullptr, nullptr);
  REQUIRE(capture != nullptr);

  ws_dissect_t *dissect = ws_dissect_capture(capture);
  REQUIRE(dissect != nullptr);

  handler.setDissector(dissect);

  uint32_t      counter = 0;
  ws_dissection diss;
  while (ws_dissect_next(dissect, &diss, nullptr, nullptr)) {
    Packet packet = handler.parsePacket(&diss);

#if ENABLE_DEBUG_PRINT
    std::cout << "\x1b[32;1mCOUNTER:\x1b[34;1m " << counter << "\x1b[m" << std::endl << packet.getMiscData();
#endif

    counter++;
  }

  handler.setDissector(nullptr);
  ws_dissect_free(dissect);
  ws_capture_close(capture);
}

TEST_CASE("InputHandler parsing 1CN-with-ObjectMapping-PDO", "[InputHandler]") {
  InputHandler handler;
  std::string  file = constants::EPL_DC_BUILD_DIR_ROOT + "/external/resources/pcaps/1CN-with-ObjectMapping-PDO.pcapng";

  fs::path filePath(file);
  REQUIRE(fs::exists(filePath));
  REQUIRE(fs::is_regular_file(filePath));

  ws_capture_t *capture = ws_capture_open_offline(file.c_str(), 0, nullptr, nullptr);
  REQUIRE(capture != nullptr);

  ws_dissect_t *dissect = ws_dissect_capture(capture);
  REQUIRE(dissect != nullptr);

  handler.setDissector(dissect);

  uint32_t      counter = 0;
  ws_dissection diss;
  while (ws_dissect_next(dissect, &diss, nullptr, nullptr)) {
    Packet packet = handler.parsePacket(&diss);

#if ENABLE_DEBUG_PRINT
    std::cout << "\x1b[32;1mCOUNTER:\x1b[34;1m " << counter << "\x1b[m" << std::endl << packet.getMiscData();
#endif

    counter++;
  }

  ws_dissect_free(dissect);
  handler.setDissector(nullptr);
  ws_capture_close(capture);
}


TEST_CASE("InputHandler Cycle parsing", "[InputHandler]") {
  InputHandler handler;
  std::string  file = constants::EPL_DC_BUILD_DIR_ROOT + "/external/resources/pcaps/EPL_Example.cap";

  fs::path filePath(file);
  REQUIRE(fs::exists(filePath));
  REQUIRE(fs::is_regular_file(filePath));

  ws_capture_t *capture = ws_capture_open_offline(file.c_str(), 0, nullptr, nullptr);
  REQUIRE(capture != nullptr);

  ws_dissect_t *dissect = ws_dissect_capture(capture);
  REQUIRE(dissect != nullptr);

  handler.setDissector(dissect);

  InputHandler::CompletedCycle cd(0);
  cd.num = 0;
  REQUIRE(handler.parseCycle(&cd) == true);
  REQUIRE((cd.flags & InputHandler::DONE) != 0);
  REQUIRE((cd.flags & InputHandler::USED_SEEK) == 0);
  REQUIRE((cd.flags & InputHandler::ERROR) == 0);
  REQUIRE(cd.packets.size() == 11);
  REQUIRE(cd.packets[0].getType() == PacketType::START_OF_ASYNC);
  REQUIRE(cd.packets[5].getType() == PacketType::ASYNC_SEND);

  cd.flags = 0;
  cd.num   = 1;
  REQUIRE(handler.parseCycle(&cd) == true);
  REQUIRE((cd.flags & InputHandler::DONE) != 0);
  REQUIRE((cd.flags & InputHandler::USED_SEEK) == 0);
  REQUIRE((cd.flags & InputHandler::ERROR) == 0);
  REQUIRE(cd.packets.size() == 2);
  REQUIRE(cd.packets[0].getType() == PacketType::START_OF_CYCLE);
  REQUIRE(cd.packets[0].SoC.get() != nullptr);
  REQUIRE(cd.packets[0].SoC->RelativeTime == 0);

  cd.flags = 0;
  cd.num   = 2;
  REQUIRE(handler.parseCycle(&cd) == true);
  REQUIRE((cd.flags & InputHandler::DONE) != 0);
  REQUIRE((cd.flags & InputHandler::USED_SEEK) == 0);
  REQUIRE((cd.flags & InputHandler::ERROR) == 0);
  REQUIRE(cd.packets.size() == 2);
  REQUIRE(cd.num == 2);
  REQUIRE(cd.packets[0].getType() == PacketType::START_OF_CYCLE);
  REQUIRE(cd.packets[1].getType() == PacketType::START_OF_ASYNC);

  cd.flags = 0;
  cd.num   = 4;
  REQUIRE(handler.parseCycle(&cd) == false);
  REQUIRE((cd.flags & InputHandler::DONE) != 0);
  REQUIRE((cd.flags & InputHandler::USED_SEEK) == 0);
  REQUIRE((cd.flags & InputHandler::ERROR) != 0);

  cd.flags = 0;
  cd.num   = 3;
  REQUIRE(handler.parseCycle(&cd) == true);
  REQUIRE((cd.flags & InputHandler::DONE) != 0);
  REQUIRE((cd.flags & InputHandler::USED_SEEK) == 0);
  REQUIRE((cd.flags & InputHandler::ERROR) == 0);
  REQUIRE(cd.packets.size() == 3);
  REQUIRE(cd.packets[0].getType() == PacketType::START_OF_CYCLE);

  cd.flags = 0;
  cd.num   = 4;
  REQUIRE(handler.parseCycle(&cd) == true);
  REQUIRE((cd.flags & InputHandler::DONE) != 0);
  REQUIRE((cd.flags & InputHandler::USED_SEEK) == 0);
  REQUIRE((cd.flags & InputHandler::ERROR) == 0);
  REQUIRE(cd.packets.size() == 3);
  REQUIRE(cd.packets[0].getType() == PacketType::START_OF_CYCLE);
  REQUIRE(cd.packets[2].getType() == PacketType::ASYNC_SEND);
  REQUIRE(cd.packets[2].ASnd.get() != nullptr);
  REQUIRE(cd.packets[2].ASnd->RequestedServiceID == ASndServiceID::IDENT_RESPONSE);
  REQUIRE(cd.packets[2].IdentResponse.get() != nullptr);
  REQUIRE(cd.packets[2].IdentResponse->Profile == 401);
  REQUIRE(cd.packets[2].IdentResponse->VendorId == 0x0100006C);

  cd.flags = 0;
  cd.num   = 2;
  REQUIRE(handler.parseCycle(&cd) == true);
  REQUIRE((cd.flags & InputHandler::DONE) != 0);
  REQUIRE((cd.flags & InputHandler::USED_SEEK) != 0);
  REQUIRE((cd.flags & InputHandler::ERROR) == 0);
  REQUIRE(cd.packets.size() == 2);
  REQUIRE(cd.num == 2);
  REQUIRE(cd.packets[0].getType() == PacketType::START_OF_CYCLE);
  REQUIRE(cd.packets[1].getType() == PacketType::START_OF_ASYNC);

  ws_dissect_free(dissect);
  handler.setDissector(nullptr);
  ws_capture_close(capture);
}


TEST_CASE("InputHandler Cycle run", "[InputHandler]") {
  InputHandler handler;
  std::string  file = constants::EPL_DC_BUILD_DIR_ROOT + "/external/resources/pcaps/EPL_Example.cap";

  fs::path filePath(file);
  REQUIRE(fs::exists(filePath));
  REQUIRE(fs::is_regular_file(filePath));

  ws_capture_t *capture = ws_capture_open_offline(file.c_str(), 0, nullptr, nullptr);
  REQUIRE(capture != nullptr);

  ws_dissect_t *dissect = ws_dissect_capture(capture);
  REQUIRE(dissect != nullptr);

  REQUIRE(handler.getPacketsMetadata()->empty());
  handler.setDissector(dissect);
  REQUIRE(!handler.getPacketsMetadata()->empty());
  handler.startLoop();

  std::vector<Packet> packets;
  packets = handler.getCyclePackets(0);
  REQUIRE(packets.size() == 11);
  REQUIRE(packets[0].getType() == PacketType::START_OF_ASYNC);
  uint32_t i = 1;
  for (;;) {
    packets = handler.getCyclePackets(i);

    if (packets.empty())
      break;

    REQUIRE(packets.size() > 1);
    REQUIRE(packets[0].getType() == PacketType::START_OF_CYCLE);
    ++i;
  }

  REQUIRE(i == 249);

  packets = handler.getCyclePackets(4);
  REQUIRE(packets.size() == 3);
  REQUIRE(packets[0].getType() == PacketType::START_OF_CYCLE);
  REQUIRE(packets[1].getType() == PacketType::START_OF_ASYNC);
  REQUIRE(packets[2].getType() == PacketType::ASYNC_SEND);

  handler.stopLoop();
  handler.setDissector(nullptr);

  REQUIRE(handler.getIsLoopRunning() == false);

  ws_dissect_free(dissect);
  ws_capture_close(capture);
}

TEST_CASE("PacketMetadata bit field", "[InputHandler][temp]") {
  InputHandler::PacketMetadata d;

  d.writeFiled(0, 0x01);
  d.writeFiled(1, 0xFF);
  d.writeFiled(2, 0x10);
  d.writeFiled(3, 0x12);
  d.writeFiled(4, 0x13);
  d.writeFiled(5, 0x14);
  d.writeFiled(6, 0x15);
  d.writeFiled(7, 0x00);
  d.writeFiled(8, 0xFF);
  REQUIRE(d.getFiled(0) == 0x01);
  REQUIRE(d.getFiled(1) == 0xFF);
  REQUIRE(d.getFiled(2) == 0x10);
  REQUIRE(d.getFiled(3) == 0x12);
  REQUIRE(d.getFiled(4) == 0x13);
  REQUIRE(d.getFiled(5) == 0x14);
  REQUIRE(d.getFiled(6) == 0x15);
  REQUIRE(d.getFiled(7) == 0x00);
  REQUIRE(d.getFiled(8) == 0);

  d.writeFiled(1, 0x42);
  REQUIRE(d.getFiled(0) == 0x01);
  REQUIRE(d.getFiled(1) == 0x42);
  REQUIRE(d.getFiled(0) == 0x01);
  REQUIRE(d.getFiled(2) == 0x10);
}
