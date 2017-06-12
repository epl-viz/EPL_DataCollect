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

#include <catch.hpp>

#include <CaptureInstance.hpp>
#include <DefaultFilter.hpp>
#include "EPLEnums.h"

#if __cplusplus <= 201402L
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#else
#include <filesystem>
namespace fs = std::filesystem;
#endif

using namespace EPL_DataCollect;
using namespace EPL_DataCollect::plugins;
using namespace EPL_DataCollect::constants;

TEST_CASE("Testing DefaultFilter", "[plugin][filter]") {
  CaptureInstance ci;
  ci.getPluginManager()->addPlugin(std::make_shared<DefaultFilter>());

  std::string file = constants::EPL_DC_BUILD_DIR_ROOT + "/external/resources/pcaps/1CN-with-ObjectMapping-PDO.pcapng";
  fs::path    filePath(file);
  REQUIRE(fs::exists(filePath));
  REQUIRE(fs::is_regular_file(filePath));

  ci.loadPCAP(file);
  ci.getCycleBuilder()->waitForLoopToFinish();

  Cycle             c   = ci.getCycleContainer()->pollCycle();
  CycleStorageBase *b   = c.getCycleStorage(EPL_DC_PLUGIN_VIEW_FILTERS_CSID);
  CSViewFilters *   csF = dynamic_cast<CSViewFilters *>(b);

  REQUIRE(b != nullptr);
  REQUIRE(csF != nullptr);

  REQUIRE(csF->isNumericValue() == false);
  REQUIRE(csF->getNumericValue() <= std::numeric_limits<double>::epsilon());

  auto filters = csF->getFilters();
  REQUIRE(filters.size() == 1);

  auto f = *filters.begin();
  REQUIRE(f.getName() == "Default");
  REQUIRE(f.getType() == FilterType::INCLUDE);
  REQUIRE(f.includeIndex(0x1001) == true);
  f.unSetIndex(0x1001);
  REQUIRE(f.includeIndex(0x1001) == false);
  REQUIRE(f.includeIndex(0x5000) == false);
  REQUIRE(f.isSet(0x5000) == false);
  REQUIRE(f.includeIndex(0x6000) == true);
  REQUIRE(f.isSet(0x6000) == true);

  // CSViewFilters is movable
  CSViewFilters::Filter csFMoveFrom(FilterType::INCLUDE, "Move test");
  CSViewFilters::Filter csF2(std::move(csFMoveFrom));
  REQUIRE(csF2.getName() == "Move test");
  REQUIRE(csF2.getType() == FilterType::INCLUDE);
}
