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
 * \file InputParser.hpp
 * \brief Contains functions used by class InputHandler
 */

#pragma once

#include "defines.hpp"
#include "EPLEnum2Str.hpp"
#include "Packet.hpp"
#include <epan/proto.h>
#include <type_traits>

namespace EPL_DataCollect {
namespace WiresharkParser {

// BEGIN DEBUG CONFIG

#define ENABLE_DEBUG_PRINT 1

#if ENABLE_DEBUG_PRINT
#define DPRINT(a, b, c, d) debugFN(a, b, c)
#else
#define DPRINT(a, b, c, d) (void)a;
#endif

// END DEBUG CONFOG

struct parserData {
  std::string const *eplFrameName = nullptr;
  int                currentLevel = 0;

  MessageType         pType       = PT_UNDEF;
  SoARequestServiceID soaID       = SOAR_NO_SERVICE;
  NMTState            nmtState    = NMT_OFF;
  CommandID           cID         = CMD_ID_NIL;
  ODDescription *     descPTR     = nullptr;
  std::string         wsString    = "";
  std::string         wsOther     = "<UNDEFINED>";
  uint8_t             src         = UINT8_MAX;
  uint8_t             dst         = UINT8_MAX;
  uint16_t            captureSize = UINT16_MAX;
  Packet::TIME_POINT  tp          = std::chrono::system_clock::now();
  uint8_t             transactID  = 0;
  uint32_t            numSegments = 0;

  /* SoC data fields*/
  bool     SoC_multiplexedCycleCompleted = false;
  bool     SoC_prescaledSlot             = false;
  nstime_t SoC_netTime                   = {0, 0};
  uint64_t SoC_relativeTime              = UINT64_MAX;

  /* PReq data fields*/
  bool     PReq_multiplexedSlot      = false;
  bool     PReq_exceptionAcknoledged = false;
  bool     PReq_Ready                = false;
  uint8_t  PReq_PDOVersion           = UINT8_MAX;
  uint16_t PReq_size                 = UINT16_MAX;

  /* PRes data fields*/
  bool              PRes_multiplexedSlot = false;
  bool              PRes_exceptionNew    = false;
  bool              PRes_ready           = false;
  AsyncSendPriority PRes_priority        = ASSP_LOWEST;
  uint8_t           PRes_requestToSend   = UINT8_MAX;
  std::string       PRes_PDOVersion      = "";
  uint16_t          PRes_size            = UINT16_MAX;

  /* SoA data fields*/
  bool                SoA_exceptionAcknowledge     = false;
  bool                SoA_exceptionReset           = false;
  SoARequestServiceID SoA_requestedServiceID       = SOAR_NO_SERVICE;
  uint8_t             SoA_requestedServiceTarget   = UINT8_MAX;
  uint8_t             SoA_EPLVersion               = UINT8_MAX;
  uint8_t             SoA_syncControl              = UINT8_MAX;
  bool                SoA_destMacAddressValid      = false;
  bool                SoA_PResFallBackTimeoutValid = false;
  bool                SoA_SyncMNDelaySecondValid   = false;
  bool                SoA_SyncMNDelayFirstValid    = false;
  bool                SoA_PResTimeSecondValid      = false;
  bool                SoA_PResTimeFirstValid       = false;
  bool                SoA_PResModeSet              = false;
  bool                SoA_PResModeReset            = false;
  bool                SoA_ANGlobal                 = false;
  bool                SoA_ANLocal                  = false;

  /* ASnd header */
  ASndServiceID        ASnd_requestedServiceID     = ASND_RESERVED_0;
  uint8_t              ASnd_requestedServiceTarget = UINT8_MAX;
  std::vector<uint8_t> ASnd_data;

  /* ASnd-->IdentResponse */
  bool                 ASndID_exceptionNew             = false;
  bool                 ASndID_exceptionClear           = false;
  AsyncSendPriority    ASndID_priority                 = ASSP_LOWEST;
  uint8_t              ASndID_RequestToSend            = UINT8_MAX;
  uint8_t              ASndID_EPLVersion               = UINT8_MAX;
  bool                 ASndID_Isochronous              = false;
  bool                 ASndID_SDOByUDP_IP              = false;
  bool                 ASndID_SDOByASnd                = false;
  bool                 ASndID_SDOByPDO                 = false;
  bool                 ASndID_NMTInfoServices          = false;
  bool                 ASndID_ExtNMTStateCommands      = false;
  bool                 ASndID_DynamicPDOMapping        = false;
  bool                 ASndID_NMTServiceByUDP_IP       = false;
  bool                 ASndID_ConfigurationManager     = false;
  bool                 ASndID_MultiplexedAccess        = false;
  bool                 ASndID_NodeIDSetupBySW          = false;
  bool                 ASndID_NMBasicEthernetMode      = false;
  bool                 ASndID_RoutingType1Support      = false;
  bool                 ASndID_RoutingType2Support      = false;
  bool                 ASndID_SDOReadWriteAll          = false;
  bool                 ASndID_SDOReadWriteMultiple     = false;
  bool                 ASndID_MultipleASendSupport     = false;
  bool                 ASndID_RingRedundancy           = false;
  bool                 ASndID_PResChaining             = false;
  bool                 ASndID_MultiplePReqPRes         = false;
  bool                 ASndID_DynamicNodeAllocation    = false;
  uint16_t             ASndID_MTU                      = UINT16_MAX;
  uint16_t             ASndID_PollInSize               = UINT16_MAX;
  uint16_t             ASndID_PollOutSize              = UINT16_MAX;
  uint32_t             ASndID_ResponseTime             = UINT32_MAX;
  std::string          ASndID_DeviceType               = "";
  uint16_t             ASndID_Profile                  = UINT16_MAX;
  uint32_t             ASndID_VendorId                 = UINT32_MAX;
  uint32_t             ASndID_ProductCode              = UINT32_MAX;
  uint32_t             ASndID_RevisionNumber           = UINT32_MAX;
  uint32_t             ASndID_SerialNumber             = UINT32_MAX;
  uint64_t             ASndID_VendorSpecificExtension1 = UINT64_MAX;
  uint32_t             ASndID_VerifyConfigurationDate  = UINT32_MAX;
  uint32_t             ASndID_VerifyConfigurationTime  = UINT32_MAX;
  uint32_t             ASndID_ApplicationSwDate        = UINT32_MAX;
  uint32_t             ASndID_ApplicationSwTime        = UINT32_MAX;
  std::string          ASndID_IPAddress                = "";
  std::string          ASndID_SubnetMask               = "";
  std::string          ASndID_DefaultGateway           = "";
  std::string          ASndID_HostName                 = "";
  std::vector<uint8_t> ASndID_VendorSpecificExtension2;


