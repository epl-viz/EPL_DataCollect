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
 * \file InputParser.cpp
 * \brief Contains functions used by class InputHandler
 */

#include "InputParser.hpp"
#include <epan/address_types.h>
#include <epan/print.h>
#include <epan/proto.h>
#include <iomanip> //! \todo Remove debug code
#include <iostream>
#include <sstream> //! \todo Remove debug code

namespace EPL_DataCollect {
namespace WiresharkParser {

//! \todo Consider other function
// https://softwareengineering.stackexchange.com/questions/49550/which-hashing-algorithm-is-best-for-uniqueness-and-speed/

#if ENABLE_DEBUG_PRINT
void debugFN(parserData *d, field_info *fi, std::string str) {
  gchar label_str[ITEM_LABEL_LENGTH];
  proto_item_fill_label(fi, label_str);

  std::stringstream stream;
  auto              t    = d->currentLevel > 0 ? "\x1b[1;33m" : "";
  auto              todo = str == "TODO" ? "\x1b[1;32m= \x1b[1;33m" : "\x1b[1;32m= \x1b[1;36m";
  stream << t << d->currentLevel << "\x1b[m; \x1b[31m" << std::setfill(' ') << std::setw(17) << std::left
         << EPLEnum2Str::toStr(fi->hfinfo->type) << todo << std::setw(25) << str << "\x1b[m -- "
         << std::setw(4 * d->currentLevel) << "" << label_str << std::endl;

  d->wsOther += stream.str();
}
#endif

// Bind Functions

void bindUINT8(parserData *d, field_info *fi, uint8_t &val) {
  if (fi->hfinfo->type != FT_UINT8) {
    std::cerr << "[WS Parser] bindUINT8: '" << fi->hfinfo->name << "' is not FT_UINT8 but "
              << EPLEnum2Str::toStr(fi->hfinfo->type) << std::endl;
    return;
  }

  val = static_cast<uint8_t>(fi->value.value.uinteger64);
  DPRINT(d, fi, std::to_string(val), "");
}

void bindBOOL(parserData *d, field_info *fi, bool &val) {
  if (fi->hfinfo->type != FT_BOOLEAN) {
    std::cerr << "[WS Parser] bindBOOL: '" << fi->hfinfo->name << "' is not field_info but "
              << EPLEnum2Str::toStr(fi->hfinfo->type) << std::endl;
    return;
  }

  val = fi->value.value.uinteger64 == 0 ? false : true;
  DPRINT(d, fi, val ? "True" : "False", "");
}


void foreachEPLFunc(proto_tree *node, gpointer data) {
  parserData *d  = reinterpret_cast<parserData *>(data);
  field_info *fi = node->finfo;


  if (!fi || !fi->hfinfo)
    return;

  header_field_info *hi = fi->hfinfo;

  if (!hi->name || !hi->abbrev)
    return;

  switch (jenkinsHash(hi->abbrev)) {
#if 0
    case "MessageType"_h: bindEnum(d, fi, d->pType); break;
    case "Destination"_h: bindUINT8(d, fi, d->dst); break;
    case "Source"_h: bindUINT8(d, fi, d->src); break;
    case "Captured Size"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.payload.capture_size" == FT_UINT16 -- BASE_DEC


    // IGNORE
    case "Node"_h: // "epl-xdd.node" == FT_UINT8 -- BASE_DEC_HEX
      break;

    /* SoC data fields*/
    case "Flags"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.soc" == FT_UINT8 -- BASE_HEX
    case "MC (Multiplexed Cycle Completed)"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.soc.mc" == FT_BOOLEAN -- 8
    case "PS (Prescaled Slot)"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.soc.ps" == FT_BOOLEAN -- 8
    case "NetTime"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.soc.nettime" == FT_ABSOLUTE_TIME -- ABSOLUTE_TIME_LOCAL
    case "RelativeTime"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.soc.relativetime" == FT_UINT64 -- BASE_DEC

    /* PReq data fields*/
    // case "Flags"_h:
    //   DPRINT(d, fi, "TODO");
    //   break; // "epl-xdd.preq" == FT_UINT8 -- BASE_HEX
    case "MS (Multiplexed Slot)"_h: bindBOOL(d, fi, d->multiplexedSlot); break;
    case "EA (Exception Acknowledge)"_h: bindBOOL(d, fi, d->exceptionAcknoledged); break;
    case "RD (Ready)"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.preq.rd" == FT_BOOLEAN -- 8
    case "PDOVersion"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.preq.pdov" == FT_UINT8 -- BASE_CUSTOM
    case "Size"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.preq.size" == FT_UINT16 -- BASE_DEC

    /* PRes data fields*/
    case "NMTStatus"_h:
      bindEnum(d, fi, d->nmtState);
      break;
    // case "Flags"_h:
    //   DPRINT(d, fi, "TODO");
    //   break; // "epl-xdd.pres" == FT_UINT8 -- BASE_HEX
    // case "MS (Multiplexed Slot)"_h:
    //   DPRINT(d, fi, "TODO");
    //   break; // "epl-xdd.pres.ms" == FT_BOOLEAN -- 8
    case "EN (Exception New)"_h:
      bindBOOL(d, fi, d->exceptionNew);
      break;
    // case "RD (Ready)"_h:
    //   DPRINT(d, fi, "TODO");
    //   break; // "epl-xdd.pres.rd" == FT_BOOLEAN -- 8
    case "PR (Priority)"_h: bindEnum(d, fi, d->sendPriority); break;
    case "RS (RequestToSend)"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.pres.rs" == FT_UINT8 -- BASE_DEC
    // case "PDOVersion"_h:
    //   DPRINT(d, fi, "TODO");
    //   break; // "epl-xdd.pres.pdov" == FT_STRING -- BASE_NONE
    // case "Size"_h:
    //   DPRINT(d, fi, "TODO");
    //   break; // "epl-xdd.pres.size" == FT_UINT16 -- BASE_DEC

    /* SoA data fields*/
    // case "NMTStatus"_h: break; // "epl-xdd.soa.stat" == FT_UINT8 -- BASE_HEX
    // case "EA (Exception Acknowledge)"_h: break; // "epl-xdd.soa.ea" == FT_BOOLEAN -- 8
    case "ER (Exception Reset)"_h: bindBOOL(d, fi, d->exceptionReset); break;
    case "RequestedServiceID"_h: bindEnum(d, fi, d->soaID); break;
    case "RequestedServiceTarget"_h: bindUINT8(d, fi, d->requestedServiceTarget); break;
    case "EPLVersion"_h: bindUINT8(d, fi, d->eplVersion); break;
    case "SyncControl"_h: bindUINT8(d, fi, d->syncControl); break;
    case "DestMacAddressValid"_h: bindBOOL(d, fi, d->destMacAddressValid); break;
    case "PResFallBackTimeoutValid"_h: bindBOOL(d, fi, d->PResFallBackTimeoutValid); break;
    case "SyncMNDelaySecondValid"_h: bindBOOL(d, fi, d->SyncMNDelaySecondValid); break;
    case "SyncMNDelayFirstValid"_h: bindBOOL(d, fi, d->SyncMNDelayFirstValid); break;
    case "PResTimeSecondValid"_h: bindBOOL(d, fi, d->PResTimeSecondValid); break;
    case "PResTimeFirstValid"_h: bindBOOL(d, fi, d->PResTimeFirstValid); break;
    case "PResModeSet"_h: bindBOOL(d, fi, d->PResModeSet); break;
    case "PResModeReset"_h: bindBOOL(d, fi, d->PResModeReset); break;
    case "DestMacAddress"_h:
      bindBOOL(d, fi, d->destMacAddress);
      break;
    // case "PResFallBackTimeoutValid"_h: break; // "epl-xdd.soa.tm.end" == FT_UINT8 -- BASE_DEC
    // case "SyncMNDelaySecondValid"_h: break; // "epl-xdd.soa.mnsc.end" == FT_UINT8 -- BASE_DEC
    // case "SyncMNDelayFirstValid"_h: break; // "epl-xdd.soa.mnft.end" == FT_UINT8 -- BASE_DEC
    // case "PResTimeSecondValid"_h: break; // "epl-xdd.soa.prsc.end" == FT_UINT8 -- BASE_DEC
    // case "PResTimeFirstValid"_h: break; // "epl-xdd.soa.prft.end" == FT_UINT8 -- BASE_DEC
    case "AN (Global)"_h: bindBOOL(d, fi, d->ANGlobal); break;
    case "AN (Local)"_h:
      bindBOOL(d, fi, d->ANLocal);
      break;

    /* ASnd header */
    case "Requested Service ID"_h: bindEnum(d, fi, d->sID); break;
    case "Requested Service Target"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.svtg" == FT_UINT8 -- BASE_DEC
    case "Data"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.data" == FT_BYTES -- BASE_NONE

    /* ASnd-->IdentResponse */
    // case "EN (Exception New)"_h: break; // "epl-xdd.asnd.ires.en" == FT_BOOLEAN -- 8
    case "EC (Exception Clear)"_h:
      bindBOOL(d, fi, d->exceptionClear);
      break;
    // case "PR (Priority)"_h: break; // "epl-xdd.asnd.ires.pr" == FT_UINT8 -- BASE_DEC
    // case "RS (RequestToSend)"_h:
    //   DPRINT(d, fi, "TODO");
    //   break; // "epl-xdd.asnd.ires.rs" == FT_UINT8 -- BASE_DEC
    // case "NMTStatus"_h:
    //   DPRINT(d, fi, "TODO");
    //   break; // "epl-xdd.asnd.ires.state" == FT_UINT8 -- BASE_HEX
    // case "EPLVersion"_h: break; // "epl-xdd.asnd.ires.eplver" == FT_UINT8 -- BASE_CUSTOM
    case "FeatureFlags"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.ires.features" == FT_UINT32 -- BASE_HEX
    case "Isochronous"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.ires.features.bit0" == FT_BOOLEAN -- 32
    case "SDO by UDP/IP"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.ires.features.bit1" == FT_BOOLEAN -- 32
    case "SDO by ASnd"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.ires.features.bit2" == FT_BOOLEAN -- 32
    case "SDO by PDO"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.ires.features.bit3" == FT_BOOLEAN -- 32
    case "NMT Info Services"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.ires.features.bit4" == FT_BOOLEAN -- 32
    case "Ext. NMT State Commands"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.ires.features.bit5" == FT_BOOLEAN -- 32
    case "Dynamic PDO Mapping"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.ires.features.bit6" == FT_BOOLEAN -- 32
    case "NMT Service by UDP/IP"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.ires.features.bit7" == FT_BOOLEAN -- 32
    case "Configuration Manager"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.ires.features.bit8" == FT_BOOLEAN -- 32
    case "Multiplexed Access"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.ires.features.bit9" == FT_BOOLEAN -- 32
    case "NodeID setup by SW"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.ires.features.bitA" == FT_BOOLEAN -- 32
    case "MN Basic Ethernet Mode"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.ires.features.bitB" == FT_BOOLEAN -- 32
    case "Routing Type 1 Support"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.ires.features.bitC" == FT_BOOLEAN -- 32
    case "Routing Type 2 Support"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.ires.features.bitD" == FT_BOOLEAN -- 32
    case "SDO Read/Write All"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.ires.features.bitE" == FT_BOOLEAN -- 32
    case "SDO Read/Write Multiple"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.ires.features.bitF" == FT_BOOLEAN -- 32
    case "Multiple-ASend Support"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.ires.features.bit10" == FT_BOOLEAN -- 32
    case "Ring Redundancy"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.ires.features.bit11" == FT_BOOLEAN -- 32
    case "PResChaining"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.ires.features.bit12" == FT_BOOLEAN -- 32
    case "Multiple PReq/PRes"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.ires.features.bit13" == FT_BOOLEAN -- 32
    case "Dynamic Node Allocation"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.ires.features.bit14" == FT_BOOLEAN -- 32
    case "MTU"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.ires.mtu" == FT_UINT16 -- BASE_DEC
    case "PollInSize"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.ires.pollinsize" == FT_UINT16 -- BASE_DEC
    case "PollOutSize"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.ires.polloutsizes" == FT_UINT16 -- BASE_DEC
    case "ResponseTime"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.ires.resptime" == FT_UINT32 -- BASE_DEC
    case "DeviceType"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.ires.devicetype" == FT_STRING -- BASE_NONE
    case "Profile"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.ires.profile" == FT_UINT16 -- BASE_DEC
    case "VendorId"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.ires.vendorid" == FT_UINT32 -- BASE_DEC_HEX
    case "ProductCode"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.ires.productcode" == FT_UINT32 -- BASE_DEC_HEX
    case "RevisionNumber"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.ires.revisionno" == FT_UINT32 -- BASE_DEC_HEX
    case "SerialNumber"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.ires.serialno" == FT_UINT32 -- BASE_DEC_HEX
    case "VendorSpecificExtension1"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.ires.vendorext1" == FT_UINT64 -- BASE_DEC_HEX
    case "VerifyConfigurationDate"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.ires.confdate" == FT_UINT32 -- BASE_DEC_HEX
    case "VerifyConfigurationTime"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.ires.conftime" == FT_UINT32 -- BASE_DEC_HEX
    case "ApplicationSwDate"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.ires.appswdate" == FT_UINT32 -- BASE_DEC_HEX
    case "ApplicationSwTime"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.ires.appswtime" == FT_UINT32 -- BASE_DEC_HEX
    case "IPAddress"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.ires.ip" == FT_IPv4 -- BASE_NONE
    case "SubnetMask"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.ires.subnet" == FT_IPv4 -- BASE_NETMASK
    case "DefaultGateway"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.ires.gateway" == FT_IPv4 -- BASE_NONE
    case "HostName"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.ires.hostname" == FT_STRING -- BASE_NONE
    case "VendorSpecificExtension2"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.ires.vendorext2" == FT_BYTES -- BASE_NONE

    /* ASnd-->StatusResponse */
    // case "EN (Exception New)"_h:
    //   DPRINT(d, fi, "TODO");
    //   break; // "epl-xdd.asnd.sres.en" == FT_BOOLEAN -- 8
    // case "EC (Exception Clear)"_h:
    //   DPRINT(d, fi, "TODO");
    //   break; // "epl-xdd.asnd.sres.ec" == FT_BOOLEAN -- 8
    // case "PR (Priority)"_h:
    //   DPRINT(d, fi, "TODO");
    //   break; // "epl-xdd.asnd.sres.pr" == FT_UINT8 -- BASE_DEC
    // case "RS (RequestToSend)"_h:
    //   DPRINT(d, fi, "TODO");
    //   break; // "epl-xdd.asnd.sres.rs" == FT_UINT8 -- BASE_DEC
    // case "NMTStatus"_h:
    //   DPRINT(d, fi, "TODO");
    //   break; // "epl-xdd.asnd.sres.stat" == FT_UINT8 -- BASE_HEX

    /* ASnd-->SyncResponse */
    case "SyncResponse"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.syncresponse.sync" == FT_UINT8 -- BASE_HEX
    // case "PResTimeFirstValid"_h:
    //   DPRINT(d, fi, "TODO");
    //   break; // "epl-xdd.asnd.syncresponse.fst.val" == FT_BOOLEAN -- 8
    // case "PResTimeSecondValid"_h:
    //   DPRINT(d, fi, "TODO");
    //   break; // "epl-xdd.asnd.syncresponse.sec.val" == FT_BOOLEAN -- 8
    case "PResModeStatus"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.syncresponse.mode" == FT_BOOLEAN -- 8
    case "Latency"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.syncresponse.latency" == FT_UINT8 -- BASE_DEC
    case "SyncDelayStation"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.syncresponse.delay.station" == FT_UINT8 -- BASE_DEC
    case "SyncDelay"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.syncresponse.delay.station" == FT_UINT8 -- BASE_DEC
    case "PResTimeFirst"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.syncresponse.pres.fst" == FT_UINT8 -- BASE_DEC
    case "PResTimeSecond"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.syncresponse.pres.sec" == FT_UINT8 -- BASE_DEC
    case "StaticErrorBitField"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.sres.seb" == FT_BYTES -- BASE_NONE

    /*StaticErrorBitField */
    case "Generic error"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.res.seb.bit0" == FT_UINT8 -- BASE_DEC
    case "Current"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.res.seb.bit1" == FT_UINT8 -- BASE_DEC
    case "Voltage"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.res.seb.bit2" == FT_UINT8 -- BASE_DEC
    case "Temperature"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.res.seb.bit3" == FT_UINT8 -- BASE_DEC
    case "Communication error"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.res.seb.bit4" == FT_UINT8 -- BASE_DEC
    case "Device Profile Spec"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.res.seb.bit5" == FT_UINT8 -- BASE_DEC
    case "Manufacturer Spec"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.res.seb.bit7" == FT_UINT8 -- BASE_DEC
    // case "Device Profile Spec"_h:
    //   DPRINT(d, fi, "TODO");
    //   break; // "epl-xdd.asnd.res.seb.devicespecific_err" == FT_BYTES -- BASE_NONE

    case "ErrorCodesList"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.sres.el" == FT_BYTES -- BASE_NONE
    case "Entry"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.sres.el.entry" == FT_BYTES -- BASE_NONE

    /*List of Errors/Events*/
    case "Entry Type"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.sres.el.entry.type" == FT_UINT16 -- BASE_HEX
    // case "Profile"_h:
    //   DPRINT(d, fi, "TODO");
    //   break; // "epl-xdd.asnd.sres.el.entry.type.profile" == FT_UINT16 -- BASE_DEC
    case "Mode"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.sres.el.entry.type.mode" == FT_UINT16 -- BASE_DEC
    case "Bit14"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.sres.el.entry.type.bit14" == FT_UINT16 -- BASE_DEC
    case "Bit15"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.sres.el.entry.type.bit15" == FT_UINT16 -- BASE_DEC
    case "Error Code"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.sres.el.entry.code" == FT_UINT16 -- BASE_DEC
    case "Time Stamp"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.sres.el.entry.time" == FT_UINT64 -- BASE_DEC
    case "Additional Information"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.sres.el.entry.add" == FT_UINT64 -- BASE_DEC


    /* ASnd-->NMTRequest */
    case "NMTRequestedCommandID"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.nmtrequest.rcid" == FT_UINT8 -- BASE_HEX_DEC
    case "NMTRequestedCommandTarget"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.nmtrequest.rct" == FT_UINT8 -- BASE_DEC_HEX
    case "NMTRequestedCommandData"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.nmtrequest.rcd" == FT_BYTES -- BASE_NONE

    /* ASnd-->NMTCommand */
    case "NMTCommandId"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.nmtcommand.cid" == FT_UINT8 -- BASE_HEX_DEC | BASE_EXT_STRING
    case "Reset Reason"_h:
      DPRINT(d, fi, "TODO");
      break; //"epl-xdd.asnd.nmtcommand.resetnode_reason" == FT_UINT16 -- BASE_HEX | BASE_EXT_STRING
    case "NMTCommandData"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.nmtcommand.cdat" == FT_BYTES -- BASE_NONE

    // case "HostName"_h:
    //   DPRINT(d, fi, "TODO");
    //   break; // "epl-xdd.asnd.nmtcommand.nmtnethostnameset.hn" == FT_BYTES -- BASE_NONE
    case "NodeID"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.nmtcommand.nmtflusharpentry.nid" == FT_UINT8 -- BASE_DEC_HEX
    case "DateTime"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.nmtcommand.nmtpublishtime.dt" == FT_BYTES -- BASE_NONE
    case "DNA"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.nmtcommand.dna" == FT_BYTES -- BASE_NONE
    case "Valid flags"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.nmtcommand.dna.flags" == FT_UINT8 -- BASE_HEX
    case "Lease time valid"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.nmtcommand.dna.ltv" == FT_BOOLEAN -- 8
    case "Hub port enable mask valid"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.nmtcommand.dna.hpm" == FT_BOOLEAN -- 8
    case "Set new node number"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.nmtcommand.dna.nnn" == FT_BOOLEAN -- 8
    case "Compare current MAC ID"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.nmtcommand.dna.mac" == FT_BOOLEAN -- 8
    case "Compare current node number"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.nmtcommand.dna.cnn" == FT_BOOLEAN -- 8
    case "Current MAC ID"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.nmtcommand.dna.currmac" == FT_ETHER -- BASE_NONE
    case "Hub port enable mask"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.nmtcommand.dna.hubenmsk" == FT_UINT64 -- BASE_HEX
    case "Current node number"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.nmtcommand.dna.currnn" == FT_UINT32 -- BASE_DEC
    case "New node number"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.nmtcommand.dna.newnn" == FT_UINT32 -- BASE_DEC
    case "Lease Time"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.nmtcommand.dna.leasetime" == FT_RELATIVE_TIME -- BASE_NONE

    /* ASnd-->SDO */
    case "Sequence Layer"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.sdo.seq" == FT_NONE -- BASE_NONE
    case "ReceiveSequenceNumber"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.sdo.seq.receive.sequence.number" == FT_UINT8 -- BASE_DEC
    case "ReceiveCon"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.sdo.seq.receive.con" == FT_UINT8 -- BASE_DEC
    case "SendSequenceNumber"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.sdo.seq.send.sequence.number" == FT_UINT8 -- BASE_DEC
    case "SendCon"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.sdo.seq.send.con" == FT_UINT8 -- BASE_DEC
    case "Command Layer"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.sdo.cmd" == FT_NONE -- BASE_NONE
    case "SDO Transaction ID"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.sdo.cmd.transaction.id" == FT_UINT8 -- BASE_DEC
    case "SDO Response"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.sdo.cmd.response" == FT_UINT8 -- BASE_DEC
    case "SDO Abort"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.sdo.cmd.abort" == FT_UINT8 -- BASE_DEC
    case "SDO Sub Transfer"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.sdo.cmd.sub.abort" == FT_UINT8 -- BASE_DEC
    case "SDO Segmentation"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.sdo.cmd.segmentation" == FT_UINT8 -- BASE_DEC
    case "SDO Command ID"_h:
      DPRINT(d, fi, "TODO");
      break; //"epl-xdd.asnd.sdo.cmd.command.id" == FT_UINT8 == BASE_DEC | BASE_EXT_STRING
    case "SDO Segment size"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.sdo.cmd.segment.size" == FT_UINT8 -- BASE_DEC
    case "SDO Data size"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.sdo.cmd.data.size" == FT_UINT8 -- BASE_DEC
    case "SDO Data Padding"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.sdo.cmd.data.padding" == FT_UINT8 -- BASE_DEC
    case "SDO Transfer Abort"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.sdo.cmd.abort.code" == FT_UINT8 -- BASE_HEX
    case "OD Index"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.sdo.cmd.data.index" == FT_UINT16 -- BASE_HEX
    case "OD SubIndex"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.sdo.cmd.data.subindex" == FT_UINT8 -- BASE_HEX
    case "Mapping"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.sdo.cmd.data.mapping" == FT_NONE -- BASE_NONE
    case "Index"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.sdo.cmd.data.mapping.index" == FT_UINT16 -- BASE_HEX
    case "SubIndex"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.sdo.cmd.data.mapping.subindex" == FT_UINT8 -- BASE_HEX
    case "Offset"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.sdo.cmd.data.mapping.offset" == FT_UINT16 -- BASE_HEX
    case "Length"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.sdo.cmd.data.mapping.length" == FT_UINT16 -- BASE_DEC
    case "Message fragments"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.sdo.cmd.fragments" == FT_NONE -- BASE_NONE
    case "Message fragment"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.sdo.cmd.fragment" == FT_FRAMENUM -- BASE_NONE
    case "Message fragment overlap"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.sdo.cmd.fragment.overlap" == FT_BOOLEAN -- 0
    case "Message fragment overlapping with conflicting data"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.sdo.cmd.fragment.overlap.conflicts" == FT_BOOLEAN -- 0
    case "Message has multiple tail fragments"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.sdo.cmd.fragment.multiple_tails" == FT_BOOLEAN -- 0
    case "Message fragment too long"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.sdo.cmd.fragment.too_long_fragment" == FT_BOOLEAN -- 0
    case "Message defragmentation error"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.sdo.cmd.fragment.error" == FT_FRAMENUM -- BASE_NONE
    case "Message fragment count"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.sdo.cmd.fragment.count" == FT_UINT32 -- BASE_DEC
    case "Reassembled"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.sdo.cmd.reassembled" == FT_UINT8 -- BASE_DEC
    case "Reassembled in"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.sdo.cmd.reassembled.in" == FT_FRAMENUM -- BASE_NONE
    case "Reassembled length"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.sdo.cmd.reassembled.length" == FT_UINT32 -- BASE_DEC
    case "Reassembled Data"_h:
      DPRINT(d, fi, "TODO");
      break; // "epl-xdd.asnd.sdo.cmd.reassembled.data" == FT_BYTES -- BASE_NONE
#endif

    /*
     * Replace regex
     * 1. '[\t]+\{[\t ]*&[^,]+,\n'                                    ==> ''
     * 2. '[\t]+\}[^,]*,[^\n]*\n'                                     ==> ''
     * 3. '([^"][ \t]*,)[ \t]*\n[\t]+([^"])'                          ==> '\1 \2'
     * 4. '[\t]+[ ]*(FT_[^,]+),[ ]*([^,]*),[^\n]*'                    ==> '== \1 -- \2'
     * 5. '[\t]+\{[ ]*("[^"]+")[ \t]*,[ \t]*("[^"]+")[ \t]*,[^\n]*\n' ==> 'case \2_h: DPRINT(d, fi, "TODO", \1); break;
     * // '
     */


    /* Common data fields (same for all message types) */
    case "epl-xdd.mtyp"_h:
      DPRINT(d, fi, "TODO", "MessageType");
      break; // == FT_UINT8 -- BASE_DEC
    case "epl-xdd.node"_h:
      DPRINT(d, fi, "TODO", "Node");
      break; // == FT_UINT8 -- BASE_DEC_HEX
    case "epl-xdd.dest"_h:
      DPRINT(d, fi, "TODO", "Destination");
      break; // == FT_UINT8 -- BASE_DEC_HEX
    case "epl-xdd.src"_h:
      DPRINT(d, fi, "TODO", "Source");
      break; // == FT_UINT8 -- BASE_DEC_HEX
    case "epl-xdd.payload.capture_size"_h:
      DPRINT(d, fi, "TODO", "Captured Size");
      break; // == FT_UINT16 -- BASE_DEC

    /* SoC data fields*/
    case "epl-xdd.soc"_h:
      DPRINT(d, fi, "TODO", "Flags");
      break; // == FT_UINT8 -- BASE_HEX
    case "epl-xdd.soc.mc"_h:
      DPRINT(d, fi, "TODO", "MC (Multiplexed Cycle Completed)");
      break; // == FT_BOOLEAN -- 8
    case "epl-xdd.soc.ps"_h:
      DPRINT(d, fi, "TODO", "PS (Prescaled Slot)");
      break; // == FT_BOOLEAN -- 8
    case "epl-xdd.soc.nettime"_h:
      DPRINT(d, fi, "TODO", "NetTime");
      break; // == FT_ABSOLUTE_TIME -- ABSOLUTE_TIME_LOCAL
    case "epl-xdd.soc.relativetime"_h:
      DPRINT(d, fi, "TODO", "RelativeTime");
      break; // == FT_UINT64 -- BASE_DEC

    /* PReq data fields*/
    case "epl-xdd.preq"_h:
      DPRINT(d, fi, "TODO", "Flags");
      break; // == FT_UINT8 -- BASE_HEX
    case "epl-xdd.preq.ms"_h:
      DPRINT(d, fi, "TODO", "MS (Multiplexed Slot)");
      break; // == FT_BOOLEAN -- 8
    case "epl-xdd.preq.ea"_h:
      DPRINT(d, fi, "TODO", "EA (Exception Acknowledge)");
      break; // == FT_BOOLEAN -- 8
    case "epl-xdd.preq.rd"_h:
      DPRINT(d, fi, "TODO", "RD (Ready)");
      break; // == FT_BOOLEAN -- 8
    case "epl-xdd.preq.pdov"_h:
      DPRINT(d, fi, "TODO", "PDOVersion");
      break; // == FT_UINT8 -- BASE_CUSTOM
    case "epl-xdd.preq.size"_h:
      DPRINT(d, fi, "TODO", "Size");
      break; // == FT_UINT16 -- BASE_DEC

    /* PRes data fields*/
    case "epl-xdd.pres.stat.ms"_h:
      DPRINT(d, fi, "TODO", "NMTStatus");
      break; // == FT_UINT8 -- BASE_HEX
    case "epl-xdd.pres.stat.cs"_h:
      DPRINT(d, fi, "TODO", "NMTStatus");
      break; // == FT_UINT8 -- BASE_HEX
    case "epl-xdd.pres"_h:
      DPRINT(d, fi, "TODO", "Flags");
      break; // == FT_UINT8 -- BASE_HEX
    case "epl-xdd.pres.ms"_h:
      DPRINT(d, fi, "TODO", "MS (Multiplexed Slot)");
      break; // == FT_BOOLEAN -- 8
    case "epl-xdd.pres.en"_h:
      DPRINT(d, fi, "TODO", "EN (Exception New)");
      break; // == FT_BOOLEAN -- 8
    case "epl-xdd.pres.rd"_h:
      DPRINT(d, fi, "TODO", "RD (Ready)");
      break; // == FT_BOOLEAN -- 8
    case "epl-xdd.pres.pr"_h:
      DPRINT(d, fi, "TODO", "PR (Priority)");
      break; // == FT_UINT8 -- BASE_DEC
    case "epl-xdd.pres.rs"_h:
      DPRINT(d, fi, "TODO", "RS (RequestToSend)");
      break; // == FT_UINT8 -- BASE_DEC
    case "epl-xdd.pres.pdov"_h:
      DPRINT(d, fi, "TODO", "PDOVersion");
      break; // == FT_STRING -- BASE_NONE
    case "epl-xdd.pres.size"_h:
      DPRINT(d, fi, "TODO", "Size");
      break; // == FT_UINT16 -- BASE_DEC

    /* SoA data fields*/
    case "epl-xdd.soa.stat.ms"_h:
      DPRINT(d, fi, "TODO", "NMTStatus");
      break; // == FT_UINT8 -- BASE_HEX
    case "epl-xdd.soa.stat.cs"_h:
      DPRINT(d, fi, "TODO", "NMTStatus");
      break; // == FT_UINT8 -- BASE_HEX
    case "epl-xdd.soa.ea"_h:
      DPRINT(d, fi, "TODO", "EA (Exception Acknowledge)");
      break; // == FT_BOOLEAN -- 8
    case "epl-xdd.soa.er"_h:
      DPRINT(d, fi, "TODO", "ER (Exception Reset)");
      break; // == FT_BOOLEAN -- 8
    case "epl-xdd.soa.svid"_h:
      DPRINT(d, fi, "TODO", "RequestedServiceID");
      break; // == FT_UINT8 -- BASE_DEC|BASE_RANGE_STRING
    case "epl-xdd.soa.svtg"_h:
      DPRINT(d, fi, "TODO", "RequestedServiceTarget");
      break; // == FT_UINT8 -- BASE_DEC
    case "epl-xdd.soa.eplv"_h:
      DPRINT(d, fi, "TODO", "EPLVersion");
      break; // == FT_UINT8 -- BASE_CUSTOM
    case "epl-xdd.soa.sync"_h:
      DPRINT(d, fi, "TODO", "SyncControl");
      break; // == FT_UINT8 -- BASE_HEX
    case "epl-xdd.soa.adva"_h:
      DPRINT(d, fi, "TODO", "DestMacAddressValid");
      break; // == FT_BOOLEAN -- 8

    case "epl-xdd.soa.tm"_h:
      DPRINT(d, fi, "TODO", "PResFallBackTimeoutValid");
      break; // == FT_BOOLEAN -- 8
    case "epl-xdd.soa.mnsc"_h:
      DPRINT(d, fi, "TODO", "SyncMNDelaySecondValid");
      break; // == FT_BOOLEAN -- 8
    case "epl-xdd.soa.mnft"_h:
      DPRINT(d, fi, "TODO", "SyncMNDelayFirstValid");
      break; // == FT_BOOLEAN -- 8
    case "epl-xdd.soa.prsc"_h:
      DPRINT(d, fi, "TODO", "PResTimeSecondValid");
      break; // == FT_BOOLEAN -- 8
    case "epl-xdd.soa.prft"_h:
      DPRINT(d, fi, "TODO", "PResTimeFirstValid");
      break; // == FT_BOOLEAN -- 8
    case "epl-xdd.soa.mst"_h:
      DPRINT(d, fi, "TODO", "PResModeSet");
      break; // == FT_BOOLEAN -- 8
    case "epl-xdd.soa.mrst"_h:
      DPRINT(d, fi, "TODO", "PResModeReset");
      break; // == FT_BOOLEAN -- 8
    case "epl-xdd.soa.adva.end"_h:
      DPRINT(d, fi, "TODO", "DestMacAddress");
      break; // == FT_ETHER -- BASE_NONE
    case "epl-xdd.soa.tm.end"_h:
      DPRINT(d, fi, "TODO", "PResFallBackTimeoutValid");
      break; // == FT_UINT8 -- BASE_DEC
    case "epl-xdd.soa.mnsc.end"_h:
      DPRINT(d, fi, "TODO", "SyncMNDelaySecondValid");
      break; // == FT_UINT8 -- BASE_DEC
    case "epl-xdd.soa.mnft.end"_h:
      DPRINT(d, fi, "TODO", "SyncMNDelayFirstValid");
      break; // == FT_UINT8 -- BASE_DEC
    case "epl-xdd.soa.prsc.end"_h:
      DPRINT(d, fi, "TODO", "PResTimeSecondValid");
      break; // == FT_UINT8 -- BASE_DEC
    case "epl-xdd.soa.prft.end"_h:
      DPRINT(d, fi, "TODO", "PResTimeFirstValid");
      break; // == FT_UINT8 -- BASE_DEC
    case "epl-xdd.soa.an.global"_h:
      DPRINT(d, fi, "TODO", "AN (Global)");
      break; // == FT_BOOLEAN -- 8
    case "epl-xdd.soa.an.local"_h:
      DPRINT(d, fi, "TODO", "AN (Local)");
      break; // == FT_BOOLEAN -- 8
    /* ASnd header */
    case "epl-xdd.asnd.svid"_h:
      DPRINT(d, fi, "TODO", "Requested Service ID");
      break; // == FT_UINT8 -- BASE_HEX|BASE_RANGE_STRING
    case "epl-xdd.asnd.svtg"_h:
      DPRINT(d, fi, "TODO", "Requested Service Target");
      break; // == FT_UINT8 -- BASE_DEC
    case "epl-xdd.asnd.data"_h:
      DPRINT(d, fi, "TODO", "Data");
      break; // == FT_BYTES -- BASE_NONE

    /* ASnd-->IdentResponse */
    case "epl-xdd.asnd.ires.en"_h:
      DPRINT(d, fi, "TODO", "EN (Exception New)");
      break; // == FT_BOOLEAN -- 8
    case "epl-xdd.asnd.ires.ec"_h:
      DPRINT(d, fi, "TODO", "EC (Exception Clear)");
      break; // == FT_BOOLEAN -- 8
    case "epl-xdd.asnd.ires.pr"_h:
      DPRINT(d, fi, "TODO", "PR (Priority)");
      break; // == FT_UINT8 -- BASE_DEC
    case "epl-xdd.asnd.ires.rs"_h:
      DPRINT(d, fi, "TODO", "RS (RequestToSend)");
      break; // == FT_UINT8 -- BASE_DEC
    case "epl-xdd.asnd.ires.state.ms"_h:
      DPRINT(d, fi, "TODO", "NMTStatus");
      break; // == FT_UINT8 -- BASE_HEX
    case "epl-xdd.asnd.ires.state.cs"_h:
      DPRINT(d, fi, "TODO", "NMTStatus");
      break; // == FT_UINT8 -- BASE_HEX
    case "epl-xdd.asnd.ires.eplver"_h:
      DPRINT(d, fi, "TODO", "EPLVersion");
      break; // == FT_UINT8 -- BASE_CUSTOM
    case "epl-xdd.asnd.ires.features"_h:
      DPRINT(d, fi, "TODO", "FeatureFlags");
      break; // == FT_UINT32 -- BASE_HEX
    case "epl-xdd.asnd.ires.features.bit0"_h:
      DPRINT(d, fi, "TODO", "Isochronous");
      break; // == FT_BOOLEAN -- 32
    case "epl-xdd.asnd.ires.features.bit1"_h:
      DPRINT(d, fi, "TODO", "SDO by UDP/IP");
      break; // == FT_BOOLEAN -- 32
    case "epl-xdd.asnd.ires.features.bit2"_h:
      DPRINT(d, fi, "TODO", "SDO by ASnd");
      break; // == FT_BOOLEAN -- 32
    case "epl-xdd.asnd.ires.features.bit3"_h:
      DPRINT(d, fi, "TODO", "SDO by PDO");
      break; // == FT_BOOLEAN -- 32
    case "epl-xdd.asnd.ires.features.bit4"_h:
      DPRINT(d, fi, "TODO", "NMT Info Services");
      break; // == FT_BOOLEAN -- 32
    case "epl-xdd.asnd.ires.features.bit5"_h:
      DPRINT(d, fi, "TODO", "Ext. NMT State Commands");
      break; // == FT_BOOLEAN -- 32
    case "epl-xdd.asnd.ires.features.bit6"_h:
      DPRINT(d, fi, "TODO", "Dynamic PDO Mapping");
      break; // == FT_BOOLEAN -- 32
    case "epl-xdd.asnd.ires.features.bit7"_h:
      DPRINT(d, fi, "TODO", "NMT Service by UDP/IP");
      break; // == FT_BOOLEAN -- 32
    case "epl-xdd.asnd.ires.features.bit8"_h:
      DPRINT(d, fi, "TODO", "Configuration Manager");
      break; // == FT_BOOLEAN -- 32
    case "epl-xdd.asnd.ires.features.bit9"_h:
      DPRINT(d, fi, "TODO", "Multiplexed Access");
      break; // == FT_BOOLEAN -- 32
    case "epl-xdd.asnd.ires.features.bitA"_h:
      DPRINT(d, fi, "TODO", "NodeID setup by SW");
      break; // == FT_BOOLEAN -- 32
    case "epl-xdd.asnd.ires.features.bitB"_h:
      DPRINT(d, fi, "TODO", "MN Basic Ethernet Mode");
      break; // == FT_BOOLEAN -- 32
    case "epl-xdd.asnd.ires.features.bitC"_h:
      DPRINT(d, fi, "TODO", "Routing Type 1 Support");
      break; // == FT_BOOLEAN -- 32
    case "epl-xdd.asnd.ires.features.bitD"_h:
      DPRINT(d, fi, "TODO", "Routing Type 2 Support");
      break; // == FT_BOOLEAN -- 32
    case "epl-xdd.asnd.ires.features.bitE"_h:
      DPRINT(d, fi, "TODO", "SDO Read/Write All");
      break; // == FT_BOOLEAN -- 32
    case "epl-xdd.asnd.ires.features.bitF"_h:
      DPRINT(d, fi, "TODO", "SDO Read/Write Multiple");
      break; // == FT_BOOLEAN -- 32
    case "epl-xdd.asnd.ires.features.bit10"_h:
      DPRINT(d, fi, "TODO", "Multiple-ASend Support");
      break; // == FT_BOOLEAN -- 32
    case "epl-xdd.asnd.ires.features.bit11"_h:
      DPRINT(d, fi, "TODO", "Ring Redundancy");
      break; // == FT_BOOLEAN -- 32
    case "epl-xdd.asnd.ires.features.bit12"_h:
      DPRINT(d, fi, "TODO", "PResChaining");
      break; // == FT_BOOLEAN -- 32
    case "epl-xdd.asnd.ires.features.bit13"_h:
      DPRINT(d, fi, "TODO", "Multiple PReq/PRes");
      break; // == FT_BOOLEAN -- 32
    case "epl-xdd.asnd.ires.features.bit14"_h:
      DPRINT(d, fi, "TODO", "Dynamic Node Allocation");
      break; // == FT_BOOLEAN -- 32
    case "epl-xdd.asnd.ires.mtu"_h:
      DPRINT(d, fi, "TODO", "MTU");
      break; // == FT_UINT16 -- BASE_DEC
    case "epl-xdd.asnd.ires.pollinsize"_h:
      DPRINT(d, fi, "TODO", "PollInSize");
      break; // == FT_UINT16 -- BASE_DEC
    case "epl-xdd.asnd.ires.polloutsizes"_h:
      DPRINT(d, fi, "TODO", "PollOutSize");
      break; // == FT_UINT16 -- BASE_DEC
    case "epl-xdd.asnd.ires.resptime"_h:
      DPRINT(d, fi, "TODO", "ResponseTime");
      break; // == FT_UINT32 -- BASE_DEC
    case "epl-xdd.asnd.ires.devicetype"_h:
      DPRINT(d, fi, "TODO", "DeviceType");
      break; // == FT_STRING -- BASE_NONE
    case "epl-xdd.asnd.ires.profile"_h:
      DPRINT(d, fi, "TODO", "Profile");
      break; // == FT_UINT16 -- BASE_DEC
    case "epl-xdd.asnd.ires.vendorid"_h:
      DPRINT(d, fi, "TODO", "VendorId");
      break; // == FT_UINT32 -- BASE_DEC_HEX
    case "epl-xdd.asnd.ires.productcode"_h:
      DPRINT(d, fi, "TODO", "ProductCode");
      break; // == FT_UINT32 -- BASE_DEC_HEX
    case "epl-xdd.asnd.ires.revisionno"_h:
      DPRINT(d, fi, "TODO", "RevisionNumber");
      break; // == FT_UINT32 -- BASE_DEC_HEX
    case "epl-xdd.asnd.ires.serialno"_h:
      DPRINT(d, fi, "TODO", "SerialNumber");
      break; // == FT_UINT32 -- BASE_DEC_HEX
    case "epl-xdd.asnd.ires.vendorext1"_h:
      DPRINT(d, fi, "TODO", "VendorSpecificExtension1");
      break; // == FT_UINT64 -- BASE_DEC_HEX
    case "epl-xdd.asnd.ires.confdate"_h:
      DPRINT(d, fi, "TODO", "VerifyConfigurationDate");
      break; // == FT_UINT32 -- BASE_DEC_HEX
    case "epl-xdd.asnd.ires.conftime"_h:
      DPRINT(d, fi, "TODO", "VerifyConfigurationTime");
      break; // == FT_UINT32 -- BASE_DEC_HEX
    case "epl-xdd.asnd.ires.appswdate"_h:
      DPRINT(d, fi, "TODO", "ApplicationSwDate");
      break; // == FT_UINT32 -- BASE_DEC_HEX
    case "epl-xdd.asnd.ires.appswtime"_h:
      DPRINT(d, fi, "TODO", "ApplicationSwTime");
      break; // == FT_UINT32 -- BASE_DEC_HEX
    case "epl-xdd.asnd.ires.ip"_h:
      DPRINT(d, fi, "TODO", "IPAddress");
      break; // == FT_IPv4 -- BASE_NONE
    case "epl-xdd.asnd.ires.subnet"_h:
      DPRINT(d, fi, "TODO", "SubnetMask");
      break; // == FT_IPv4 -- BASE_NETMASK
    case "epl-xdd.asnd.ires.gateway"_h:
      DPRINT(d, fi, "TODO", "DefaultGateway");
      break; // == FT_IPv4 -- BASE_NONE
    case "epl-xdd.asnd.ires.hostname"_h:
      DPRINT(d, fi, "TODO", "HostName");
      break; // == FT_STRING -- BASE_NONE
    case "epl-xdd.asnd.ires.vendorext2"_h:
      DPRINT(d, fi, "TODO", "VendorSpecificExtension2");
      break; // == FT_BYTES -- BASE_NONE

    /* ASnd-->StatusResponse */
    case "epl-xdd.asnd.sres.en"_h:
      DPRINT(d, fi, "TODO", "EN (Exception New)");
      break; // == FT_BOOLEAN -- 8
    case "epl-xdd.asnd.sres.ec"_h:
      DPRINT(d, fi, "TODO", "EC (Exception Clear)");
      break; // == FT_BOOLEAN -- 8
    case "epl-xdd.asnd.sres.pr"_h:
      DPRINT(d, fi, "TODO", "PR (Priority)");
      break; // == FT_UINT8 -- BASE_DEC
    case "epl-xdd.asnd.sres.rs"_h:
      DPRINT(d, fi, "TODO", "RS (RequestToSend)");
      break; // == FT_UINT8 -- BASE_DEC
    case "epl-xdd.asnd.sres.stat.ms"_h:
      DPRINT(d, fi, "TODO", "NMTStatus");
      break; // == FT_UINT8 -- BASE_HEX
    case "epl-xdd.asnd.sres.stat.cs"_h:
      DPRINT(d, fi, "TODO", "NMTStatus");
      break; // == FT_UINT8 -- BASE_HEX
    /* ASnd-->SyncResponse */
    case "epl-xdd.asnd.syncresponse.sync"_h:
      DPRINT(d, fi, "TODO", "SyncResponse");
      break; // == FT_UINT8 -- BASE_HEX
    case "epl-xdd.asnd.syncresponse.fst.val"_h:
      DPRINT(d, fi, "TODO", "PResTimeFirstValid");
      break; // == FT_BOOLEAN -- 8
    case "epl-xdd.asnd.syncresponse.sec.val"_h:
      DPRINT(d, fi, "TODO", "PResTimeSecondValid");
      break; // == FT_BOOLEAN -- 8
    case "epl-xdd.asnd.syncresponse.mode"_h:
      DPRINT(d, fi, "TODO", "PResModeStatus");
      break; // == FT_BOOLEAN -- 8
    case "epl-xdd.asnd.syncresponse.latency"_h:
      DPRINT(d, fi, "TODO", "Latency");
      break; // == FT_UINT8 -- BASE_DEC
    case "epl-xdd.asnd.syncresponse.delay.station"_h:
      DPRINT(d, fi, "TODO", "SyncDelayStation");
      break; // == FT_UINT8 -- BASE_DEC
    case "epl-xdd.asnd.syncresponse.delay"_h:
      DPRINT(d, fi, "TODO", "SyncDelay");
      break; // == FT_UINT8 -- BASE_DEC
    case "epl-xdd.asnd.syncresponse.pres.fst"_h:
      DPRINT(d, fi, "TODO", "PResTimeFirst");
      break; // == FT_UINT8 -- BASE_DEC
    case "epl-xdd.asnd.syncresponse.pres.sec"_h:
      DPRINT(d, fi, "TODO", "PResTimeSecond");
      break; // == FT_UINT8 -- BASE_DEC
    case "epl-xdd.asnd.sres.seb"_h:
      DPRINT(d, fi, "TODO", "StaticErrorBitField");
      break; // == FT_BYTES -- BASE_NONE

    /*StaticErrorBitField */
    case "epl-xdd.asnd.res.seb.bit0"_h:
      DPRINT(d, fi, "TODO", "Generic error");
      break; // == FT_UINT8 -- BASE_DEC
    case "epl-xdd.asnd.res.seb.bit1"_h:
      DPRINT(d, fi, "TODO", "Current");
      break; // == FT_UINT8 -- BASE_DEC
    case "epl-xdd.asnd.res.seb.bit2"_h:
      DPRINT(d, fi, "TODO", "Voltage");
      break; // == FT_UINT8 -- BASE_DEC
    case "epl-xdd.asnd.res.seb.bit3"_h:
      DPRINT(d, fi, "TODO", "Temperature");
      break; // == FT_UINT8 -- BASE_DEC
    case "epl-xdd.asnd.res.seb.bit4"_h:
      DPRINT(d, fi, "TODO", "Communication error");
      break; // == FT_UINT8 -- BASE_DEC
    case "epl-xdd.asnd.res.seb.bit5"_h:
      DPRINT(d, fi, "TODO", "Device Profile Spec");
      break; // == FT_UINT8 -- BASE_DEC
    case "epl-xdd.asnd.res.seb.bit7"_h:
      DPRINT(d, fi, "TODO", "Manufacturer Spec");
      break; // == FT_UINT8 -- BASE_DEC
    case "epl-xdd.asnd.res.seb.devicespecific_err"_h:
      DPRINT(d, fi, "TODO", "Device Profile Spec");
      break; // == FT_BYTES -- BASE_NONE

    case "epl-xdd.asnd.sres.el"_h:
      DPRINT(d, fi, "TODO", "ErrorCodesList");
      break; // == FT_BYTES -- BASE_NONE
    case "epl-xdd.asnd.sres.el.entry"_h:
      DPRINT(d, fi, "TODO", "Entry");
      break; // == FT_BYTES -- BASE_NONE

    /*List of Errors/Events*/
    case "epl-xdd.asnd.sres.el.entry.type"_h:
      DPRINT(d, fi, "TODO", "Entry Type");
      break; // == FT_UINT16 -- BASE_HEX
    case "epl-xdd.asnd.sres.el.entry.type.profile"_h:
      DPRINT(d, fi, "TODO", "Profile");
      break; // == FT_UINT16 -- BASE_DEC
    case "epl-xdd.asnd.sres.el.entry.type.mode"_h:
      DPRINT(d, fi, "TODO", "Mode");
      break; // == FT_UINT16 -- BASE_DEC
    case "epl-xdd.asnd.sres.el.entry.type.bit14"_h:
      DPRINT(d, fi, "TODO", "Bit14");
      break; // == FT_UINT16 -- BASE_DEC
    case "epl-xdd.asnd.sres.el.entry.type.bit15"_h:
      DPRINT(d, fi, "TODO", "Bit15");
      break; // == FT_UINT16 -- BASE_DEC
    case "epl-xdd.asnd.sres.el.entry.code"_h:
      DPRINT(d, fi, "TODO", "Error Code");
      break; // == FT_UINT16 -- BASE_DEC
    case "epl-xdd.asnd.sres.el.entry.time"_h:
      DPRINT(d, fi, "TODO", "Time Stamp");
      break; // == FT_UINT64 -- BASE_DEC
    case "epl-xdd.asnd.sres.el.entry.add"_h:
      DPRINT(d, fi, "TODO", "Additional Information");
      break; // == FT_UINT64 -- BASE_DEC


    /* ASnd-->NMTRequest */
    case "epl-xdd.asnd.nmtrequest.rcid"_h:
      DPRINT(d, fi, "TODO", "NMTRequestedCommandID");
      break; // == FT_UINT8 -- BASE_HEX_DEC
    case "epl-xdd.asnd.nmtrequest.rct"_h:
      DPRINT(d, fi, "TODO", "NMTRequestedCommandTarget");
      break; // == FT_UINT8 -- BASE_DEC_HEX
    case "epl-xdd.asnd.nmtrequest.rcd"_h:
      DPRINT(d, fi, "TODO", "NMTRequestedCommandData");
      break; // == FT_BYTES -- BASE_NONE

    /* ASnd-->NMTCommand */
    case "epl-xdd.asnd.nmtcommand.cid"_h:
      DPRINT(d, fi, "TODO", "NMTCommandId");
      break; // == FT_UINT8 -- BASE_HEX_DEC | BASE_EXT_STRING
    case "epl-xdd.asnd.nmtcommand.resetnode_reason"_h:
      DPRINT(d, fi, "TODO", "Reset Reason");
      break; // == FT_UINT16 -- BASE_HEX | BASE_EXT_STRING
    case "epl-xdd.asnd.nmtcommand.cdat"_h:
      DPRINT(d, fi, "TODO", "NMTCommandData");
      break; // == FT_BYTES -- BASE_NONE

    case "epl-xdd.asnd.nmtcommand.nmtnethostnameset.hn"_h:
      DPRINT(d, fi, "TODO", "HostName");
      break; // == FT_BYTES -- BASE_NONE
    case "epl-xdd.asnd.nmtcommand.nmtflusharpentry.nid"_h:
      DPRINT(d, fi, "TODO", "NodeID");
      break; // == FT_UINT8 -- BASE_DEC_HEX
    case "epl-xdd.asnd.nmtcommand.nmtpublishtime.dt"_h:
      DPRINT(d, fi, "TODO", "DateTime");
      break; // == FT_BYTES -- BASE_NONE
    case "epl-xdd.asnd.nmtcommand.dna"_h:
      DPRINT(d, fi, "TODO", "DNA");
      break; // == FT_BYTES -- BASE_NONE
    case "epl-xdd.asnd.nmtcommand.dna.flags"_h:
      DPRINT(d, fi, "TODO", "Valid flags");
      break; // == FT_UINT8 -- BASE_HEX
    case "epl-xdd.asnd.nmtcommand.dna.ltv"_h:
      DPRINT(d, fi, "TODO", "Lease time valid");
      break; // == FT_BOOLEAN -- 8
    case "epl-xdd.asnd.nmtcommand.dna.hpm"_h:
      DPRINT(d, fi, "TODO", "Hub port enable mask valid");
      break; // == FT_BOOLEAN -- 8
    case "epl-xdd.asnd.nmtcommand.dna.nnn"_h:
      DPRINT(d, fi, "TODO", "Set new node number");
      break; // == FT_BOOLEAN -- 8
    case "epl-xdd.asnd.nmtcommand.dna.mac"_h:
      DPRINT(d, fi, "TODO", "Compare current MAC ID");
      break; // == FT_BOOLEAN -- 8
    case "epl-xdd.asnd.nmtcommand.dna.cnn"_h:
      DPRINT(d, fi, "TODO", "Compare current node number");
      break; // == FT_BOOLEAN -- 8
    case "epl-xdd.asnd.nmtcommand.dna.currmac"_h:
      DPRINT(d, fi, "TODO", "Current MAC ID");
      break; // == FT_ETHER -- BASE_NONE
    case "epl-xdd.asnd.nmtcommand.dna.hubenmsk"_h:
      DPRINT(d, fi, "TODO", "Hub port enable mask");
      break; // == FT_UINT64 -- BASE_HEX
    case "epl-xdd.asnd.nmtcommand.dna.currnn"_h:
      DPRINT(d, fi, "TODO", "Current node number");
      break; // == FT_UINT32 -- BASE_DEC
    case "epl-xdd.asnd.nmtcommand.dna.newnn"_h:
      DPRINT(d, fi, "TODO", "New node number");
      break; // == FT_UINT32 -- BASE_DEC
    case "epl-xdd.asnd.nmtcommand.dna.leasetime"_h:
      DPRINT(d, fi, "TODO", "Lease Time");
      break; // == FT_RELATIVE_TIME -- BASE_NONE

    /* ASnd-->SDO */
    case "epl-xdd.asnd.sdo.seq"_h:
      DPRINT(d, fi, "TODO", "Sequence Layer");
      break; // == FT_NONE -- BASE_NONE
    case "epl-xdd.asnd.sdo.seq.receive.sequence.number"_h:
      DPRINT(d, fi, "TODO", "ReceiveSequenceNumber");
      break; // == FT_UINT8 -- BASE_DEC
    case "epl-xdd.asnd.sdo.seq.receive.con"_h:
      DPRINT(d, fi, "TODO", "ReceiveCon");
      break; // == FT_UINT8 -- BASE_DEC
    case "epl-xdd.asnd.sdo.seq.send.sequence.number"_h:
      DPRINT(d, fi, "TODO", "SendSequenceNumber");
      break; // == FT_UINT8 -- BASE_DEC
    case "epl-xdd.asnd.sdo.seq.send.con"_h:
      DPRINT(d, fi, "TODO", "SendCon");
      break; // == FT_UINT8 -- BASE_DEC
    case "epl-xdd.asnd.sdo.cmd"_h:
      DPRINT(d, fi, "TODO", "Command Layer");
      break; // == FT_NONE -- BASE_NONE
    case "epl-xdd.asnd.sdo.cmd.transaction.id"_h:
      DPRINT(d, fi, "TODO", "SDO Transaction ID");
      break; // == FT_UINT8 -- BASE_DEC
    case "epl-xdd.asnd.sdo.cmd.response"_h:
      DPRINT(d, fi, "TODO", "SDO Response");
      break; // == FT_UINT8 -- BASE_DEC
    case "epl-xdd.asnd.sdo.cmd.abort"_h:
      DPRINT(d, fi, "TODO", "SDO Abort");
      break; // == FT_UINT8 -- BASE_DEC
    case "epl-xdd.asnd.sdo.cmd.sub.abort"_h:
      DPRINT(d, fi, "TODO", "SDO Sub Transfer");
      break; // == FT_UINT8 -- BASE_DEC
    case "epl-xdd.asnd.sdo.cmd.segmentation"_h:
      DPRINT(d, fi, "TODO", "SDO Segmentation");
      break; // == FT_UINT8 -- BASE_DEC
    case "epl-xdd.asnd.sdo.cmd.command.id"_h:
      DPRINT(d, fi, "TODO", "SDO Command ID");
      break; // == FT_UINT8 -- BASE_DEC | BASE_EXT_STRING
    case "epl-xdd.asnd.sdo.cmd.segment.size"_h:
      DPRINT(d, fi, "TODO", "SDO Segment size");
      break; // == FT_UINT8 -- BASE_DEC
    case "epl-xdd.asnd.sdo.cmd.data.size"_h:
      DPRINT(d, fi, "TODO", "SDO Data size");
      break; // == FT_UINT8 -- BASE_DEC
    case "epl-xdd.asnd.sdo.cmd.data.padding"_h:
      DPRINT(d, fi, "TODO", "SDO Data Padding");
      break; // == FT_UINT8 -- BASE_DEC
    case "epl-xdd.asnd.sdo.cmd.abort.code"_h:
      DPRINT(d, fi, "TODO", "SDO Transfer Abort");
      break; // == FT_UINT8 -- BASE_HEX | BASE_EXT_STRING
    case "epl-xdd.asnd.sdo.cmd.data.index"_h:
      DPRINT(d, fi, "TODO", "OD Index");
      break; // == FT_UINT16 -- BASE_HEX
    case "epl-xdd.asnd.sdo.cmd.data.subindex"_h:
      DPRINT(d, fi, "TODO", "OD SubIndex");
      break; // == FT_UINT8 -- BASE_HEX
    case "epl-xdd.asnd.sdo.cmd.data.mapping"_h:
      DPRINT(d, fi, "TODO", "Mapping");
      break; // == FT_NONE -- BASE_NONE
    case "epl-xdd.asnd.sdo.cmd.data.mapping.index"_h:
      DPRINT(d, fi, "TODO", "Index");
      break; // == FT_UINT16 -- BASE_HEX
    case "epl-xdd.asnd.sdo.cmd.data.mapping.subindex"_h:
      DPRINT(d, fi, "TODO", "SubIndex");
      break; // == FT_UINT8 -- BASE_HEX
    case "epl-xdd.asnd.sdo.cmd.data.mapping.offset"_h:
      DPRINT(d, fi, "TODO", "Offset");
      break; // == FT_UINT16 -- BASE_HEX
    case "epl-xdd.asnd.sdo.cmd.data.mapping.length"_h:
      DPRINT(d, fi, "TODO", "Length");
      break; // == FT_UINT16 -- BASE_DEC
    case "epl-xdd.asnd.sdo.cmd.fragments"_h:
      DPRINT(d, fi, "TODO", "Message fragments");
      break; // == FT_NONE -- BASE_NONE
    case "epl-xdd.asnd.sdo.cmd.fragment"_h:
      DPRINT(d, fi, "TODO", "Message fragment");
      break; // == FT_FRAMENUM -- BASE_NONE
    case "epl-xdd.asnd.sdo.cmd.fragment.overlap"_h:
      DPRINT(d, fi, "TODO", "Message fragment overlap");
      break; // == FT_BOOLEAN -- 0
    case "epl-xdd.asnd.sdo.cmd.fragment.overlap.conflicts"_h:
      DPRINT(d, fi, "TODO", "Message fragment overlapping with conflicting data");
      break; // == FT_BOOLEAN -- 0
    case "epl-xdd.asnd.sdo.cmd.fragment.multiple_tails"_h:
      DPRINT(d, fi, "TODO", "Message has multiple tail fragments");
      break; // == FT_BOOLEAN -- 0
    case "epl-xdd.asnd.sdo.cmd.fragment.too_long_fragment"_h:
      DPRINT(d, fi, "TODO", "Message fragment too long");
      break; // == FT_BOOLEAN -- 0
    case "epl-xdd.asnd.sdo.cmd.fragment.error"_h:
      DPRINT(d, fi, "TODO", "Message defragmentation error");
      break; // == FT_FRAMENUM -- BASE_NONE
    case "epl-xdd.asnd.sdo.cmd.fragment.count"_h:
      DPRINT(d, fi, "TODO", "Message fragment count");
      break; // == FT_UINT32 -- BASE_DEC
    case "epl-xdd.asnd.sdo.cmd.reassembled"_h:
      DPRINT(d, fi, "TODO", "Reassembled");
      break; // == FT_UINT8 -- BASE_DEC
    case "epl-xdd.asnd.sdo.cmd.reassembled.in"_h:
      DPRINT(d, fi, "TODO", "Reassembled in");
      break; // == FT_FRAMENUM -- BASE_NONE
    case "epl-xdd.asnd.sdo.cmd.reassembled.length"_h:
      DPRINT(d, fi, "TODO", "Reassembled length");
      break; // == FT_UINT32 -- BASE_DEC
    case "epl-xdd.asnd.sdo.cmd.reassembled.data"_h:
      DPRINT(d, fi, "TODO", "Reassembled Data");
      break; // == FT_BYTES -- BASE_NONE


    default:
      std::cerr << "[WiresharkParser] Unknown ID " << hi->abbrev << std::endl;
      DPRINT(d, fi, "<UNDEF>", "<NOTFOUND>");
  }

  if (node->first_child != nullptr) {
    ++d->currentLevel;
    proto_tree_children_foreach(node, foreachEPLFunc, data);
    --d->currentLevel;
  }
}

void foreachFunc(proto_tree *node, gpointer data) {
  parserData *d  = reinterpret_cast<parserData *>(data);
  field_info *fi = node->finfo;

  if (!fi || !fi->hfinfo)
    return;

  header_field_info *hi = fi->hfinfo;

  if (!hi->name)
    return;

  if (*d->eplFrameName == hi->name) {
    if (node->first_child != nullptr) {
      proto_tree_children_foreach(node, foreachEPLFunc, data);
    }
  }
}
}
}
