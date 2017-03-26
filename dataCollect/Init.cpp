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
/*!
 * \file Init.cpp
 * \brief Contains class Init
 */

#include "Init.hpp"
#include <epan/proto.h>
#include <iostream>
#include <ws_capture.h>
#include <ws_dissect.h>
#include <wsutil/filesystem.h>
#include <wsutil/privileges.h>

namespace EPL_DataCollect {

using namespace constants;

Init::Init(std::string pluginsDir, std::string wiresharkLibDir) {
  init_process_policies();

  if (started_with_special_privs() || running_with_special_privs()) {
    std::cout << "[Init] Program started with root privileges! This is dangerous ==> dropping them" << std::endl;
    relinquish_special_privs_perm();

    if (started_with_special_privs() || running_with_special_privs()) {
      std::cerr << "[Init] Failed to dropp special privileges! Start failed" << std::endl;

      errorCode = APPLICATION_STARTED_AS_ROOT;
      return;
    }
  }

  char *currentLD = getenv("LD_LIBRARY_PATH");
  if (currentLD) {
    std::string temp(currentLD);
    if (!temp.empty()) {
      wiresharkLibDir += ":";
      wiresharkLibDir += temp;
    }
  }

  auto ret0 = setenv("LD_LIBRARY_PATH", wiresharkLibDir.c_str(), 1);

  if (ws_dissect_plugin_dir(pluginsDir.c_str()) != TRUE) {
    std::cerr << "Failed to set the wireshark plugin dir" << std::endl;
    errorCode = SETTING_WS_PLUGIN_DIR_FAILED;
    return;
  }

  auto ret1 = ws_capture_init();
  auto ret2 = ws_dissect_init();
  auto ret3 = proto_name_already_registered(EPL_DC_PLUGIN_PROTO_NAME.c_str());

  std::string foundProto = ret3 != 0 ? "true" : "false";

  ws_dissect_proto_disable("epl"); // Disable old dissector

  std::cout << "[Init] Wireshark plugins dir:             " << pluginsDir << std::endl;
  std::cout << "[Init] new LD_LIBRARY_PATH:               " << wiresharkLibDir << std::endl;
  std::cout << "[Init] setenv returned:                   " << ret0 << std::endl;
  std::cout << "[Init] ws_capture_init returned:          " << ret1 << std::endl;
  std::cout << "[Init] ws_dissect_init returned:          " << ret2 << std::endl;
  std::cout << "[Init] Loaded the advanced EPL dissector: " << foundProto << std::endl;

  if (ret0 != 0 || ret1 != 0 || ret2 != 0) {
    errorCode = WIRESHARK_INIT_FAILED;
    return;
  }

  if (ret3 == 0) {
    errorCode = LOADING_DISECTOR_FAILED;
    return;
  }

  errorCode = OK;
}

Init::~Init() {
  std::cout << "[Init] finalizing wireshark" << std::endl;

  if (errorCode != OK)
    return;

  ws_capture_finalize();
  ws_dissect_finalize();
}
}
