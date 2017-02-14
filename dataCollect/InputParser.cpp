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
#include <epan/to_str.h>
#include <iomanip> //! \todo Remove debug code
#include <iostream>
#include <sstream> //! \todo Remove debug code

#define EPL_PREFIX "epl"

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

void bindUINT16(parserData *d, field_info *fi, uint16_t &val) {
  if (fi->hfinfo->type != FT_UINT16) {
    std::cerr << "[WS Parser] bindUINT16: '" << fi->hfinfo->name << "' is not FT_UINT16 but "
              << EPLEnum2Str::toStr(fi->hfinfo->type) << std::endl;
    return;
  }

  val = static_cast<uint16_t>(fi->value.value.uinteger64);
  DPRINT(d, fi, std::to_string(val), "");
}

void bindUINT32(parserData *d, field_info *fi, uint32_t &val) {
  if (fi->hfinfo->type != FT_UINT32) {
    std::cerr << "[WS Parser] bindUINT32: '" << fi->hfinfo->name << "' is not FT_UINT32 but "
              << EPLEnum2Str::toStr(fi->hfinfo->type) << std::endl;
    return;
  }

  val = static_cast<uint32_t>(fi->value.value.uinteger64);
  DPRINT(d, fi, std::to_string(val), "");
}

void bindUINT64(parserData *d, field_info *fi, uint64_t &val) {
  if (fi->hfinfo->type != FT_UINT64) {
    std::cerr << "[WS Parser] bindUINT64: '" << fi->hfinfo->name << "' is not FT_UINT64 but "
              << EPLEnum2Str::toStr(fi->hfinfo->type) << std::endl;
    return;
  }

  val = static_cast<uint64_t>(fi->value.value.uinteger64);
  DPRINT(d, fi, std::to_string(val), "");
}

void bindABSOLUTE_TIME(parserData *d, field_info *fi, nstime_t &val) {
  if (fi->hfinfo->type != FT_ABSOLUTE_TIME) {
    std::cerr << "[WS Parser] bindABSOLUTE_TIME: '" << fi->hfinfo->name << "' is not FT_ABSOLUTE_TIME but "
              << EPLEnum2Str::toStr(fi->hfinfo->type) << std::endl;
    return;
  }

  val = fi->value.value.time;
  DPRINT(d, fi, std::to_string(val.secs) + std::string("s ,") + std::to_string(val.nsecs) + "ns", "");
}

void bindBOOL(parserData *d, field_info *fi, bool &val) {
  if (fi->hfinfo->type != FT_BOOLEAN && fi->hfinfo->type != FT_UINT8) {
    std::cerr << "[WS Parser] bindBOOL: '" << fi->hfinfo->name << "' is not FT_BOOLEAN but "
              << EPLEnum2Str::toStr(fi->hfinfo->type) << std::endl;
    return;
  }

  val = fi->value.value.uinteger64 == 0 ? false : true;
  DPRINT(d, fi, val ? "True" : "False", "");
}

void bindSTRING(parserData *d, field_info *fi, std::string &val) {
  if (fi->hfinfo->type != FT_STRING) {
    std::cerr << "[WS Parser] bindSTRING: '" << fi->hfinfo->name << "' is not FT_STRING but "
              << EPLEnum2Str::toStr(fi->hfinfo->type) << std::endl;
    return;
  }

  val = fi->value.value.string;
  DPRINT(d, fi, val, "");
}

void bindBYTES(parserData *d, field_info *fi, std::vector<uint8_t> &val) {
  if (fi->hfinfo->type != FT_BYTES) {
    std::cerr << "[WS Parser] bindBYTES: '" << fi->hfinfo->name << "' is not FT_BYTES but "
              << EPLEnum2Str::toStr(fi->hfinfo->type) << std::endl;
    return;
  }

  val.resize(static_cast<size_t>(fi->length));
  memcpy(val.data(), fi->value.value.bytes, static_cast<size_t>(fi->length));
  DPRINT(d, fi, std::to_string(fi->length) + " Bytes", "");
}

void bindIPV4(parserData *d, field_info *fi, std::string &val) {
  if (fi->hfinfo->type != FT_IPv4) {
    std::cerr << "[WS Parser] bindIPV4: '" << fi->hfinfo->name << "' is not FT_IPv4 but "
              << EPLEnum2Str::toStr(fi->hfinfo->type) << std::endl;
    return;
  }

  ipv4_addr_and_mask *ipv4;
  guint32             n_addr; /* network-order IPv4 address */
  address             addr;
  char *              addr_str;

  ipv4   = reinterpret_cast<ipv4_addr_and_mask *>(fvalue_get(&fi->value));
  n_addr = ipv4_get_net_order_addr(ipv4);

  addr.type = AT_IPv4;
  addr.len  = 4;
  addr.data = &n_addr;

  if (fi->hfinfo->display == BASE_NETMASK) {
    addr_str = static_cast<char *>(address_to_str(NULL, &addr));
  } else {
    addr_str = static_cast<char *>(address_with_resolution_to_str(NULL, &addr));
  }

  val = addr_str;
  wmem_free(NULL, addr_str);
  DPRINT(d, fi, val, "");
}


