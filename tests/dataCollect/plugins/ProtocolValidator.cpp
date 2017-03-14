
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

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wold-style-cast"

#include "CSViewFilters.hpp"
#include "DefaultFilter.hpp"
#include "PluginManager.hpp"
#include <CaptureInstance.hpp>
#include <Cycle.hpp>
#include <EvView.hpp>
#include <PythonPlugin.hpp>
#include <catch.hpp>
#include <fakeit.hpp>
#include <iostream>

#pragma clang diagnostic pop

#if __cplusplus <= 201402L
#include <ProtocolValidator.hpp>
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#else
#include <filesystem>
namespace fs = std::filesystem;
#endif

using namespace fakeit;
using namespace EPL_DataCollect;
using namespace EPL_DataCollect::plugins;

TEST_CASE("Protocol Validator Plugin", "[ProtocolValidator]") {
  std::cout << std::endl;
  CaptureInstance inst;
  auto            id              = inst.getEventLog()->getAppID();
  auto            validatorPlugin = std::make_shared<ProtocolValidator>();
  inst.getPluginManager()->addPlugin(validatorPlugin);

  std::string file = constants::EPL_DC_BUILD_DIR_ROOT + "/external/resources/pcaps/1CN-with-ObjectMapping-PDO.pcapng";
  fs::path    filePath(file);
  REQUIRE(fs::exists(filePath));
  REQUIRE(fs::is_regular_file(filePath));

  REQUIRE(inst.loadPCAP(file) == 0);

  inst.getCycleBuilder()->waitForLoopToFinish();
  auto events = inst.getEventLog()->pollEvents(id);
  std::cout << std::endl;
}
