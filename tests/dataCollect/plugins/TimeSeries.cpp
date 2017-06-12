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

#include <CSTimeSeriesPtr.hpp>
#include <CaptureInstance.hpp>
#include <TimeSeries.hpp>
#include <TimeSeriesBuilder.hpp>
#include <iostream>

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

TEST_CASE("Testing TimeSeries plugin -- no data", "[plugin][TimeSeries]") {
  CaptureInstance ci;
  ci.getPluginManager()->addPlugin(std::make_shared<TimeSeriesBuilder>());

  auto ts = std::make_shared<TimeSeries>(0, 0, 0);
  ci.registerCycleStorage<CSTimeSeriesPtr>(EPL_DC_PLUGIN_TIME_SERIES_CSID);
  auto *           base = ci.getStartCycle()->getCycleStorage(EPL_DC_PLUGIN_TIME_SERIES_CSID);
  CSTimeSeriesPtr *csTS = dynamic_cast<CSTimeSeriesPtr *>(base);

  REQUIRE(base != nullptr);
  REQUIRE(csTS != nullptr);

  csTS->addTS(ts);

  std::string file = constants::EPL_DC_BUILD_DIR_ROOT + "/external/resources/pcaps/1CN-with-ObjectMapping-PDO.pcapng";
  fs::path    filePath(file);
  REQUIRE(fs::exists(filePath));
  REQUIRE(fs::is_regular_file(filePath));

  ci.loadPCAP(file);
  ci.getCycleBuilder()->waitForLoopToFinish();
}

TEST_CASE("Testing TimeSeries plugin -- with data", "[plugin][TimeSeries]") {
  CaptureInstance ci;
  ci.getPluginManager()->addPlugin(std::make_shared<TimeSeriesBuilder>());

  auto ts = std::make_shared<TimeSeries>(1, 0x6200, 1);
  ci.registerCycleStorage<CSTimeSeriesPtr>(EPL_DC_PLUGIN_TIME_SERIES_CSID);
  auto *           base = ci.getStartCycle()->getCycleStorage(EPL_DC_PLUGIN_TIME_SERIES_CSID);
  CSTimeSeriesPtr *csTS = dynamic_cast<CSTimeSeriesPtr *>(base);

  REQUIRE(base != nullptr);
  REQUIRE(csTS != nullptr);

  csTS->addTS(ts);

  std::string file = constants::EPL_DC_BUILD_DIR_ROOT + "/external/resources/pcaps/1CN-with-ObjectMapping-PDO.pcapng";
  fs::path    filePath(file);
  REQUIRE(fs::exists(filePath));
  REQUIRE(fs::is_regular_file(filePath));

  ci.loadPCAP(file);
  ci.getCycleBuilder()->waitForLoopToFinish();
  REQUIRE(ts->tsData.size() == 287);
}