void foreachEPLFunc(proto_tree *node, gpointer data) {
  parserData *d  = reinterpret_cast<parserData *>(data);
  field_info *fi = node->finfo;

  auto test = node->tree_data->interesting_hfids;
  (void)test;

  if (!fi || !fi->hfinfo)
    return;

  header_field_info *hi = fi->hfinfo;

  if (!hi->name || !hi->abbrev)
    return;

  switch (jenkinsHash(hi->abbrev)) {
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
    case EPL_PREFIX ".mtyp"_h:
      bindEnum(d, fi, d->pType);
      break; // == FT_UINT8 -- BASE_DEC ("MessageType")
    case EPL_PREFIX ".dest"_h:
      bindUINT8(d, fi, d->dst);
      break; // == FT_UINT8 -- BASE_DEC_HEX ("Destination")
    case EPL_PREFIX ".src"_h:
      bindUINT8(d, fi, d->src);
      break; // == FT_UINT8 -- BASE_DEC_HEX ("Source")
    case EPL_PREFIX ".payload.capture_size"_h:
      bindUINT16(d, fi, d->captureSize);
      break; // == FT_UINT16 -- BASE_DEC ("Captured Size")

    // IGONRE:
    case EPL_PREFIX ".asnd.ires.features"_h: // == FT_UINT32 -- BASE_HEX ("FeatureFlags")
    case EPL_PREFIX ".soc"_h:                // == FT_UINT8 -- BASE_HEX ("Flags")
    case EPL_PREFIX ".preq"_h:               // == FT_UINT8 -- BASE_HEX ("Flags")
    case EPL_PREFIX ".pres"_h:               // == FT_UINT8 -- BASE_HEX ("Flags")
    case EPL_PREFIX ".node"_h:               // == FT_UINT8 -- BASE_DEC_HEX ("Node")
    case "text"_h: break;

    case EPL_PREFIX ".asnd.sres.stat"_h:
    case EPL_PREFIX ".asnd.ires.state"_h:
    case EPL_PREFIX ".soa.stat"_h:
    case EPL_PREFIX ".pres.stat"_h:
      bindEnum(d, fi, d->nmtState);
      break; // == FT_UINT8 -- BASE_HEX ("NMTStatus")

    /* SoC data fields*/
    case EPL_PREFIX ".soc.mc"_h:
      bindBOOL(d, fi, d->SoC_multiplexedCycleCompleted);
      break; // == FT_BOOLEAN -- 8 ("MC (Multiplexed Cycle Completed)")
    case EPL_PREFIX ".soc.ps"_h:
      bindBOOL(d, fi, d->SoC_prescaledSlot);
      break; // == FT_BOOLEAN -- 8 ("PS (Prescaled Slot)")
    case EPL_PREFIX ".soc.nettime"_h:
      bindABSOLUTE_TIME(d, fi, d->SoC_netTime);
      break; // == FT_ABSOLUTE_TIME -- ABSOLUTE_TIME_LOCAL ("NetTime")
    case EPL_PREFIX ".soc.relativetime"_h:
      bindUINT64(d, fi, d->SoC_relativeTime);
      break; // == FT_UINT64 -- BASE_DEC ("RelativeTime")

    /* PReq data fields*/
    case EPL_PREFIX ".preq.ms"_h:
      bindBOOL(d, fi, d->PReq_multiplexedSlot);
      break; // == FT_BOOLEAN -- 8 ("MS (Multiplexed Slot)")
    case EPL_PREFIX ".preq.ea"_h:
      bindBOOL(d, fi, d->PReq_exceptionAcknoledged);
      break; // == FT_BOOLEAN -- 8 ("EA (Exception Acknowledge)")
    case EPL_PREFIX ".preq.rd"_h:
      bindBOOL(d, fi, d->PReq_Ready);
      break; // == FT_BOOLEAN -- 8 ("RD (Ready)")
    case EPL_PREFIX ".preq.pdov"_h:
      bindUINT8(d, fi, d->PReq_PDOVersion);
      break; // == FT_UINT8 -- BASE_CUSTOM ("PDOVersion")
    case EPL_PREFIX ".preq.size"_h:
      bindUINT16(d, fi, d->PReq_size);
      break; // == FT_UINT16 -- BASE_DEC ("Size")

    /* PRes data fields*/
    case EPL_PREFIX ".pres.ms"_h:
      bindBOOL(d, fi, d->PRes_multiplexedSlot);
      break; // == FT_BOOLEAN -- 8 ("MS (Multiplexed Slot)")
    case EPL_PREFIX ".pres.en"_h:
      bindBOOL(d, fi, d->PRes_exceptionNew);
      break; // == FT_BOOLEAN -- 8 ( "EN (Exception New)")
    case EPL_PREFIX ".pres.rd"_h:
      bindBOOL(d, fi, d->PRes_ready);
      break; // == FT_BOOLEAN -- 8 ("RD (Ready)")
    case EPL_PREFIX ".pres.pr"_h:
      bindEnum(d, fi, d->PRes_priority);
      break; // == FT_UINT8 -- BASE_DEC ("PR (Priority)")
    case EPL_PREFIX ".pres.rs"_h:
      bindUINT8(d, fi, d->PRes_requestToSend);
      break; // == FT_UINT8 -- BASE_DEC ("RS (RequestToSend)")
    case EPL_PREFIX ".pres.pdov"_h:
      bindSTRING(d, fi, d->PRes_PDOVersion);
      break; // == FT_STRING -- BASE_NONE ("PDOVersion")
    case EPL_PREFIX ".pres.size"_h:
      bindUINT16(d, fi, d->PRes_size);
      break; // == FT_UINT16 -- BASE_DEC ("Size")

    /* SoA data fields*/
    case EPL_PREFIX ".soa.ea"_h:
      bindBOOL(d, fi, d->SoA_exceptionAcknowledge);
      break; // == FT_BOOLEAN -- 8 ("EA (Exception Acknowledge)")
    case EPL_PREFIX ".soa.er"_h:
      bindBOOL(d, fi, d->SoA_exceptionReset);
      break; // == FT_BOOLEAN -- 8 ("ER (Exception Reset)")
    case EPL_PREFIX ".soa.svid"_h:
      bindEnum(d, fi, d->SoA_requestedServiceID);
      break; // == FT_UINT8 -- BASE_DEC|BASE_RANGE_STRING ("RequestedServiceID")
    case EPL_PREFIX ".soa.svtg"_h:
      bindUINT8(d, fi, d->SoA_requestedServiceTarget);
      break; // == FT_UINT8 -- BASE_DEC ("RequestedServiceTarget")
    case EPL_PREFIX ".soa.eplv"_h:
      bindUINT8(d, fi, d->SoA_EPLVersion);
      break; // == FT_UINT8 -- BASE_CUSTOM ("EPLVersion")
    case EPL_PREFIX ".soa.sync"_h:
      bindUINT8(d, fi, d->SoA_syncControl);
      break; // == FT_UINT8 -- BASE_HEX ("SyncControl")
    case EPL_PREFIX ".soa.adva"_h:
      bindBOOL(d, fi, d->SoA_destMacAddressValid);
      break; // == FT_BOOLEAN -- 8 ("DestMacAddressValid")

    case EPL_PREFIX ".soa.tm"_h:
      bindBOOL(d, fi, d->SoA_PResFallBackTimeoutValid);
      break; // == FT_BOOLEAN -- 8 ("PResFallBackTimeoutValid")
    case EPL_PREFIX ".soa.mnsc"_h:
      bindBOOL(d, fi, d->SoA_SyncMNDelaySecondValid);
      break; // == FT_BOOLEAN -- 8 ("SyncMNDelaySecondValid")
    case EPL_PREFIX ".soa.mnft"_h:
      bindBOOL(d, fi, d->SoA_SyncMNDelayFirstValid);
      break; // == FT_BOOLEAN -- 8 ("SyncMNDelayFirstValid")
    case EPL_PREFIX ".soa.prsc"_h:
      bindBOOL(d, fi, d->SoA_PResTimeSecondValid);
      break; // == FT_BOOLEAN -- 8 ("PResTimeSecondValid")
    case EPL_PREFIX ".soa.prft"_h:
      bindBOOL(d, fi, d->SoA_PResTimeFirstValid);
      break; // == FT_BOOLEAN -- 8 ("PResTimeFirstValid")
    case EPL_PREFIX ".soa.prmst"_h:
      bindBOOL(d, fi, d->SoA_PResModeSet);
      break; // == FT_BOOLEAN -- 8 ("PResModeSet")
    case EPL_PREFIX ".soa.prmrst"_h:
      bindBOOL(d, fi, d->SoA_PResModeReset);
      break; // == FT_BOOLEAN -- 8 ("PResModeReset")
    case EPL_PREFIX ".soa.adva.end"_h:
      DPRINT(d, fi, "TODO", "DestMacAddress");
      break; // == FT_ETHER -- BASE_NONE
    case EPL_PREFIX ".soa.tm.end"_h:
      DPRINT(d, fi, "TODO", "PResFallBackTimeoutValid");
      break; // == FT_UINT8 -- BASE_DEC
    case EPL_PREFIX ".soa.mnsc.end"_h:
      DPRINT(d, fi, "TODO", "SyncMNDelaySecondValid");
      break; // == FT_UINT8 -- BASE_DEC
    case EPL_PREFIX ".soa.mnft.end"_h:
      DPRINT(d, fi, "TODO", "SyncMNDelayFirstValid");
      break; // == FT_UINT8 -- BASE_DEC
    case EPL_PREFIX ".soa.prsc.end"_h:
      DPRINT(d, fi, "TODO", "PResTimeSecondValid");
      break; // == FT_UINT8 -- BASE_DEC
    case EPL_PREFIX ".soa.prft.end"_h:
      DPRINT(d, fi, "TODO", "PResTimeFirstValid");
      break; // == FT_UINT8 -- BASE_DEC
    case EPL_PREFIX ".soa.an.global"_h:
      bindBOOL(d, fi, d->SoA_ANGlobal);
      break; // == FT_BOOLEAN -- 8 ("AN (Global)")
    case EPL_PREFIX ".soa.an.local"_h:
      bindBOOL(d, fi, d->SoA_ANLocal);
      break; // == FT_BOOLEAN -- 8 ("AN (Local)")

    /* ASnd header */
    case EPL_PREFIX ".asnd.svid"_h:
      bindEnum(d, fi, d->ASnd_requestedServiceID);
      break; // == FT_UINT8 -- BASE_HEX|BASE_RANGE_STRING ("Requested Service ID")
    case EPL_PREFIX ".asnd.svtg"_h:
      bindUINT8(d, fi, d->ASnd_requestedServiceTarget);
      break; // == FT_UINT8 -- BASE_DEC ("Requested Service Target")
    case EPL_PREFIX ".asnd.data"_h:
      bindBYTES(d, fi, d->ASnd_data);
      break; // == FT_BYTES -- BASE_NONE ("Data")

    /* ASnd-->IdentResponse */
    case EPL_PREFIX ".asnd.ires.en"_h:
      bindBOOL(d, fi, d->ASndID_exceptionNew);
      break; // == FT_BOOLEAN -- 8 ("EN (Exception New)")
    case EPL_PREFIX ".asnd.ires.ec"_h:
      bindBOOL(d, fi, d->ASndID_exceptionClear);
      break; // == FT_BOOLEAN -- 8 ("EC (Exception Clear)")
    case EPL_PREFIX ".asnd.ires.pr"_h:
      bindEnum(d, fi, d->ASndID_priority);
      break; // == FT_UINT8 -- BASE_DEC ("PR (Priority)")
    case EPL_PREFIX ".asnd.ires.rs"_h:
      bindUINT8(d, fi, d->ASndID_RequestToSend);
      break; // == FT_UINT8 -- BASE_DEC ("RS (RequestToSend)")
    case EPL_PREFIX ".asnd.ires.eplver"_h:
      bindUINT8(d, fi, d->ASndID_EPLVersion);
      break; // == FT_UINT8 -- BASE_CUSTOM ("EPLVersion")
    case EPL_PREFIX ".asnd.ires.features.bit0"_h:
      bindBOOL(d, fi, d->ASndID_Isochronous);
      break; // == FT_BOOLEAN -- 32 ("Isochronous")
    case EPL_PREFIX ".asnd.ires.features.bit1"_h:
      bindBOOL(d, fi, d->ASndID_SDOByUDP_IP);
      break; // == FT_BOOLEAN -- 32 ("SDO by UDP/IP")
    case EPL_PREFIX ".asnd.ires.features.bit2"_h:
      bindBOOL(d, fi, d->ASndID_SDOByASnd);
      break; // == FT_BOOLEAN -- 32 ("SDO by ASnd")
    case EPL_PREFIX ".asnd.ires.features.bit3"_h:
      bindBOOL(d, fi, d->ASndID_SDOByPDO);
      break; // == FT_BOOLEAN -- 32 ("SDO by PDO")
    case EPL_PREFIX ".asnd.ires.features.bit4"_h:
      bindBOOL(d, fi, d->ASndID_NMTInfoServices);
      break; // == FT_BOOLEAN -- 32 ("NMT Info Services")
    case EPL_PREFIX ".asnd.ires.features.bit5"_h:
      bindBOOL(d, fi, d->ASndID_ExtNMTStateCommands);
      break; // == FT_BOOLEAN -- 32 ("Ext. NMT State Commands")
    case EPL_PREFIX ".asnd.ires.features.bit6"_h:
      bindBOOL(d, fi, d->ASndID_DynamicPDOMapping);
      break; // == FT_BOOLEAN -- 32 ("Dynamic PDO Mapping")
    case EPL_PREFIX ".asnd.ires.features.bit7"_h:
      bindBOOL(d, fi, d->ASndID_NMTServiceByUDP_IP);
      break; // == FT_BOOLEAN -- 32 ("NMT Service by UDP/IP")
    case EPL_PREFIX ".asnd.ires.features.bit8"_h:
      bindBOOL(d, fi, d->ASndID_ConfigurationManager);
      break; // == FT_BOOLEAN -- 32 ("Configuration Manager")
    case EPL_PREFIX ".asnd.ires.features.bit9"_h:
      bindBOOL(d, fi, d->ASndID_MultiplexedAccess);
      break; // == FT_BOOLEAN -- 32 ("Multiplexed Access")
    case EPL_PREFIX ".asnd.ires.features.bitA"_h:
      bindBOOL(d, fi, d->ASndID_NodeIDSetupBySW);
      break; // == FT_BOOLEAN -- 32 ("NodeID setup by SW")
    case EPL_PREFIX ".asnd.ires.features.bitB"_h:
      bindBOOL(d, fi, d->ASndID_NMBasicEthernetMode);
      break; // == FT_BOOLEAN -- 32 ("MN Basic Ethernet Mode")
    case EPL_PREFIX ".asnd.ires.features.bitC"_h:
      bindBOOL(d, fi, d->ASndID_RoutingType1Support);
      break; // == FT_BOOLEAN -- 32 ("Routing Type 1 Support")
    case EPL_PREFIX ".asnd.ires.features.bitD"_h:
      bindBOOL(d, fi, d->ASndID_RoutingType2Support);
      break; // == FT_BOOLEAN -- 32 ("Routing Type 2 Support")
    case EPL_PREFIX ".asnd.ires.features.bitE"_h:
      bindBOOL(d, fi, d->ASndID_SDOReadWriteAll);
      break; // == FT_BOOLEAN -- 32 ("SDO Read/Write All")
    case EPL_PREFIX ".asnd.ires.features.bitF"_h:
      bindBOOL(d, fi, d->ASndID_SDOReadWriteMultiple);
      break; // == FT_BOOLEAN -- 32 ("SDO Read/Write Multiple")
    case EPL_PREFIX ".asnd.ires.features.bit10"_h:
      bindBOOL(d, fi, d->ASndID_MultipleASendSupport);
      break; // == FT_BOOLEAN -- 32 ("Multiple-ASend Support")
    case EPL_PREFIX ".asnd.ires.features.bit11"_h:
      bindBOOL(d, fi, d->ASndID_RingRedundancy);
      break; // == FT_BOOLEAN -- 32 ("Ring Redundancy")
    case EPL_PREFIX ".asnd.ires.features.bit12"_h:
      bindBOOL(d, fi, d->ASndID_PResChaining);
      break; // == FT_BOOLEAN -- 32 ("PResChaining")
    case EPL_PREFIX ".asnd.ires.features.bit13"_h:
      bindBOOL(d, fi, d->ASndID_MultiplePReqPRes);
      break; // == FT_BOOLEAN -- 32 ("Multiple PReq/PRes")
    case EPL_PREFIX ".asnd.ires.features.bit14"_h:
      bindBOOL(d, fi, d->ASndID_DynamicNodeAllocation);
      break; // == FT_BOOLEAN -- 32 ("Dynamic Node Allocation")
    case EPL_PREFIX ".asnd.ires.mtu"_h:
      bindUINT16(d, fi, d->ASndID_MTU);
      break; // == FT_UINT16 -- BASE_DEC ("MTU")
    case EPL_PREFIX ".asnd.ires.pollinsize"_h:
      bindUINT16(d, fi, d->ASndID_PollInSize);
      break; // == FT_UINT16 -- BASE_DEC ("PollInSize")
    case EPL_PREFIX ".asnd.ires.polloutsizes"_h:
      bindUINT16(d, fi, d->ASndID_PollOutSize);
      break; // == FT_UINT16 -- BASE_DEC ("PollOutSize")
    case EPL_PREFIX ".asnd.ires.resptime"_h:
      bindUINT32(d, fi, d->ASndID_ResponseTime);
      break; // == FT_UINT32 -- BASE_DEC ("ResponseTime")
    case EPL_PREFIX ".asnd.ires.devicetype"_h:
      bindSTRING(d, fi, d->ASndID_DeviceType);
      break; // == FT_STRING -- BASE_NONE ("DeviceType")
    case EPL_PREFIX ".asnd.ires.profile"_h:
      bindUINT16(d, fi, d->ASndID_Profile);
      break; // == FT_UINT16 -- BASE_DEC ("Profile")
    case EPL_PREFIX ".asnd.ires.vendorid"_h:
      bindUINT32(d, fi, d->ASndID_VendorId);
      break; // == FT_UINT32 -- BASE_DEC_HEX ("VendorId")
    case EPL_PREFIX ".asnd.ires.productcode"_h:
      bindUINT32(d, fi, d->ASndID_ProductCode);
      break; // == FT_UINT32 -- BASE_DEC_HEX ("ProductCode")
    case EPL_PREFIX ".asnd.ires.revisionno"_h:
      bindUINT32(d, fi, d->ASndID_RevisionNumber);
      break; // == FT_UINT32 -- BASE_DEC_HEX ("RevisionNumber")
    case EPL_PREFIX ".asnd.ires.serialno"_h:
      bindUINT32(d, fi, d->ASndID_SerialNumber);
      break; // == FT_UINT32 -- BASE_DEC_HEX ("SerialNumber")
    case EPL_PREFIX ".asnd.ires.vendorext1"_h:
      bindUINT64(d, fi, d->ASndID_VendorSpecificExtension1);
      break; // == FT_UINT64 -- BASE_DEC_HEX ("VendorSpecificExtension1")
    case EPL_PREFIX ".asnd.ires.confdate"_h:
      bindUINT32(d, fi, d->ASndID_VerifyConfigurationDate);
      break; // == FT_UINT32 -- BASE_DEC_HEX ("VerifyConfigurationDate")
    case EPL_PREFIX ".asnd.ires.conftime"_h:
      bindUINT32(d, fi, d->ASndID_VerifyConfigurationTime);
      break; // == FT_UINT32 -- BASE_DEC_HEX ("VerifyConfigurationTime")
    case EPL_PREFIX ".asnd.ires.appswdate"_h:
      bindUINT32(d, fi, d->ASndID_ApplicationSwDate);
      break; // == FT_UINT32 -- BASE_DEC_HEX ("ApplicationSwDate")
    case EPL_PREFIX ".asnd.ires.appswtime"_h:
      bindUINT32(d, fi, d->ASndID_ApplicationSwTime);
      break; // == FT_UINT32 -- BASE_DEC_HEX ("ApplicationSwTime")
    case EPL_PREFIX ".asnd.ires.ip"_h:
      bindIPV4(d, fi, d->ASndID_IPAddress);
      break; // == FT_IPv4 -- BASE_NONE ("IPAddress")
    case EPL_PREFIX ".asnd.ires.subnet"_h:
      bindIPV4(d, fi, d->ASndID_SubnetMask);
      break; // == FT_IPv4 -- BASE_NETMASK ("SubnetMask")
    case EPL_PREFIX ".asnd.ires.gateway"_h:
      bindIPV4(d, fi, d->ASndID_DefaultGateway);
      break; // == FT_IPv4 -- BASE_NONE ("DefaultGateway")
    case EPL_PREFIX ".asnd.ires.hostname"_h:
      bindSTRING(d, fi, d->ASndID_HostName);
      break; // == FT_STRING -- BASE_NONE ("HostName")
    case EPL_PREFIX ".asnd.ires.vendorext2"_h:
      bindBYTES(d, fi, d->ASndID_VendorSpecificExtension2);
      break; // == FT_BYTES -- BASE_NONE ("VendorSpecificExtension2")

    /* ASnd-->StatusResponse */
    case EPL_PREFIX ".asnd.sres.en"_h:
      bindBOOL(d, fi, d->ASndSR_exceptionNew);
      break; // == FT_BOOLEAN -- 8 ("EN (Exception New)")
    case EPL_PREFIX ".asnd.sres.ec"_h:
      bindBOOL(d, fi, d->ASndSR_exceptionClear);
      break; // == FT_BOOLEAN -- 8 ("EC (Exception Clear)")
    case EPL_PREFIX ".asnd.sres.pr"_h:
      bindEnum(d, fi, d->ASndSR_priority);
      break; // == FT_UINT8 -- BASE_DEC ("PR (Priority)")
    case EPL_PREFIX ".asnd.sres.rs"_h:
      bindUINT8(d, fi, d->ASndRS_requestToSend);
      break; // == FT_UINT8 -- BASE_DEC ("RS (RequestToSend)")

    /* ASnd-->SyncResponse */
    case EPL_PREFIX ".asnd.syncresponse.sync"_h:
      bindUINT8(d, fi, d->ASndSyR_SyncResponse);
      break; // == FT_UINT8 -- BASE_HEX ("SyncResponse")
    case EPL_PREFIX ".asnd.syncresponse.fst.val"_h:
      bindBOOL(d, fi, d->ASndSyR_PResTimeFirstValid);
      break; // == FT_BOOLEAN -- 8 ("PResTimeFirstValid")
    case EPL_PREFIX ".asnd.syncresponse.sec.val"_h:
      bindBOOL(d, fi, d->ASndSyR_PResTimeSecondValid);
      break; // == FT_BOOLEAN -- 8 ("PResTimeSecondValid")
    case EPL_PREFIX ".asnd.syncresponse.mode"_h:
      bindBOOL(d, fi, d->ASndSyR_PResModeStatus);
      break; // == FT_BOOLEAN -- 8 ("PResModeStatus")
    case EPL_PREFIX ".asnd.syncresponse.latency"_h:
      bindUINT8(d, fi, d->ASndSyR_Latency);
      break; // == FT_UINT8 -- BASE_DEC ("Latency")
    case EPL_PREFIX ".asnd.syncresponse.delay.station"_h:
      bindUINT8(d, fi, d->ASndSyR_SyncDelayStation);
      break; // == FT_UINT8 -- BASE_DEC ("SyncDelayStation")
    case EPL_PREFIX ".asnd.syncresponse.delay"_h:
      bindUINT8(d, fi, d->ASndSyR_SyncDelay);
      break; // == FT_UINT8 -- BASE_DEC ("SyncDelay")
    case EPL_PREFIX ".asnd.syncresponse.pres.fst"_h:
      bindUINT8(d, fi, d->ASndSyR_PResTimeFirst);
      break; // == FT_UINT8 -- BASE_DEC ("PResTimeFirst")
    case EPL_PREFIX ".asnd.syncresponse.pres.sec"_h:
      bindUINT8(d, fi, d->ASndSyR_PResTimeSecond);
      break; // == FT_UINT8 -- BASE_DEC ("PResTimeSecond")
    case EPL_PREFIX ".asnd.sres.seb"_h:
      bindUINT8(d, fi, d->ASndSyR_StaticErrorBitField);
      break; // == FT_BYTES -- BASE_NONE ("StaticErrorBitField")

    /*StaticErrorBitField */
    case EPL_PREFIX ".asnd.res.seb.bit0"_h:
      bindBOOL(d, fi, d->SEBF_genericError);
      break; // == FT_UINT8 -- BASE_DEC ("Generic error")
    case EPL_PREFIX ".asnd.res.seb.bit1"_h:
      bindBOOL(d, fi, d->SEBF_current);
      break; // == FT_UINT8 -- BASE_DEC ("Current")
    case EPL_PREFIX ".asnd.res.seb.bit2"_h:
      bindBOOL(d, fi, d->SEBF_Voltage);
      break; // == FT_UINT8 -- BASE_DEC ("Voltage")
    case EPL_PREFIX ".asnd.res.seb.bit3"_h:
      bindBOOL(d, fi, d->SEBF_Temperature);
      break; // == FT_UINT8 -- BASE_DEC ("Temperature")
    case EPL_PREFIX ".asnd.res.seb.bit4"_h:
      bindBOOL(d, fi, d->SEBF_CommunicationError);
      break; // == FT_UINT8 -- BASE_DEC ("Communication error")
    case EPL_PREFIX ".asnd.res.seb.bit5"_h:
      bindBOOL(d, fi, d->SEBF_DeviceProfileSpecB);
      break; // == FT_UINT8 -- BASE_DEC ("Device Profile Spec")
    case EPL_PREFIX ".asnd.res.seb.bit7"_h:
      bindBOOL(d, fi, d->SEBF_ManufacturerSpec);
      break; // == FT_UINT8 -- BASE_DEC ("Manufacturer Spec")
    case EPL_PREFIX ".asnd.res.seb.devicespecific_err"_h:
      bindBYTES(d, fi, d->SEBF_DeviceProfileSpec);
      break; // == FT_BYTES -- BASE_NONE ("Device Profile Spec")

    case EPL_PREFIX ".asnd.sres.el"_h:
      DPRINT(d, fi, "TODO", "ErrorCodesList");
      break; // == FT_BYTES -- BASE_NONE
    case EPL_PREFIX ".asnd.sres.el.entry"_h:
      DPRINT(d, fi, "TODO", "Entry");
      break; // == FT_BYTES -- BASE_NONE

    /*List of Errors/Events*/
    case EPL_PREFIX ".asnd.sres.el.entry.type"_h:
      DPRINT(d, fi, "TODO", "Entry Type");
      break; // == FT_UINT16 -- BASE_HEX
    case EPL_PREFIX ".asnd.sres.el.entry.type.profile"_h:
      DPRINT(d, fi, "TODO", "Profile");
      break; // == FT_UINT16 -- BASE_DEC
    case EPL_PREFIX ".asnd.sres.el.entry.type.mode"_h:
      DPRINT(d, fi, "TODO", "Mode");
      break; // == FT_UINT16 -- BASE_DEC
    case EPL_PREFIX ".asnd.sres.el.entry.type.bit14"_h:
      DPRINT(d, fi, "TODO", "Bit14");
      break; // == FT_UINT16 -- BASE_DEC
    case EPL_PREFIX ".asnd.sres.el.entry.type.bit15"_h:
      DPRINT(d, fi, "TODO", "Bit15");
      break; // == FT_UINT16 -- BASE_DEC
    case EPL_PREFIX ".asnd.sres.el.entry.code"_h:
      DPRINT(d, fi, "TODO", "Error Code");
      break; // == FT_UINT16 -- BASE_DEC
    case EPL_PREFIX ".asnd.sres.el.entry.time"_h:
      DPRINT(d, fi, "TODO", "Time Stamp");
      break; // == FT_UINT64 -- BASE_DEC
    case EPL_PREFIX ".asnd.sres.el.entry.add"_h:
      DPRINT(d, fi, "TODO", "Additional Information");
      break; // == FT_UINT64 -- BASE_DEC


    /* ASnd-->NMTRequest */
    case EPL_PREFIX ".asnd.nmtrequest.rcid"_h:
      DPRINT(d, fi, "TODO", "NMTRequestedCommandID");
      break; // == FT_UINT8 -- BASE_HEX_DEC
    case EPL_PREFIX ".asnd.nmtrequest.rct"_h:
      DPRINT(d, fi, "TODO", "NMTRequestedCommandTarget");
      break; // == FT_UINT8 -- BASE_DEC_HEX
    case EPL_PREFIX ".asnd.nmtrequest.rcd"_h:
      DPRINT(d, fi, "TODO", "NMTRequestedCommandData");
      break; // == FT_BYTES -- BASE_NONE

    /* ASnd-->NMTCommand */
    case EPL_PREFIX ".asnd.nmtcommand.cid"_h:
      DPRINT(d, fi, "TODO", "NMTCommandId");
      break; // == FT_UINT8 -- BASE_HEX_DEC | BASE_EXT_STRING
    case EPL_PREFIX ".asnd.nmtcommand.resetnode_reason"_h:
      DPRINT(d, fi, "TODO", "Reset Reason");
      break; // == FT_UINT16 -- BASE_HEX | BASE_EXT_STRING
    case EPL_PREFIX ".asnd.nmtcommand.cdat"_h:
      DPRINT(d, fi, "TODO", "NMTCommandData");
      break; // == FT_BYTES -- BASE_NONE

    case EPL_PREFIX ".asnd.nmtcommand.nmtnethostnameset.hn"_h:
      DPRINT(d, fi, "TODO", "HostName");
      break; // == FT_BYTES -- BASE_NONE
    case EPL_PREFIX ".asnd.nmtcommand.nmtflusharpentry.nid"_h:
      DPRINT(d, fi, "TODO", "NodeID");
      break; // == FT_UINT8 -- BASE_DEC_HEX
    case EPL_PREFIX ".asnd.nmtcommand.nmtpublishtime.dt"_h:
      DPRINT(d, fi, "TODO", "DateTime");
      break; // == FT_BYTES -- BASE_NONE
    case EPL_PREFIX ".asnd.nmtcommand.dna"_h:
      DPRINT(d, fi, "TODO", "DNA");
      break; // == FT_BYTES -- BASE_NONE
    case EPL_PREFIX ".asnd.nmtcommand.dna.flags"_h:
      DPRINT(d, fi, "TODO", "Valid flags");
      break; // == FT_UINT8 -- BASE_HEX
    case EPL_PREFIX ".asnd.nmtcommand.dna.ltv"_h:
      DPRINT(d, fi, "TODO", "Lease time valid");
      break; // == FT_BOOLEAN -- 8
    case EPL_PREFIX ".asnd.nmtcommand.dna.hpm"_h:
      DPRINT(d, fi, "TODO", "Hub port enable mask valid");
      break; // == FT_BOOLEAN -- 8
    case EPL_PREFIX ".asnd.nmtcommand.dna.nnn"_h:
      DPRINT(d, fi, "TODO", "Set new node number");
      break; // == FT_BOOLEAN -- 8
    case EPL_PREFIX ".asnd.nmtcommand.dna.mac"_h:
      DPRINT(d, fi, "TODO", "Compare current MAC ID");
      break; // == FT_BOOLEAN -- 8
    case EPL_PREFIX ".asnd.nmtcommand.dna.cnn"_h:
      DPRINT(d, fi, "TODO", "Compare current node number");
      break; // == FT_BOOLEAN -- 8
    case EPL_PREFIX ".asnd.nmtcommand.dna.currmac"_h:
      DPRINT(d, fi, "TODO", "Current MAC ID");
      break; // == FT_ETHER -- BASE_NONE
    case EPL_PREFIX ".asnd.nmtcommand.dna.hubenmsk"_h:
      DPRINT(d, fi, "TODO", "Hub port enable mask");
      break; // == FT_UINT64 -- BASE_HEX
    case EPL_PREFIX ".asnd.nmtcommand.dna.currnn"_h:
      DPRINT(d, fi, "TODO", "Current node number");
      break; // == FT_UINT32 -- BASE_DEC
    case EPL_PREFIX ".asnd.nmtcommand.dna.newnn"_h:
      DPRINT(d, fi, "TODO", "New node number");
      break; // == FT_UINT32 -- BASE_DEC
    case EPL_PREFIX ".asnd.nmtcommand.dna.leasetime"_h:
      DPRINT(d, fi, "TODO", "Lease Time");
      break; // == FT_RELATIVE_TIME -- BASE_NONE

    /* ASnd-->SDO */
    case EPL_PREFIX ".asnd.sdo.seq"_h:
      DPRINT(d, fi, "TODO", "Sequence Layer");
      break; // == FT_NONE -- BASE_NONE
    case EPL_PREFIX ".asnd.sdo.seq.receive.sequence.number"_h:
      DPRINT(d, fi, "TODO", "ReceiveSequenceNumber");
      break; // == FT_UINT8 -- BASE_DEC
    case EPL_PREFIX ".asnd.sdo.seq.receive.con"_h:
      DPRINT(d, fi, "TODO", "ReceiveCon");
      break; // == FT_UINT8 -- BASE_DEC
    case EPL_PREFIX ".asnd.sdo.seq.send.sequence.number"_h:
      DPRINT(d, fi, "TODO", "SendSequenceNumber");
      break; // == FT_UINT8 -- BASE_DEC
    case EPL_PREFIX ".asnd.sdo.seq.send.con"_h:
      DPRINT(d, fi, "TODO", "SendCon");
      break; // == FT_UINT8 -- BASE_DEC
    case EPL_PREFIX ".asnd.sdo.cmd"_h:
      DPRINT(d, fi, "TODO", "Command Layer");
      break; // == FT_NONE -- BASE_NONE
    case EPL_PREFIX ".asnd.sdo.cmd.transaction.id"_h:
      DPRINT(d, fi, "TODO", "SDO Transaction ID");
      break; // == FT_UINT8 -- BASE_DEC
    case EPL_PREFIX ".asnd.sdo.cmd.response"_h:
      DPRINT(d, fi, "TODO", "SDO Response");
      break; // == FT_UINT8 -- BASE_DEC
    case EPL_PREFIX ".asnd.sdo.cmd.abort"_h:
      DPRINT(d, fi, "TODO", "SDO Abort");
      break; // == FT_UINT8 -- BASE_DEC
    case EPL_PREFIX ".asnd.sdo.cmd.sub.abort"_h:
      DPRINT(d, fi, "TODO", "SDO Sub Transfer");
      break; // == FT_UINT8 -- BASE_DEC
    case EPL_PREFIX ".asnd.sdo.cmd.segmentation"_h:
      DPRINT(d, fi, "TODO", "SDO Segmentation");
      break; // == FT_UINT8 -- BASE_DEC
    case EPL_PREFIX ".asnd.sdo.cmd.command.id"_h:
      DPRINT(d, fi, "TODO", "SDO Command ID");
      break; // == FT_UINT8 -- BASE_DEC | BASE_EXT_STRING
    case EPL_PREFIX ".asnd.sdo.cmd.segment.size"_h:
      DPRINT(d, fi, "TODO", "SDO Segment size");
      break; // == FT_UINT8 -- BASE_DEC
    case EPL_PREFIX ".asnd.sdo.cmd.data.size"_h:
      DPRINT(d, fi, "TODO", "SDO Data size");
      break; // == FT_UINT8 -- BASE_DEC
    case EPL_PREFIX ".asnd.sdo.cmd.data.padding"_h:
      DPRINT(d, fi, "TODO", "SDO Data Padding");
      break; // == FT_UINT8 -- BASE_DEC
    case EPL_PREFIX ".asnd.sdo.cmd.abort.code"_h:
      DPRINT(d, fi, "TODO", "SDO Transfer Abort");
      break; // == FT_UINT8 -- BASE_HEX | BASE_EXT_STRING
    case EPL_PREFIX ".asnd.sdo.cmd.data.index"_h:
      DPRINT(d, fi, "TODO", "OD Index");
      break; // == FT_UINT16 -- BASE_HEX
    case EPL_PREFIX ".asnd.sdo.cmd.data.subindex"_h:
      DPRINT(d, fi, "TODO", "OD SubIndex");
      break; // == FT_UINT8 -- BASE_HEX
    case EPL_PREFIX ".asnd.sdo.cmd.data.mapping"_h:
      DPRINT(d, fi, "TODO", "Mapping");
      break; // == FT_NONE -- BASE_NONE
    case EPL_PREFIX ".asnd.sdo.cmd.data.mapping.index"_h:
      DPRINT(d, fi, "TODO", "Index");
      break; // == FT_UINT16 -- BASE_HEX
    case EPL_PREFIX ".asnd.sdo.cmd.data.mapping.subindex"_h:
      DPRINT(d, fi, "TODO", "SubIndex");
      break; // == FT_UINT8 -- BASE_HEX
    case EPL_PREFIX ".asnd.sdo.cmd.data.mapping.offset"_h:
      DPRINT(d, fi, "TODO", "Offset");
      break; // == FT_UINT16 -- BASE_HEX
    case EPL_PREFIX ".asnd.sdo.cmd.data.mapping.length"_h:
      DPRINT(d, fi, "TODO", "Length");
      break; // == FT_UINT16 -- BASE_DEC
    case EPL_PREFIX ".asnd.sdo.cmd.fragments"_h:
      DPRINT(d, fi, "TODO", "Message fragments");
      break; // == FT_NONE -- BASE_NONE
    case EPL_PREFIX ".asnd.sdo.cmd.fragment"_h:
      DPRINT(d, fi, "TODO", "Message fragment");
      break; // == FT_FRAMENUM -- BASE_NONE
    case EPL_PREFIX ".asnd.sdo.cmd.fragment.overlap"_h:
      DPRINT(d, fi, "TODO", "Message fragment overlap");
      break; // == FT_BOOLEAN -- 0
    case EPL_PREFIX ".asnd.sdo.cmd.fragment.overlap.conflicts"_h:
      DPRINT(d, fi, "TODO", "Message fragment overlapping with conflicting data");
      break; // == FT_BOOLEAN -- 0
    case EPL_PREFIX ".asnd.sdo.cmd.fragment.multiple_tails"_h:
      DPRINT(d, fi, "TODO", "Message has multiple tail fragments");
      break; // == FT_BOOLEAN -- 0
    case EPL_PREFIX ".asnd.sdo.cmd.fragment.too_long_fragment"_h:
      DPRINT(d, fi, "TODO", "Message fragment too long");
      break; // == FT_BOOLEAN -- 0
    case EPL_PREFIX ".asnd.sdo.cmd.fragment.error"_h:
      DPRINT(d, fi, "TODO", "Message defragmentation error");
      break; // == FT_FRAMENUM -- BASE_NONE
    case EPL_PREFIX ".asnd.sdo.cmd.fragment.count"_h:
      DPRINT(d, fi, "TODO", "Message fragment count");
      break; // == FT_UINT32 -- BASE_DEC
    case EPL_PREFIX ".asnd.sdo.cmd.reassembled"_h:
      DPRINT(d, fi, "TODO", "Reassembled");
      break; // == FT_UINT8 -- BASE_DEC
    case EPL_PREFIX ".asnd.sdo.cmd.reassembled.in"_h:
      DPRINT(d, fi, "TODO", "Reassembled in");
      break; // == FT_FRAMENUM -- BASE_NONE
    case EPL_PREFIX ".asnd.sdo.cmd.reassembled.length"_h:
      DPRINT(d, fi, "TODO", "Reassembled length");
      break; // == FT_UINT32 -- BASE_DEC
    case EPL_PREFIX ".asnd.sdo.cmd.reassembled.data"_h:
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
