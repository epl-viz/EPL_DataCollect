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
#include "PacketDiff.hpp"
#include "EPLEnums.h"
#include <chrono>
#include <epan/proto.h>
#include <plf_colony.h>
#include <type_traits>
#include <vector>

namespace EPL_DataCollect {
namespace WiresharkParser {

// BEGIN DEBUG CONFIG

#define ENABLE_DEBUG_PRINT 0

#if ENABLE_DEBUG_PRINT
#define DPRINT(a, b, c, d) debugFN(a, b, c)
#else
#define DPRINT(a, b, c, d) (void)a;
#endif

// END DEBUG CONFOG

struct parserData final {
  std::string const *eplFrameName = nullptr;

  std::string             data;
  plf::colony<PacketDiff> diffs;

  PacketType                            pType       = PacketType::UNDEF;
  NMTState                              nmtState    = NMTState::OFF;
  std::string                           wsOther     = "";
  uint8_t                               src         = UINT8_MAX;
  uint8_t                               dst         = UINT8_MAX;
  uint16_t                              captureSize = UINT16_MAX;
  std::chrono::system_clock::time_point tp;

  struct TempPDO {
    uint16_t   Index    = UINT16_MAX;
    uint8_t    SubIndex = UINT8_MAX;
    PacketDiff Data     = PacketDiff(UINT16_MAX, UINT8_MAX, UINT64_C(0));

    TempPDO()  = default;
    ~TempPDO() = default;
  } TempPDO;

  /* SoC data fields*/
  struct SoC final {
    bool     MultiplexedCycleCompleted = false;
    bool     PrescaledSlot             = false;
    nstime_t NetTime                   = {0, 0};
    uint64_t RelativeTime              = UINT64_MAX;
  } SoC;

  /* PReq data fields*/
  struct PReq final {
    bool     MultiplexedSlot      = false;
    bool     ExceptionAcknoledged = false;
    bool     Ready                = false;
    uint8_t  PDOVersion           = UINT8_MAX;
    uint16_t Size                 = UINT16_MAX;
  } PReq;

  /* PRes data fields*/
  struct PRes final {
    bool              MultiplexedSlot = false;
    bool              ExceptionNew    = false;
    bool              Ready           = false;
    AsyncSendPriority Priority        = AsyncSendPriority::LOWEST;
    uint8_t           RequestToSend   = UINT8_MAX;
    std::string       PDOVersion      = "";
    uint16_t          Size            = UINT16_MAX;
  } PRes;

  /* SoA data fields*/
  struct SoA final {
    bool                ExceptionAcknowledge     = false;
    bool                ExceptionReset           = false;
    SoARequestServiceID RequestedServiceID       = SoARequestServiceID::NO_SERVICE;
    uint8_t             RequestedServiceTarget   = UINT8_MAX;
    uint8_t             EPLVersion               = UINT8_MAX;
    uint8_t             SyncControl              = UINT8_MAX;
    bool                DestMacAddressValid      = false;
    bool                PResFallBackTimeoutValid = false;
    bool                SyncMNDelaySecondValid   = false;
    bool                SyncMNDelayFirstValid    = false;
    bool                PResTimeSecondValid      = false;
    bool                PResTimeFirstValid       = false;
    bool                PResModeSet              = false;
    bool                PResModeReset            = false;
    std::string         DestMacAddress           = "";
    bool                ANGlobal                 = false;
    bool                ANLocal                  = false;
  } SoA;

  /* ASnd header */
  struct ASnd final {
    ASndServiceID RequestedServiceID     = ASndServiceID::RESERVED_0;
    uint8_t       RequestedServiceTarget = UINT8_MAX;
    std::string   Data;

    /* ASnd-->IdentResponse
     * \sa EPSG DS 301 V1.3.0 page 267-269
     */
    struct IdentResponse final {
      bool              ExceptionNew             = false;
      bool              ExceptionClear           = false;
      AsyncSendPriority Priority                 = AsyncSendPriority::LOWEST;
      uint8_t           RequestToSend            = UINT8_MAX;
      uint8_t           EPLVersion               = UINT8_MAX;
      bool              Isochronous              = false;
      bool              SDOByUDP_IP              = false;
      bool              SDOByASnd                = false;
      bool              SDOByPDO                 = false;
      bool              NMTInfoServices          = false;
      bool              ExtNMTStateCommands      = false;
      bool              DynamicPDOMapping        = false;
      bool              NMTServiceByUDP_IP       = false;
      bool              ConfigurationManager     = false;
      bool              MultiplexedAccess        = false;
      bool              NodeIDSetupBySW          = false;
      bool              NMBasicEthernetMode      = false;
      bool              RoutingType1Support      = false;
      bool              RoutingType2Support      = false;
      bool              SDOReadWriteAll          = false;
      bool              SDOReadWriteMultiple     = false;
      bool              MultipleASendSupport     = false;
      bool              RingRedundancy           = false;
      bool              PResChaining             = false;
      bool              MultiplePReqPRes         = false;
      bool              DynamicNodeAllocation    = false;
      uint16_t          MTU                      = UINT16_MAX;
      uint16_t          PollInSize               = UINT16_MAX;
      uint16_t          PollOutSize              = UINT16_MAX;
      uint32_t          ResponseTime             = UINT32_MAX;
      uint16_t          DeviceType               = UINT16_MAX;
      uint16_t          AdditionalDeviceType     = UINT16_MAX;
      uint32_t          VendorId                 = UINT32_MAX;
      uint32_t          ProductCode              = UINT32_MAX;
      uint32_t          RevisionNumber           = UINT32_MAX;
      uint32_t          SerialNumber             = UINT32_MAX;
      uint64_t          VendorSpecificExtension1 = UINT64_MAX;
      uint32_t          VerifyConfigurationDate  = UINT32_MAX;
      uint32_t          VerifyConfigurationTime  = UINT32_MAX;
      uint32_t          ApplicationSwDate        = UINT32_MAX;
      uint32_t          ApplicationSwTime        = UINT32_MAX;
      std::string       IPAddress                = "";
      std::string       SubnetMask               = "";
      std::string       DefaultGateway           = "";
      std::string       HostName                 = "";
      std::string       VendorSpecificExtension2;
    } IdentResponse;


