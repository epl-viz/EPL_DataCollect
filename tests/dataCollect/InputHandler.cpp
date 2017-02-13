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

TEST_CASE("InputHandler parsing", "[InputHandler]") {
  InputHandler handler;
  std::string  file = constants::EPL_DC_BUILD_DIR_ROOT + "/external/resources/pcaps/EPL_Example.cap";

  fs::path filePath(file);
  REQUIRE(fs::exists(filePath));
  REQUIRE(fs::is_regular_file(filePath));

  ws_capture_t *capture = ws_capture_open_offline(file.c_str(), 0);
  REQUIRE(capture != nullptr);

  ws_dissect_t *dissect = ws_dissect_capture(capture);
  REQUIRE(dissect != nullptr);

  handler.setDissector(dissect);

  print_args_t    print_args;
  print_stream_t *print_stream = print_stream_text_stdio_new(stdout);

  memset(&print_args, 0, sizeof(print_args_t));
  print_args.print_hex         = FALSE;
  print_args.print_dissections = print_dissections_expanded;

  uint32_t      counter = 0;
  ws_dissection diss;
  while (ws_dissect_next(dissect, &diss)) {
    Packet packet = handler.parsePacket(&diss);
    std::cout << "\x1b[32;1mCOUNTER:\x1b[34;1m " << counter << "\x1b[m" << std::endl << packet.getMiscData();

    //     epan_dissect_t *edt = ws_dissect_epan_get_np(dissect);
    //     proto_tree_print(&print_args, edt, NULL, print_stream);

    counter++;
    if (counter >= 20)
      break;
  }

  ws_dissect_free(dissect);
  handler.setDissector(nullptr);
  ws_capture_close(capture);
}
