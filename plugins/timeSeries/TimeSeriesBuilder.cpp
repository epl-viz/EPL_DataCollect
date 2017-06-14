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
 * \file TimeSeriesBuilder.cpp
 * \brief Contains class TimeSeriesBuilder
 * \todo IMPLEMENT
 */

#include "TimeSeriesBuilder.hpp"
#include "CSTimeSeriesPtr.hpp"
#include <iostream>

namespace EPL_DataCollect {
namespace plugins {

using namespace constants;

TimeSeriesBuilder::~TimeSeriesBuilder() {}

void TimeSeriesBuilder::run(Cycle *cycle) {
  uint32_t          cycleNum = cycle->getCycleNum();
  CycleStorageBase *storage  = cycle->getCycleStorage(EPL_DC_PLUGIN_TIME_SERIES_CSID);
  CSTimeSeriesPtr * pointers = dynamic_cast<CSTimeSeriesPtr *>(storage);

  if (!pointers) {
    std::cerr << "[pl::TimeSeriesBuilder] invalid cycle storage! ID=" << EPL_DC_PLUGIN_TIME_SERIES_CSID << std::endl;
    return;
  }

  for (auto i : *pointers->getTsPTRs()) {
    if (i->isCustomEntry()) {
      i->addDataPoint(cycleNum, cycle->getCycleStorage(i->getCSID()));
    } else {
      ODEntry *entry = nullptr;
      auto *   node  = cycle->getNode(i->getNodeID());
      if (node) {
        entry = node->getOD()->getEntry(i->getIndex());
      }

      i->addDataPoint(cycleNum, entry);
    }
  }
}


std::string TimeSeriesBuilder::getDependencies() { return ""; }
std::string TimeSeriesBuilder::getID() { return EPL_DC_PLUGIN_TIME_SERIES_CSID; }

bool TimeSeriesBuilder::initialize(CaptureInstance *ci) {
  if (!ci->getStartCycle()->getCycleStorage(EPL_DC_PLUGIN_TIME_SERIES_CSID))
    return ci->registerCycleStorage<CSTimeSeriesPtr>(EPL_DC_PLUGIN_TIME_SERIES_CSID);

  return true;
}

bool TimeSeriesBuilder::reset(CaptureInstance *) { return true; }
}
}