    /* ASnd-->NMTRequest */
    struct NMTRequest final {
      NMTCommand  NMTRequestedCommandID     = NMTCommand::INVALID_SERVICE;
      uint8_t     NMTRequestedCommandTarget = UINT8_MAX;
      std::string NMTRequestedCommandData;
    } NMTRequest;


    /* ASnd-->NMTCommand */
    struct NMTCmd final {
      NMTCommand  NMTCommandId = NMTCommand::INVALID_SERVICE;
      uint16_t    ResetReason  = UINT16_MAX;
      std::string CommandData;
      std::string HostName;
      uint8_t     NodeID = UINT8_MAX;
      std::string DateTime;
      std::string DNA;
      uint8_t     ValidFlags               = UINT8_MAX;
      bool        LeaseTimeValid           = false;
      bool        HubPortEnableMaskValid   = false;
      bool        SetNewNodeNumber         = false;
      bool        CompareCurrentMacID      = false;
      bool        CompareCurrentNodeNumber = false;
      std::string CurrentMacID             = "";
      uint64_t    HubPortEnableMask        = UINT64_MAX;
      uint32_t    CurrentNodeNumber        = UINT32_MAX;
      uint32_t    NewNodeNumber            = UINT32_MAX;
      nstime_t    LeaseTime                = {0, 0};
    } NMTCmd;


    /* ASnd-->StatusResponse */
    struct StatusResponse final {
      bool              ExceptionNew   = false;
      bool              ExceptionClear = false;
      AsyncSendPriority Priority       = AsyncSendPriority::LOWEST;
      uint8_t           RequestToSend  = UINT8_MAX;

      /* StaticErrorBitField */
      bool        GenericError       = false;
      bool        Current            = false;
      bool        Voltage            = false;
      bool        Temperature        = false;
      bool        CommunicationError = false;
      bool        DeviceProfileSpecB = false;
      bool        ManufacturerSpec   = false;
      std::string DeviceProfileSpec;
    } StatusResponse;

    /* ASnd-->SyncResponse */
    struct SyncResponse final {
      uint8_t SyncResponseID      = UINT8_MAX;
      bool    PResTimeFirstValid  = false;
      bool    PResTimeSecondValid = false;
      bool    PResModeStatus      = false;
      uint8_t Latency             = UINT8_MAX;
      uint8_t SyncDelayStation    = UINT8_MAX;
      uint8_t SyncDelay           = UINT8_MAX;
      uint8_t PResTimeFirst       = UINT8_MAX;
      uint8_t PResTimeSecond      = UINT8_MAX;
      uint8_t StaticErrorBitField = UINT8_MAX;
    } SyncResponse;

    /* ASnd-->SDO */
    struct SDO final {
      struct SEQ final {
        uint8_t              ReceiveSequenceNumber = UINT8_MAX;
        SDOResConnectionType ReceiveCon            = SDOResConnectionType::NO_CONNECTION;
        uint8_t              SendSequenceNumber    = UINT8_MAX;
        SDOSndConnectionType SendCon               = SDOSndConnectionType::NO_CONNECTION;
      } SEQ;

      struct CMD final {
        uint8_t         SDOTransactionID  = UINT8_MAX;
        bool            IsResponse        = false;
        bool            AbortTransfer     = false;
        uint8_t         SubTransfer       = UINT8_MAX;
        SDOSegmentation Segmentation      = SDOSegmentation::EXPEDITED_TRANSFER;
        SDOCommandID    CommandID         = SDOCommandID::NIL;
        uint8_t         SegmentSize       = UINT8_MAX;
        uint8_t         DataSize          = UINT8_MAX;
        uint8_t         DataPadding       = UINT8_MAX;
        uint8_t         TransferAbortCode = UINT8_MAX;
        uint16_t        ODIndex           = UINT16_MAX;
        uint8_t         ODSubIndex        = 0;
        uint16_t        MappingIndex      = UINT16_MAX;
        uint8_t         MappingSubIndex   = UINT8_MAX;
        uint16_t        MappingOffset     = UINT16_MAX;
        uint16_t        MappingLength     = UINT16_MAX;

        bool        FragmentOverlap          = false;
        bool        FragmentOverlapConflicts = false;
        bool        FragmentTailFragments    = false;
        bool        FragmentTooLong          = false;
        uint32_t    FragmentCount            = UINT32_MAX;
        uint8_t     Reassembled              = UINT8_MAX;
        uint32_t    ReassembledLength        = UINT32_MAX;
        std::string ReassembledData;

        PacketDiff data = PacketDiff(UINT16_MAX, UINT8_MAX, UINT64_C(0));
      } CMD;
    } SDO;

  } ASnd;

  int currentLevel = 0;


  parserData()  = default;
  ~parserData() = default;

  parserData(const parserData &) = delete;
  parserData(parserData &&)      = delete;

  parserData &operator=(const parserData &) = delete;
  parserData &operator=(parserData &&) = delete;
};

#if ENABLE_DEBUG_PRINT
void debugFN(parserData *d, field_info *fi, std::string str);
#endif

void foreachFunc(proto_tree *node, gpointer data);
void foreachEPLFunc(proto_tree *node, gpointer data);
void foreachPDOFunc(proto_tree *node, gpointer data);
}
}
