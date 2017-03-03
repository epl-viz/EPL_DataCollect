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
#include "EPLEnum2Str.hpp"
#include <StringHash.hpp>
#include <epan/address_types.h>
#include <epan/print.h>
#include <epan/proto.h>
#include <epan/to_str.h>
#include <iomanip> //! \todo Remove debug code
#include <iostream>
#include <sstream> //! \todo Remove debug code

#define EPL_PREFIX EPL_DC_DISSECTOR_EPL_ID_PREFIX

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

inline void bindUINT8(parserData *d, field_info *fi, uint8_t &val);
inline void bindUINT16(parserData *d, field_info *fi, uint16_t &val);
inline void bindUINT32(parserData *d, field_info *fi, uint32_t &val);
inline void bindUINT64(parserData *d, field_info *fi, uint64_t &val);
inline void bindTIME(parserData *d, field_info *fi, nstime_t &val);
inline void bindBOOL(parserData *d, field_info *fi, bool &val);
inline void bindSTRING(parserData *d, field_info *fi, std::string &val);
inline void bindBYTES(parserData *d, field_info *fi, std::vector<uint8_t> &val);
inline void bindIPV4(parserData *d, field_info *fi, std::string &val);
inline void bindETHER(parserData *d, field_info *fi, std::string &val);

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

