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
 * \file DefaultFilter.hpp
 * \brief Contains class DefaultFilter
 * \todo IMPLEMENT
 */

#include "DefaultFilter.hpp"
#include "CSViewFilters.hpp"

namespace EPL_DataCollect {
namespace plugins {

using namespace constants;

static const uint16_t toInclude[] = {
      0x1000, 0x1001, 0x1002, 0x1003, 0x1004, 0x1005, 0x1006, 0x1007, 0x1008, 0x1009,
};

void DefaultFilter::run(Cycle *cycle) {
  CSViewFilters *filters   = dynamic_cast<CSViewFilters *>(cycle->getCycleStorage(EPL_DC_PLUGIN_VIEW_FILTERS_CSID));
  auto *         defFilter = filters->getFilter(filterID);

  for (auto const &i : cycle->getPackets())
    for (auto const &j : *i.getDiffs())
      defFilter->setIndex(j.getIndex());
}

std::string DefaultFilter::getDependencies() { return ""; }
std::string DefaultFilter::getID() { return EPL_DC_PLUGIN_TIME_SERIES_CSID; }

bool DefaultFilter::initialize(CaptureInstance *ci) {
  if (!ci->getStartCycle()->getCycleStorage(EPL_DC_PLUGIN_VIEW_FILTERS_CSID))
    if (!ci->registerCycleStorage<CSViewFilters>(EPL_DC_PLUGIN_VIEW_FILTERS_CSID))
      return false;

  CycleStorageBase *base    = ci->getStartCycle()->getCycleStorage(EPL_DC_PLUGIN_VIEW_FILTERS_CSID);
  CSViewFilters *   filters = dynamic_cast<CSViewFilters *>(base);

  if (!filters)
    return false;

  filterID        = filters->newFilter(CSViewFilters::INCLUDE, "Default");
  auto *defFilter = filters->getFilter(filterID);

  for (auto i : toInclude)
    defFilter->setIndex(i);

  return true;
}

bool DefaultFilter::reset(CaptureInstance *) { return true; }
}
}
