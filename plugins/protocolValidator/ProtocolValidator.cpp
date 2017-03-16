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
 * \file ProtocolValidator.cpp
 * \brief Contains class ProtocolValidator
 * \todo IMPLEMENT
 */

#include "ProtocolValidator.hpp"
#include "EvPluginText.hpp"
#include "Packet.hpp"
#include <EvView.hpp>
#include "EPLEnums.h"
#include <iostream>
#include "EPLEnum2Str.hpp"

#define PROTOCOL_VAL "ProtocolValidatorEvent"
#define CASE_NO_EPL_PACKET "Packet is not a EPL packet"
#define CASE_STATUS_CHANGE(id, former, later) "Node [" + id + "] changes state from\t[" + former + "] to state\t[" + later + "]"
#define CYCLE_TIME_ENTRY 0x1006
#define MN 240


namespace EPL_DataCollect {
namespace plugins {

ProtocolValidator::ProtocolValidator() {}

ProtocolValidator::~ProtocolValidator() {}

std::string ProtocolValidator::getID() { return pluginID; }

std::string ProtocolValidator::getDependencies() { return ""; }

bool ProtocolValidator::initialize(CaptureInstance *ci) {
  (void)ci;
  return registerCycleStorage<CSValidatorPluginStorage>(pluginID);
}

bool ProtocolValidator::reset(CaptureInstance *ci) {
  (void)ci;
  return true;

} // no reset necessary

void ProtocolValidator::run(Cycle *cycle) {
  for (auto packet : cycle->getPackets()) {
    // checking if it's a EPL packet in the first place
    if (packet.getType() == PacketType::UNDEF) {
      shootValidatorEvent(CASE_NO_EPL_PACKET, static_cast<uint64_t>(packet.getTime()), cycle);
    }

    auto *_nmtstati     = dynamic_cast<CSValidatorPluginStorage *>(cycle->getCycleStorage(pluginID))->getMap();

    // for each node check for status changes
    for (uint8_t nodeID : cycle->getNodeList()) {
      if ((*_nmtstati)[nodeID] != cycle->getNode(nodeID)->getStatus()) {
        std::string temp = CASE_STATUS_CHANGE(std::to_string(nodeID), EPLEnum2Str::toStr((*_nmtstati)[nodeID]),cycle->getNode(nodeID)->getStatusStr());
        shootValidatorEvent(temp, 0, cycle);
        (*_nmtstati)[nodeID] = cycle->getNode(nodeID)->getStatus();
      }
    }

    //std::cout << "entry is there" << cycle->getNode(nodeID)->getOD()->getEntry(CYCLE_TIME_ENTRY)->toString() << std::endl;




  }
}

void ProtocolValidator::shootValidatorEvent(std::string message, uint64_t flag, Cycle *cycle) {
  std::cout << "SHOOTING with {" << message << "}" << std::endl;
  addEvent(std::make_unique<EvPluginText>(
        getID(), std::string(PROTOCOL_VAL), message, flag, cycle, EventBase::INDEX_MAP()));
}
}
}