void bindTIME(parserData *d, field_info *fi, nstime_t &val) {
  if (fi->hfinfo->type != FT_ABSOLUTE_TIME && fi->hfinfo->type != FT_RELATIVE_TIME) {
    std::cerr << "[WS Parser] bindABSOLUTE_TIME: '" << fi->hfinfo->name
              << "' is not FT_ABSOLUTE_TIME or FT_RELATIVE_TIME but " << EPLEnum2Str::toStr(fi->hfinfo->type)
              << std::endl;
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
  n_addr = g_htonl(ipv4->addr);

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

void bindETHER(parserData *d, field_info *fi, std::string &val) {
  if (fi->hfinfo->type != FT_ETHER) {
    std::cerr << "[WS Parser] bindETHER: '" << fi->hfinfo->name << "' is not FT_ETHER but "
              << EPLEnum2Str::toStr(fi->hfinfo->type) << std::endl;
    return;
  }

  address addr;
  char *  addr_str;
  guint8 *bytes;

  bytes = static_cast<guint8 *>(fvalue_get(&fi->value));

  addr.type = AT_ETHER;
  addr.len  = 6;
  addr.data = bytes;

  addr_str = static_cast<char *>(address_with_resolution_to_str(NULL, &addr));
  val      = addr_str;

  wmem_free(NULL, addr_str);
  DPRINT(d, fi, val, "");
}


template <class E>
inline void bindEnum(parserData *d, field_info *fi, E &val) {
  static_assert(std::is_enum<E>::value == true, "E is not an enum");
  val = static_cast<E>(fi->value.value.uinteger64);
  DPRINT(d, fi, EPLEnum2Str::toStr(val), "");
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

  switch (hashFunc(hi->abbrev)) {
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

    // Unparsed data
    case "data.data"_h:
      bindBYTES(d, fi, d->data);
      break;

    // Ignore Section
    case "_ws.expert"_h:
      return;

    // IGONRE:
    case EPL_PREFIX ".pdo"_h:
    case EPL_PREFIX ".pdo.index"_h:
    case EPL_PREFIX ".pdo.subindex"_h:
    case EPL_PREFIX ".pdo.data"_h:

    case "data"_h:
    case "data.len"_h:
    case EPL_PREFIX ".info"_h:                            // Debug Info
    case EPL_PREFIX ".convo"_h:                           // Generaged Coversation ID (for dissector debugging)
    case EPL_PREFIX ".asnd.ires.features"_h:              // == FT_UINT32 -- BASE_HEX ("FeatureFlags")
    case EPL_PREFIX ".soc"_h:                             // == FT_UINT8 -- BASE_HEX ("Flags")
    case EPL_PREFIX ".preq"_h:                            // == FT_UINT8 -- BASE_HEX ("Flags")
    case EPL_PREFIX ".pres"_h:                            // == FT_UINT8 -- BASE_HEX ("Flags")
    case EPL_PREFIX ".node"_h:                            // == FT_UINT8 -- BASE_DEC_HEX ("Node")
    case EPL_PREFIX ".soa.tm.end"_h:                      // == FT_UINT8 -- BASE_DEC ("PResFallBackTimeoutValid")
    case EPL_PREFIX ".soa.mnsc.end"_h:                    // == FT_UINT8 -- BASE_DEC ("SyncMNDelaySecondValid")
    case EPL_PREFIX ".soa.mnft.end"_h:                    // == FT_UINT8 -- BASE_DEC ("SyncMNDelayFirstValid")
    case EPL_PREFIX ".soa.prsc.end"_h:                    // == FT_UINT8 -- BASE_DEC ("PResTimeSecondValid")
    case EPL_PREFIX ".soa.prft.end"_h:                    // == FT_UINT8 -- BASE_DEC ("PResTimeFirstValid")
    case EPL_PREFIX ".asnd.sres.el"_h:                    // == FT_BYTES -- BASE_NONE ("ErrorCodesList")
    case EPL_PREFIX ".asnd.sres.el.entry"_h:              // == FT_BYTES -- BASE_NONE ("Entry")
    case EPL_PREFIX ".asnd.sres.el.entry.type"_h:         // == FT_UINT16 -- BASE_HEX ("Entry Type")
    case EPL_PREFIX ".asnd.sres.el.entry.type.profile"_h: // == FT_UINT16 -- BASE_DEC ("Profile")
    case EPL_PREFIX ".asnd.sres.el.entry.type.mode"_h:    // == FT_UINT16 -- BASE_DEC ("Mode")
    case EPL_PREFIX ".asnd.sres.el.entry.type.bit14"_h:   // == FT_UINT16 -- BASE_DEC ("Bit14")
    case EPL_PREFIX ".asnd.sres.el.entry.type.bit15"_h:   // == FT_UINT16 -- BASE_DEC ("Bit15")
    case EPL_PREFIX ".asnd.sres.el.entry.code"_h:         // == FT_UINT16 -- BASE_DEC ("Error Code")
    case EPL_PREFIX ".asnd.sres.el.entry.time"_h:         // == FT_UINT64 -- BASE_DEC ("Time Stamp")
    case EPL_PREFIX ".asnd.sres.el.entry.add"_h:          // == FT_UINT64 -- BASE_DEC ("Additional Information")
    case EPL_PREFIX ".asnd.sdo.seq"_h:                    // == FT_NONE -- BASE_NONE
    case EPL_PREFIX ".asnd.sdo.cmd"_h:                    // == FT_NONE -- BASE_NONE
    case EPL_PREFIX ".asnd.sdo.cmd.data.mapping"_h:       // == FT_NONE -- BASE_NONE ("Mapping")
    case EPL_PREFIX ".asnd.sdo.cmd.fragments"_h:          // == FT_NONE -- BASE_NONE ("Message fragments")
    case EPL_PREFIX ".asnd.sdo.cmd.fragment"_h:           // == FT_FRAMENUM -- BASE_NONE ("Message fragment")
    case EPL_PREFIX ".asnd.sdo.cmd.fragment.error"_h: // == FT_FRAMENUM -- BASE_NONE ("Message defragmentation error")
    case EPL_PREFIX ".asnd.sdo.cmd.reassembled.in"_h: // == FT_FRAMENUM -- BASE_NONE ("Reassembled in")
    case "text"_h: break;

    case EPL_PREFIX ".asnd.sres.stat"_h:
    case EPL_PREFIX ".asnd.ires.state"_h:
    case EPL_PREFIX ".soa.stat"_h:
    case EPL_PREFIX ".pres.stat"_h:
      bindEnum(d, fi, d->nmtState);
      break; // == FT_UINT8 -- BASE_HEX ("NMTStatus")

    /* SoC data fields*/
    case EPL_PREFIX ".soc.mc"_h:
      bindBOOL(d, fi, d->SoC.MultiplexedCycleCompleted);
      break; // == FT_BOOLEAN -- 8 ("MC (Multiplexed Cycle Completed)")
    case EPL_PREFIX ".soc.ps"_h:
      bindBOOL(d, fi, d->SoC.PrescaledSlot);
      break; // == FT_BOOLEAN -- 8 ("PS (Prescaled Slot)")
    case EPL_PREFIX ".soc.nettime"_h:
      bindTIME(d, fi, d->SoC.NetTime);
      break; // == FT_ABSOLUTE_TIME -- ABSOLUTE_TIME_LOCAL ("NetTime")
    case EPL_PREFIX ".soc.relativetime"_h:
      bindUINT64(d, fi, d->SoC.RelativeTime);
      break; // == FT_UINT64 -- BASE_DEC ("RelativeTime")

    /* PReq data fields*/
    case EPL_PREFIX ".preq.ms"_h:
      bindBOOL(d, fi, d->PReq.MultiplexedSlot);
      break; // == FT_BOOLEAN -- 8 ("MS (Multiplexed Slot)")
    case EPL_PREFIX ".preq.ea"_h:
      bindBOOL(d, fi, d->PReq.ExceptionAcknoledged);
      break; // == FT_BOOLEAN -- 8 ("EA (Exception Acknowledge)")
    case EPL_PREFIX ".preq.rd"_h:
      bindBOOL(d, fi, d->PReq.Ready);
      break; // == FT_BOOLEAN -- 8 ("RD (Ready)")
    case EPL_PREFIX ".preq.pdov"_h:
      bindUINT8(d, fi, d->PReq.PDOVersion);
      break; // == FT_UINT8 -- BASE_CUSTOM ("PDOVersion")
    case EPL_PREFIX ".preq.size"_h:
      bindUINT16(d, fi, d->PReq.Size);
      break; // == FT_UINT16 -- BASE_DEC ("Size")

    /* PRes data fields*/
    case EPL_PREFIX ".pres.ms"_h:
      bindBOOL(d, fi, d->PRes.MultiplexedSlot);
      break; // == FT_BOOLEAN -- 8 ("MS (Multiplexed Slot)")
    case EPL_PREFIX ".pres.en"_h:
      bindBOOL(d, fi, d->PRes.ExceptionNew);
      break; // == FT_BOOLEAN -- 8 ( "EN (Exception New)")
    case EPL_PREFIX ".pres.rd"_h:
      bindBOOL(d, fi, d->PRes.Ready);
      break; // == FT_BOOLEAN -- 8 ("RD (Ready)")
    case EPL_PREFIX ".pres.pr"_h:
      bindEnum(d, fi, d->PRes.Priority);
      break; // == FT_UINT8 -- BASE_DEC ("PR (Priority)")
    case EPL_PREFIX ".pres.rs"_h:
      bindUINT8(d, fi, d->PRes.RequestToSend);
      break; // == FT_UINT8 -- BASE_DEC ("RS (RequestToSend)")
    case EPL_PREFIX ".pres.pdov"_h:
      bindSTRING(d, fi, d->PRes.PDOVersion);
      break; // == FT_STRING -- BASE_NONE ("PDOVersion")
    case EPL_PREFIX ".pres.size"_h:
      bindUINT16(d, fi, d->PRes.Size);
      break; // == FT_UINT16 -- BASE_DEC ("Size")

    /* SoA data fields*/
    case EPL_PREFIX ".soa.ea"_h:
      bindBOOL(d, fi, d->SoA.ExceptionAcknowledge);
      break; // == FT_BOOLEAN -- 8 ("EA (Exception Acknowledge)")
    case EPL_PREFIX ".soa.er"_h:
      bindBOOL(d, fi, d->SoA.ExceptionReset);
      break; // == FT_BOOLEAN -- 8 ("ER (Exception Reset)")
    case EPL_PREFIX ".soa.svid"_h:
      bindEnum(d, fi, d->SoA.RequestedServiceID);
      break; // == FT_UINT8 -- BASE_DEC|BASE_RANGE_STRING ("RequestedServiceID")
    case EPL_PREFIX ".soa.svtg"_h:
      bindUINT8(d, fi, d->SoA.RequestedServiceTarget);
      break; // == FT_UINT8 -- BASE_DEC ("RequestedServiceTarget")
    case EPL_PREFIX ".soa.eplv"_h:
      bindUINT8(d, fi, d->SoA.EPLVersion);
      break; // == FT_UINT8 -- BASE_CUSTOM ("EPLVersion")
    case EPL_PREFIX ".soa.sync"_h:
      bindUINT8(d, fi, d->SoA.SyncControl);
      break; // == FT_UINT8 -- BASE_HEX ("SyncControl")
    case EPL_PREFIX ".soa.adva"_h:
      bindBOOL(d, fi, d->SoA.DestMacAddressValid);
      break; // == FT_BOOLEAN -- 8 ("DestMacAddressValid")

    case EPL_PREFIX ".soa.tm"_h:
      bindBOOL(d, fi, d->SoA.PResFallBackTimeoutValid);
      break; // == FT_BOOLEAN -- 8 ("PResFallBackTimeoutValid")
    case EPL_PREFIX ".soa.mnsc"_h:
      bindBOOL(d, fi, d->SoA.SyncMNDelaySecondValid);
      break; // == FT_BOOLEAN -- 8 ("SyncMNDelaySecondValid")
    case EPL_PREFIX ".soa.mnft"_h:
      bindBOOL(d, fi, d->SoA.SyncMNDelayFirstValid);
      break; // == FT_BOOLEAN -- 8 ("SyncMNDelayFirstValid")
    case EPL_PREFIX ".soa.prsc"_h:
      bindBOOL(d, fi, d->SoA.PResTimeSecondValid);
      break; // == FT_BOOLEAN -- 8 ("PResTimeSecondValid")
    case EPL_PREFIX ".soa.prft"_h:
      bindBOOL(d, fi, d->SoA.PResTimeFirstValid);
      break; // == FT_BOOLEAN -- 8 ("PResTimeFirstValid")
    case EPL_PREFIX ".soa.prmst"_h:
      bindBOOL(d, fi, d->SoA.PResModeSet);
      break; // == FT_BOOLEAN -- 8 ("PResModeSet")
    case EPL_PREFIX ".soa.prmrst"_h:
      bindBOOL(d, fi, d->SoA.PResModeReset);
      break; // == FT_BOOLEAN -- 8 ("PResModeReset")
    case EPL_PREFIX ".soa.adva.end"_h:
      bindETHER(d, fi, d->SoA.DestMacAddress);
      break; // == FT_ETHER -- BASE_NONE ("DestMacAddress")

#if 0
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
#endif

    case EPL_PREFIX ".soa.an.global"_h:
      bindBOOL(d, fi, d->SoA.ANGlobal);
      break; // == FT_BOOLEAN -- 8 ("AN (Global)")
    case EPL_PREFIX ".soa.an.local"_h:
      bindBOOL(d, fi, d->SoA.ANLocal);
      break; // == FT_BOOLEAN -- 8 ("AN (Local)")

    /* ASnd header */
    case EPL_PREFIX ".asnd.svid"_h:
      bindEnum(d, fi, d->ASnd.RequestedServiceID);
      break; // == FT_UINT8 -- BASE_HEX|BASE_RANGE_STRING ("Requested Service ID")
    case EPL_PREFIX ".asnd.svtg"_h:
      bindUINT8(d, fi, d->ASnd.RequestedServiceTarget);
      break; // == FT_UINT8 -- BASE_DEC ("Requested Service Target")
    case EPL_PREFIX ".asnd.data"_h:
      bindBYTES(d, fi, d->ASnd.Data);
      break; // == FT_BYTES -- BASE_NONE ("Data")

    /* ASnd-->IdentResponse */
    case EPL_PREFIX ".asnd.ires.en"_h:
      bindBOOL(d, fi, d->ASnd.IdentResponse.ExceptionNew);
      break; // == FT_BOOLEAN -- 8 ("EN (Exception New)")
    case EPL_PREFIX ".asnd.ires.ec"_h:
      bindBOOL(d, fi, d->ASnd.IdentResponse.ExceptionClear);
      break; // == FT_BOOLEAN -- 8 ("EC (Exception Clear)")
    case EPL_PREFIX ".asnd.ires.pr"_h:
      bindEnum(d, fi, d->ASnd.IdentResponse.Priority);
      break; // == FT_UINT8 -- BASE_DEC ("PR (Priority)")
    case EPL_PREFIX ".asnd.ires.rs"_h:
      bindUINT8(d, fi, d->ASnd.IdentResponse.RequestToSend);
      break; // == FT_UINT8 -- BASE_DEC ("RS (RequestToSend)")
    case EPL_PREFIX ".asnd.ires.eplver"_h:
      bindUINT8(d, fi, d->ASnd.IdentResponse.EPLVersion);
      break; // == FT_UINT8 -- BASE_CUSTOM ("EPLVersion")
    case EPL_PREFIX ".asnd.ires.features.bit0"_h:
      bindBOOL(d, fi, d->ASnd.IdentResponse.Isochronous);
      break; // == FT_BOOLEAN -- 32 ("Isochronous")
    case EPL_PREFIX ".asnd.ires.features.bit1"_h:
      bindBOOL(d, fi, d->ASnd.IdentResponse.SDOByUDP_IP);
      break; // == FT_BOOLEAN -- 32 ("SDO by UDP/IP")
    case EPL_PREFIX ".asnd.ires.features.bit2"_h:
      bindBOOL(d, fi, d->ASnd.IdentResponse.SDOByASnd);
      break; // == FT_BOOLEAN -- 32 ("SDO by ASnd")
    case EPL_PREFIX ".asnd.ires.features.bit3"_h:
      bindBOOL(d, fi, d->ASnd.IdentResponse.SDOByPDO);
      break; // == FT_BOOLEAN -- 32 ("SDO by PDO")
    case EPL_PREFIX ".asnd.ires.features.bit4"_h:
      bindBOOL(d, fi, d->ASnd.IdentResponse.NMTInfoServices);
      break; // == FT_BOOLEAN -- 32 ("NMT Info Services")
    case EPL_PREFIX ".asnd.ires.features.bit5"_h:
      bindBOOL(d, fi, d->ASnd.IdentResponse.ExtNMTStateCommands);
      break; // == FT_BOOLEAN -- 32 ("Ext. NMT State Commands")
    case EPL_PREFIX ".asnd.ires.features.bit6"_h:
      bindBOOL(d, fi, d->ASnd.IdentResponse.DynamicPDOMapping);
      break; // == FT_BOOLEAN -- 32 ("Dynamic PDO Mapping")
    case EPL_PREFIX ".asnd.ires.features.bit7"_h:
      bindBOOL(d, fi, d->ASnd.IdentResponse.NMTServiceByUDP_IP);
      break; // == FT_BOOLEAN -- 32 ("NMT Service by UDP/IP")
    case EPL_PREFIX ".asnd.ires.features.bit8"_h:
      bindBOOL(d, fi, d->ASnd.IdentResponse.ConfigurationManager);
      break; // == FT_BOOLEAN -- 32 ("Configuration Manager")
    case EPL_PREFIX ".asnd.ires.features.bit9"_h:
      bindBOOL(d, fi, d->ASnd.IdentResponse.MultiplexedAccess);
      break; // == FT_BOOLEAN -- 32 ("Multiplexed Access")
    case EPL_PREFIX ".asnd.ires.features.bitA"_h:
      bindBOOL(d, fi, d->ASnd.IdentResponse.NodeIDSetupBySW);
      break; // == FT_BOOLEAN -- 32 ("NodeID setup by SW")
    case EPL_PREFIX ".asnd.ires.features.bitB"_h:
      bindBOOL(d, fi, d->ASnd.IdentResponse.NMBasicEthernetMode);
      break; // == FT_BOOLEAN -- 32 ("MN Basic Ethernet Mode")
    case EPL_PREFIX ".asnd.ires.features.bitC"_h:
      bindBOOL(d, fi, d->ASnd.IdentResponse.RoutingType1Support);
      break; // == FT_BOOLEAN -- 32 ("Routing Type 1 Support")
    case EPL_PREFIX ".asnd.ires.features.bitD"_h:
      bindBOOL(d, fi, d->ASnd.IdentResponse.RoutingType2Support);
      break; // == FT_BOOLEAN -- 32 ("Routing Type 2 Support")
    case EPL_PREFIX ".asnd.ires.features.bitE"_h:
      bindBOOL(d, fi, d->ASnd.IdentResponse.SDOReadWriteAll);
      break; // == FT_BOOLEAN -- 32 ("SDO Read/Write All")
    case EPL_PREFIX ".asnd.ires.features.bitF"_h:
      bindBOOL(d, fi, d->ASnd.IdentResponse.SDOReadWriteMultiple);
      break; // == FT_BOOLEAN -- 32 ("SDO Read/Write Multiple")
    case EPL_PREFIX ".asnd.ires.features.bit10"_h:
      bindBOOL(d, fi, d->ASnd.IdentResponse.MultipleASendSupport);
      break; // == FT_BOOLEAN -- 32 ("Multiple-ASend Support")
    case EPL_PREFIX ".asnd.ires.features.bit11"_h:
      bindBOOL(d, fi, d->ASnd.IdentResponse.RingRedundancy);
      break; // == FT_BOOLEAN -- 32 ("Ring Redundancy")
    case EPL_PREFIX ".asnd.ires.features.bit12"_h:
      bindBOOL(d, fi, d->ASnd.IdentResponse.PResChaining);
      break; // == FT_BOOLEAN -- 32 ("PResChaining")
    case EPL_PREFIX ".asnd.ires.features.bit13"_h:
      bindBOOL(d, fi, d->ASnd.IdentResponse.MultiplePReqPRes);
      break; // == FT_BOOLEAN -- 32 ("Multiple PReq/PRes")
    case EPL_PREFIX ".asnd.ires.features.bit14"_h:
      bindBOOL(d, fi, d->ASnd.IdentResponse.DynamicNodeAllocation);
      break; // == FT_BOOLEAN -- 32 ("Dynamic Node Allocation")
    case EPL_PREFIX ".asnd.ires.mtu"_h:
      bindUINT16(d, fi, d->ASnd.IdentResponse.MTU);
      break; // == FT_UINT16 -- BASE_DEC ("MTU")
    case EPL_PREFIX ".asnd.ires.pollinsize"_h:
      bindUINT16(d, fi, d->ASnd.IdentResponse.PollInSize);
      break; // == FT_UINT16 -- BASE_DEC ("PollInSize")
    case EPL_PREFIX ".asnd.ires.polloutsizes"_h:
      bindUINT16(d, fi, d->ASnd.IdentResponse.PollOutSize);
      break; // == FT_UINT16 -- BASE_DEC ("PollOutSize")
    case EPL_PREFIX ".asnd.ires.resptime"_h:
      bindUINT32(d, fi, d->ASnd.IdentResponse.ResponseTime);
      break; // == FT_UINT32 -- BASE_DEC ("ResponseTime")
    case EPL_PREFIX ".asnd.ires.devicetype"_h:
      bindSTRING(d, fi, d->ASnd.IdentResponse.DeviceType);
      break; // == FT_STRING -- BASE_NONE ("DeviceType")
    case EPL_PREFIX ".asnd.ires.profile"_h:
      bindUINT16(d, fi, d->ASnd.IdentResponse.Profile);
      break; // == FT_UINT16 -- BASE_DEC ("Profile")
    case EPL_PREFIX ".asnd.ires.vendorid"_h:
      bindUINT32(d, fi, d->ASnd.IdentResponse.VendorId);
      break; // == FT_UINT32 -- BASE_DEC_HEX ("VendorId")
    case EPL_PREFIX ".asnd.ires.productcode"_h:
      bindUINT32(d, fi, d->ASnd.IdentResponse.ProductCode);
      break; // == FT_UINT32 -- BASE_DEC_HEX ("ProductCode")
    case EPL_PREFIX ".asnd.ires.revisionno"_h:
      bindUINT32(d, fi, d->ASnd.IdentResponse.RevisionNumber);
      break; // == FT_UINT32 -- BASE_DEC_HEX ("RevisionNumber")
    case EPL_PREFIX ".asnd.ires.serialno"_h:
      bindUINT32(d, fi, d->ASnd.IdentResponse.SerialNumber);
      break; // == FT_UINT32 -- BASE_DEC_HEX ("SerialNumber")
    case EPL_PREFIX ".asnd.ires.vendorext1"_h:
      bindUINT64(d, fi, d->ASnd.IdentResponse.VendorSpecificExtension1);
      break; // == FT_UINT64 -- BASE_DEC_HEX ("VendorSpecificExtension1")
    case EPL_PREFIX ".asnd.ires.confdate"_h:
      bindUINT32(d, fi, d->ASnd.IdentResponse.VerifyConfigurationDate);
      break; // == FT_UINT32 -- BASE_DEC_HEX ("VerifyConfigurationDate")
    case EPL_PREFIX ".asnd.ires.conftime"_h:
      bindUINT32(d, fi, d->ASnd.IdentResponse.VerifyConfigurationTime);
      break; // == FT_UINT32 -- BASE_DEC_HEX ("VerifyConfigurationTime")
    case EPL_PREFIX ".asnd.ires.appswdate"_h:
      bindUINT32(d, fi, d->ASnd.IdentResponse.ApplicationSwDate);
      break; // == FT_UINT32 -- BASE_DEC_HEX ("ApplicationSwDate")
    case EPL_PREFIX ".asnd.ires.appswtime"_h:
      bindUINT32(d, fi, d->ASnd.IdentResponse.ApplicationSwTime);
      break; // == FT_UINT32 -- BASE_DEC_HEX ("ApplicationSwTime")
    case EPL_PREFIX ".asnd.ires.ip"_h:
      bindIPV4(d, fi, d->ASnd.IdentResponse.IPAddress);
      break; // == FT_IPv4 -- BASE_NONE ("IPAddress")
    case EPL_PREFIX ".asnd.ires.subnet"_h:
      bindIPV4(d, fi, d->ASnd.IdentResponse.SubnetMask);
      break; // == FT_IPv4 -- BASE_NETMASK ("SubnetMask")
    case EPL_PREFIX ".asnd.ires.gateway"_h:
      bindIPV4(d, fi, d->ASnd.IdentResponse.DefaultGateway);
      break; // == FT_IPv4 -- BASE_NONE ("DefaultGateway")
    case EPL_PREFIX ".asnd.ires.hostname"_h:
      bindSTRING(d, fi, d->ASnd.IdentResponse.HostName);
      break; // == FT_STRING -- BASE_NONE ("HostName")
    case EPL_PREFIX ".asnd.ires.vendorext2"_h:
      bindBYTES(d, fi, d->ASnd.IdentResponse.VendorSpecificExtension2);
      break; // == FT_BYTES -- BASE_NONE ("VendorSpecificExtension2")

    /* ASnd-->StatusResponse */
    case EPL_PREFIX ".asnd.sres.en"_h:
      bindBOOL(d, fi, d->ASnd.StatusResponse.ExceptionNew);
      break; // == FT_BOOLEAN -- 8 ("EN (Exception New)")
    case EPL_PREFIX ".asnd.sres.ec"_h:
      bindBOOL(d, fi, d->ASnd.StatusResponse.ExceptionClear);
      break; // == FT_BOOLEAN -- 8 ("EC (Exception Clear)")
    case EPL_PREFIX ".asnd.sres.pr"_h:
      bindEnum(d, fi, d->ASnd.StatusResponse.Priority);
      break; // == FT_UINT8 -- BASE_DEC ("PR (Priority)")
    case EPL_PREFIX ".asnd.sres.rs"_h:
      bindUINT8(d, fi, d->ASnd.StatusResponse.RequestToSend);
      break; // == FT_UINT8 -- BASE_DEC ("RS (RequestToSend)")

    /* ASnd-->SyncResponse */
    case EPL_PREFIX ".asnd.syncresponse.sync"_h:
      bindUINT8(d, fi, d->ASnd.SyncResponse.SyncResponse);
      break; // == FT_UINT8 -- BASE_HEX ("SyncResponse")
    case EPL_PREFIX ".asnd.syncresponse.fst.val"_h:
      bindBOOL(d, fi, d->ASnd.SyncResponse.PResTimeFirstValid);
      break; // == FT_BOOLEAN -- 8 ("PResTimeFirstValid")
    case EPL_PREFIX ".asnd.syncresponse.sec.val"_h:
      bindBOOL(d, fi, d->ASnd.SyncResponse.PResTimeSecondValid);
      break; // == FT_BOOLEAN -- 8 ("PResTimeSecondValid")
    case EPL_PREFIX ".asnd.syncresponse.mode"_h:
      bindBOOL(d, fi, d->ASnd.SyncResponse.PResModeStatus);
      break; // == FT_BOOLEAN -- 8 ("PResModeStatus")
    case EPL_PREFIX ".asnd.syncresponse.latency"_h:
      bindUINT8(d, fi, d->ASnd.SyncResponse.Latency);
      break; // == FT_UINT8 -- BASE_DEC ("Latency")
    case EPL_PREFIX ".asnd..station"_h:
      bindUINT8(d, fi, d->ASnd.SyncResponse.SyncDelayStation);
      break; // == FT_UINT8 -- BASE_DEC ("SyncDelayStation")
    case EPL_PREFIX ".asnd.syncresponse.delay"_h:
      bindUINT8(d, fi, d->ASnd.SyncResponse.SyncDelay);
      break; // == FT_UINT8 -- BASE_DEC ("SyncDelay")
    case EPL_PREFIX ".asnd.syncresponse.pres.fst"_h:
      bindUINT8(d, fi, d->ASnd.SyncResponse.PResTimeFirst);
      break; // == FT_UINT8 -- BASE_DEC ("PResTimeFirst")
    case EPL_PREFIX ".asnd.syncresponse.pres.sec"_h:
      bindUINT8(d, fi, d->ASnd.SyncResponse.PResTimeSecond);
      break; // == FT_UINT8 -- BASE_DEC ("PResTimeSecond")
    case EPL_PREFIX ".asnd.sres.seb"_h:
      bindUINT8(d, fi, d->ASnd.SyncResponse.StaticErrorBitField);
      break; // == FT_BYTES -- BASE_NONE ("StaticErrorBitField")

    /*StaticErrorBitField */
    case EPL_PREFIX ".asnd.res.seb.bit0"_h:
      bindBOOL(d, fi, d->ASnd.StatusResponse.GenericError);
      break; // == FT_UINT8 -- BASE_DEC ("Generic error")
    case EPL_PREFIX ".asnd.res.seb.bit1"_h:
      bindBOOL(d, fi, d->ASnd.StatusResponse.Current);
      break; // == FT_UINT8 -- BASE_DEC ("Current")
    case EPL_PREFIX ".asnd.res.seb.bit2"_h:
      bindBOOL(d, fi, d->ASnd.StatusResponse.Voltage);
      break; // == FT_UINT8 -- BASE_DEC ("Voltage")
    case EPL_PREFIX ".asnd.res.seb.bit3"_h:
      bindBOOL(d, fi, d->ASnd.StatusResponse.Temperature);
      break; // == FT_UINT8 -- BASE_DEC ("Temperature")
    case EPL_PREFIX ".asnd.res.seb.bit4"_h:
      bindBOOL(d, fi, d->ASnd.StatusResponse.CommunicationError);
      break; // == FT_UINT8 -- BASE_DEC ("Communication error")
    case EPL_PREFIX ".asnd.res.seb.bit5"_h:
      bindBOOL(d, fi, d->ASnd.StatusResponse.DeviceProfileSpecB);
      break; // == FT_UINT8 -- BASE_DEC ("Device Profile Spec")
    case EPL_PREFIX ".asnd.res.seb.bit7"_h:
      bindBOOL(d, fi, d->ASnd.StatusResponse.ManufacturerSpec);
      break; // == FT_UINT8 -- BASE_DEC ("Manufacturer Spec")
    case EPL_PREFIX ".asnd.res.seb.devicespecific_err"_h:
      bindBYTES(d, fi, d->ASnd.StatusResponse.DeviceProfileSpec);
      break; // == FT_BYTES -- BASE_NONE ("Device Profile Spec")

#if 0
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
#endif


    /* ASnd-->NMTRequest */
    case EPL_PREFIX ".asnd.nmtrequest.rcid"_h:
      bindEnum(d, fi, d->ASnd.NMTRequest.NMTRequestedCommandID);
      break; // == FT_UINT8 -- BASE_HEX_DEC ("ASnd.NMTRequest.NMTRequestedCommandID")
    case EPL_PREFIX ".asnd.nmtrequest.rct"_h:
      bindUINT8(d, fi, d->ASnd.NMTRequest.NMTRequestedCommandTarget);
      break; // == FT_UINT8 -- BASE_DEC_HEX ("NMTRequestedCommandTarget")
    case EPL_PREFIX ".asnd.nmtrequest.rcd"_h:
      bindBYTES(d, fi, d->ASnd.NMTRequest.NMTRequestedCommandData);
      break; // == FT_BYTES -- BASE_NONE ("NMTRequestedCommandData")

    /* ASnd-->NMTCommand */
    case EPL_PREFIX ".asnd.nmtcommand.cid"_h:
      bindEnum(d, fi, d->ASnd.NMTCmd.NMTCommandId);
      break; // == FT_UINT8 -- BASE_HEX_DEC | BASE_EXT_STRING ("NMTCommandId")
    case EPL_PREFIX ".asnd.nmtcommand.resetnode_reason"_h:
      bindUINT16(d, fi, d->ASnd.NMTCmd.ResetReason);
      break; // == FT_UINT16 -- BASE_HEX | BASE_EXT_STRING ("Reset Reason")
    case EPL_PREFIX ".asnd.nmtcommand.cdat"_h:
      bindBYTES(d, fi, d->ASnd.NMTCmd.CommandData);
      break; // == FT_BYTES -- BASE_NONE ("NMTCommandData")

    case EPL_PREFIX ".asnd.nmtcommand.nmtnethostnameset.hn"_h:
      bindBYTES(d, fi, d->ASnd.NMTCmd.HostName);
      break; // == FT_BYTES -- BASE_NONE ("HostName")
    case EPL_PREFIX ".asnd.nmtcommand.nmtflusharpentry.nid"_h:
      bindUINT8(d, fi, d->ASnd.NMTCmd.NodeID);
      break; // == FT_UINT8 -- BASE_DEC_HEX ("NodeID")
    case EPL_PREFIX ".asnd.nmtcommand.nmtpublishtime.dt"_h:
      bindBYTES(d, fi, d->ASnd.NMTCmd.DateTime);
      break; // == FT_BYTES -- BASE_NONE ("DateTime")
    case EPL_PREFIX ".asnd.nmtcommand.dna"_h:
      bindBYTES(d, fi, d->ASnd.NMTCmd.DNA);
      break; // == FT_BYTES -- BASE_NONE ("DNA")
    case EPL_PREFIX ".asnd.nmtcommand.dna.flags"_h:
      bindUINT8(d, fi, d->ASnd.NMTCmd.ValidFlags);
      break; // == FT_UINT8 -- BASE_HEX ("Valid flags")
    case EPL_PREFIX ".asnd.nmtcommand.dna.ltv"_h:
      bindBOOL(d, fi, d->ASnd.NMTCmd.LeaseTimeValid);
      break; // == FT_BOOLEAN -- 8 ("Lease time valid")
    case EPL_PREFIX ".asnd.nmtcommand.dna.hpm"_h:
      bindBOOL(d, fi, d->ASnd.NMTCmd.HubPortEnableMaskValid);
      break; // == FT_BOOLEAN -- 8 ("Hub port enable mask valid")
    case EPL_PREFIX ".asnd.nmtcommand.dna.nnn"_h:
      bindBOOL(d, fi, d->ASnd.NMTCmd.SetNewNodeNumber);
      break; // == FT_BOOLEAN -- 8 ("Set new node number")
    case EPL_PREFIX ".asnd.nmtcommand.dna.mac"_h:
      bindBOOL(d, fi, d->ASnd.NMTCmd.CompareCurrentMacID);
      break; // == FT_BOOLEAN -- 8 ("Compare current MAC ID")
    case EPL_PREFIX ".asnd.nmtcommand.dna.cnn"_h:
      bindBOOL(d, fi, d->ASnd.NMTCmd.CompareCurrentNodeNumber);
      break; // == FT_BOOLEAN -- 8 ("Compare current node number")
    case EPL_PREFIX ".asnd.nmtcommand.dna.currmac"_h:
      bindETHER(d, fi, d->ASnd.NMTCmd.CurrentMacID);
      break; // == FT_ETHER -- BASE_NONE ("Current MAC ID")
    case EPL_PREFIX ".asnd.nmtcommand.dna.hubenmsk"_h:
      bindUINT64(d, fi, d->ASnd.NMTCmd.HubPortEnableMask);
      break; // == FT_UINT64 -- BASE_HEX ("Hub port enable mask")
    case EPL_PREFIX ".asnd.nmtcommand.dna.currnn"_h:
      bindUINT32(d, fi, d->ASnd.NMTCmd.CurrentNodeNumber);
      break; // == FT_UINT32 -- BASE_DEC ("Current node number")
    case EPL_PREFIX ".asnd.nmtcommand.dna.newnn"_h:
      bindUINT32(d, fi, d->ASnd.NMTCmd.NewNodeNumber);
      break; // == FT_UINT32 -- BASE_DEC ("New node number")
    case EPL_PREFIX ".asnd.nmtcommand.dna.leasetime"_h:
      bindTIME(d, fi, d->ASnd.NMTCmd.LeaseTime);
      break; // == FT_RELATIVE_TIME -- BASE_NONE ("Lease Time")

    /* ASnd-->SDO */
    case EPL_PREFIX ".asnd.sdo.seq.receive.sequence.number"_h:
      bindUINT8(d, fi, d->ASnd.SDO.SEQ.ReceiveSequenceNumber);
      break; // == FT_UINT8 -- BASE_DEC ("ReceiveSequenceNumber")
    case EPL_PREFIX ".asnd.sdo.seq.receive.con"_h:
      bindEnum(d, fi, d->ASnd.SDO.SEQ.ReceiveCon);
      break; // == FT_UINT8 -- BASE_DEC ("ReceiveCon")
    case EPL_PREFIX ".asnd.sdo.seq.send.sequence.number"_h:
      bindUINT8(d, fi, d->ASnd.SDO.SEQ.SendSequenceNumber);
      break; // == FT_UINT8 -- BASE_DEC ("SendSequenceNumber")
    case EPL_PREFIX ".asnd.sdo.seq.send.con"_h:
      bindEnum(d, fi, d->ASnd.SDO.SEQ.SendCon);
      break; // == FT_UINT8 -- BASE_DEC ("SendCon")
    case EPL_PREFIX ".asnd.sdo.cmd.transaction.id"_h:
      bindUINT8(d, fi, d->ASnd.SDO.CMD.SDOTransactionID);
      break; // == FT_UINT8 -- BASE_DEC ("SDO Transaction ID")
    case EPL_PREFIX ".asnd.sdo.cmd.response"_h:
      bindBOOL(d, fi, d->ASnd.SDO.CMD.IsResponse);
      break; // == FT_UINT8 -- BASE_DEC ("SDO Response")
    case EPL_PREFIX ".asnd.sdo.cmd.abort"_h:
      bindBOOL(d, fi, d->ASnd.SDO.CMD.AbortTransfer);
      break; // == FT_UINT8 -- BASE_DEC ("SDO Abort")
    case EPL_PREFIX ".asnd.sdo.cmd.sub.abort"_h:
      bindUINT8(d, fi, d->ASnd.SDO.CMD.SubTransfer);
      break; // == FT_UINT8 -- BASE_DEC ("SDO Sub Transfer")
    case EPL_PREFIX ".asnd.sdo.cmd.segmentation"_h:
      bindEnum(d, fi, d->ASnd.SDO.CMD.Segmentation);
      break; // == FT_UINT8 -- BASE_DEC ("SDO Segmentation")
    case EPL_PREFIX ".asnd.sdo.cmd.command.id"_h:
      bindEnum(d, fi, d->ASnd.SDO.CMD.CommandID);
      break; // == FT_UINT8 -- BASE_DEC | BASE_EXT_STRING ("SDO Command ID")
    case EPL_PREFIX ".asnd.sdo.cmd.segment.size"_h:
      bindUINT8(d, fi, d->ASnd.SDO.CMD.SegmentSize);
      break; // == FT_UINT8 -- BASE_DEC ("SDO Segment size")
    case EPL_PREFIX ".asnd.sdo.cmd.data.size"_h:
      bindUINT8(d, fi, d->ASnd.SDO.CMD.DataSize);
      break; // == FT_UINT8 -- BASE_DEC ("SDO Data size")
    case EPL_PREFIX ".asnd.sdo.cmd.data.padding"_h:
      bindUINT8(d, fi, d->ASnd.SDO.CMD.DataPadding);
      break; // == FT_UINT8 -- BASE_DEC ("SDO Data Padding")
    case EPL_PREFIX ".asnd.sdo.cmd.abort.code"_h:
      bindUINT8(d, fi, d->ASnd.SDO.CMD.TransferAbortCode);
      break; // == FT_UINT8 -- BASE_HEX | BASE_EXT_STRING ("SDO Transfer Abort")
    case EPL_PREFIX ".asnd.sdo.cmd.data.index"_h:
      bindUINT16(d, fi, d->ASnd.SDO.CMD.ODIndex);
      break; // == FT_UINT16 -- BASE_HEX ("OD Index")
    case EPL_PREFIX ".asnd.sdo.cmd.data.subindex"_h:
      bindUINT8(d, fi, d->ASnd.SDO.CMD.ODSubIndex);
      break; // == FT_UINT8 -- BASE_HEX ("OD SubIndex")
    case EPL_PREFIX ".asnd.sdo.cmd.data.mapping.index"_h:
      bindUINT16(d, fi, d->ASnd.SDO.CMD.MappingIndex);
      break; // == FT_UINT16 -- BASE_HEX ("Index")
    case EPL_PREFIX ".asnd.sdo.cmd.data.mapping.subindex"_h:
      bindUINT8(d, fi, d->ASnd.SDO.CMD.MappingSubIndex);
      break; // == FT_UINT8 -- BASE_HEX ("SubIndex")
    case EPL_PREFIX ".asnd.sdo.cmd.data.mapping.offset"_h:
      bindUINT16(d, fi, d->ASnd.SDO.CMD.MappingOffset);
      break; // == FT_UINT16 -- BASE_HEX ("Offset")
    case EPL_PREFIX ".asnd.sdo.cmd.data.mapping.length"_h:
      bindUINT16(d, fi, d->ASnd.SDO.CMD.MappingLength);
      break; // == FT_UINT16 -- BASE_DEC ("Length")
    case EPL_PREFIX ".asnd.sdo.cmd.fragment.overlap"_h:
      bindBOOL(d, fi, d->ASnd.SDO.CMD.FragmentOverlap);
      break; // == FT_BOOLEAN -- 0 ("Message fragment overlap")
    case EPL_PREFIX ".asnd.sdo.cmd.fragment.overlap.conflicts"_h:
      bindBOOL(d, fi, d->ASnd.SDO.CMD.FragmentOverlapConflicts);
      break; // == FT_BOOLEAN -- 0 ("Message fragment overlapping with conflicting data")
    case EPL_PREFIX ".asnd.sdo.cmd.fragment.multiple_tails"_h:
      bindBOOL(d, fi, d->ASnd.SDO.CMD.FragmentTailFragments);
      break; // == FT_BOOLEAN -- 0 ("Message has multiple tail fragments")
    case EPL_PREFIX ".asnd.sdo.cmd.fragment.too_long_fragment"_h:
      bindBOOL(d, fi, d->ASnd.SDO.CMD.FragmentTooLong);
      break; // == FT_BOOLEAN -- 0 ("Message fragment too long")
    case EPL_PREFIX ".asnd.sdo.cmd.fragment.count"_h:
      bindUINT32(d, fi, d->ASnd.SDO.CMD.FragmentCount);
      break; // == FT_UINT32 -- BASE_DEC ("Message fragment count")
    case EPL_PREFIX ".asnd.sdo.cmd.reassembled"_h:
      bindUINT8(d, fi, d->ASnd.SDO.CMD.Reassembled);
      break; // == FT_UINT8 -- BASE_DEC ("Reassembled")
    case EPL_PREFIX ".asnd.sdo.cmd.reassembled.length"_h:
      bindUINT32(d, fi, d->ASnd.SDO.CMD.ReassembledLength);
      break; // == FT_UINT32 -- BASE_DEC ("Reassembled length")
    case EPL_PREFIX ".asnd.sdo.cmd.reassembled.data"_h:
      bindBYTES(d, fi, d->ASnd.SDO.CMD.ReassembledData);
      break; // == FT_BYTES -- BASE_NONE ("Reassembled Data")


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
