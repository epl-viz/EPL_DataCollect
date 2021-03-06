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

#define CATCH_CONFIG_RUNNER
#include <catch.hpp>

#include "Init.hpp"
#include "PythonInit.hpp"
#include <CycleBuilder.hpp>
#include "Python.h"

using namespace EPL_DataCollect;
using namespace EPL_DataCollect::constants;
using namespace EPL_DataCollect::plugins;

int main(int argc, char *argv[]) {
  Init init;
  if (init.getIsOK() != Init::OK)
    return 1;

  PythonInit pyInit(argc, argv);
  pyInit.addPath(EPL_DC_BUILD_DIR_ROOT + "/python/plugins");
  pyInit.addPath(EPL_DC_BUILD_DIR_ROOT + "/python/plugins/tests");
  pyInit.addPath(EPL_DC_BUILD_DIR_ROOT + "/python/plugins/tests/loadtests");
  pyInit.addPath(EPL_DC_BUILD_DIR_ROOT + "/python/plugins/tests/cycleaccesstests");
  pyInit.addPath(EPL_DC_BUILD_DIR_ROOT + "/python/plugins/tests/pluginguiapitests");
  pyInit.addPath(EPL_DC_BUILD_DIR_ROOT + "/python/plugins/tests/pluginapitests");
  pyInit.addPath(EPL_DC_BUILD_DIR_ROOT + "/python/plugins/tests/pluginclasstests");
  pyInit.addPath(EPL_DC_BUILD_DIR_ROOT + "/python/plugins/samples");

  Catch::Session session;

  int returnCode = session.applyCommandLine(argc, argv);
  if (returnCode != 0) // Indicates a command line error
    return returnCode;

  return session.run();
}