  /* ASnd-->StatusResponse */
  bool              ASndSR_exceptionNew   = false;
  bool              ASndSR_exceptionClear = false;
  AsyncSendPriority ASndSR_priority       = ASSP_LOWEST;
  uint8_t           ASndRS_requestToSend  = UINT8_MAX;

  /* ASnd-->SyncResponse */
  uint8_t ASndSyR_SyncResponse        = UINT8_MAX;
  bool    ASndSyR_PResTimeFirstValid  = false;
  bool    ASndSyR_PResTimeSecondValid = false;
  bool    ASndSyR_PResModeStatus      = false;
  uint8_t ASndSyR_Latency             = UINT8_MAX;
  uint8_t ASndSyR_SyncDelayStation    = UINT8_MAX;
  uint8_t ASndSyR_SyncDelay           = UINT8_MAX;
  uint8_t ASndSyR_PResTimeFirst       = UINT8_MAX;
  uint8_t ASndSyR_PResTimeSecond      = UINT8_MAX;
  uint8_t ASndSyR_StaticErrorBitField = UINT8_MAX;

  /*StaticErrorBitField */
  bool                 SEBF_genericError       = false;
  bool                 SEBF_current            = false;
  bool                 SEBF_Voltage            = false;
  bool                 SEBF_Temperature        = false;
  bool                 SEBF_CommunicationError = false;
  bool                 SEBF_DeviceProfileSpecB = false;
  bool                 SEBF_ManufacturerSpec   = false;
  std::vector<uint8_t> SEBF_DeviceProfileSpec;

  // (mostly) PReq and PRes
  bool multiplexedSlot = false;

  // (mostly) PRes data
  bool              exceptionNew = false;
  AsyncSendPriority sendPriority = ASSP_LOWEST; // also ASnd-->IdentResponse

  // ASnd header

  // ASnd-->IdentResponse
  bool exceptionClear = false;
};

#if ENABLE_DEBUG_PRINT
void debugFN(parserData *d, field_info *fi, std::string str);
#endif

void foreachFunc(proto_tree *node, gpointer data);
void foreachEPLFunc(proto_tree *node, gpointer data);

void bindUINT8(parserData *d, field_info *fi, uint8_t &val);
void bindUINT16(parserData *d, field_info *fi, uint16_t &val);
void bindUINT32(parserData *d, field_info *fi, uint32_t &val);
void bindUINT64(parserData *d, field_info *fi, uint64_t &val);
void bindABSOLUTE_TIME(parserData *d, field_info *fi, nstime_t &val);
void bindBOOL(parserData *d, field_info *fi, bool &val);
void bindSTRING(parserData *d, field_info *fi, std::string &val);
void bindBYTES(parserData *d, field_info *fi, std::vector<uint8_t> &val);
void bindIPV4(parserData *d, field_info *fi, std::string &val);

template <class E>
inline void bindEnum(parserData *d, field_info *fi, E &val) {
  static_assert(std::is_enum<E>::value == true, "E is not an enum");
  val = static_cast<E>(fi->value.value.uinteger64);
  DPRINT(d, fi, EPLEnum2Str::toStr(val), "");
}

inline size_t jenkinsHash(char const *data) {
  size_t hash = 0;
  while (*data != 0) {
    hash += static_cast<size_t>(*data);
    hash += hash << 10;
    hash ^= hash >> 6;
    ++data;
  }
  hash += hash << 3;
  hash ^= hash >> 11;
  hash += hash << 15;
  return hash;
}

constexpr size_t jenkinsHash(char const *data, size_t n) {
  size_t hash = 0;
  for (size_t i = 0; i < n; ++i) {
    hash += static_cast<size_t>(data[i]);
    hash += hash << 10;
    hash ^= hash >> 6;
  }
  hash += hash << 3;
  hash ^= hash >> 11;
  hash += hash << 15;
  return hash;
}

constexpr size_t operator"" _h(char const *data, size_t n) { return jenkinsHash(data, n); }
}
}
