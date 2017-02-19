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
 * \file enums.hpp
 * \brief Contains all enums used in EPL_DataCollect
 */

#pragma once

#include <stdint.h>

namespace EPL_DataCollect {

/*!
 * \brief The type of an OD entry
 * \note This is NOT the data type (\sa ObjectDataType)
 */
enum class ObjectType {
  NULL_OT   = 0, //!< \brief A dictionary entry with no data fields
  DEFTYPE   = 5, //!< \brief Denotes a static data type definition such as a Boolean, UNSIGNED16, float and so on
  DEFSTRUCT = 6, //!< \brief Defines a record type
  VAR       = 7, //!< \brief A single value such as an UNSIGNED8, Boolean, float, Integer16, visible string etc.

  /*
   * A multiple data field object where each data field is a simple
   * variable of the same basic or extended data type e.g. array of
   * UNSIGNED16 etc. Sub-index 0 is of UNSIGNED8 and therefore
   * not part of the ARRAY data
   */
  ARRAY = 8,

  /*!
   * A multiple data field object where the data fields may be any
   * combination of simple variables. Sub-index 0 is of UNSIGNED8
   * and therefore not part of the RECORD data
   */
  RECORD = 9
};

/*!
 * \brief The category of an OD entry
 */
enum class ObjectCategory {
  MANDATORY,   //!< \brief The device MUST implement this entry
  OPTIONAL,    //!< \brief The device MAY implement this entry
  CONDITIONAL, //!< \brief The device MUST implement this entry based on some condition
  NOT_RELEVANT //!< \brief The entry is not relevant for the MN AND the CN
};

/*!
 * \brief The DATA type of the entry
 */
enum class ObjectDataType {
  UNDEF   = 0x0000,
  BOOLEAN = 0x0001,
  INTEGER8,
  INTEGER16,
  INTEGER32,
  UNSIGNED8,
  UNSIGNED16,
  UNSIGNED32,
  REAL32,
  VISIBLE_STRING,
  OCTET_STRING,
  UNICODE_STRING,
  TIME_OF_DAY,
  TIME_DIFFERENCE,

  DOMAIN_ODT = 0x000F,
  INTEGER24,
  REAL64,
  INTEGER40,
  INTEGER48,
  INTEGER56,
  INTEGER64,
  UNSIGNED24,
  UNSIGNED40 = 0x0018,
  UNSIGNED48,
  UNSIGNED56,
  UNSIGNED64,

  IDENTITY                                       = 0x0023,
  BEGIN_MANUFACTURER_SPECIFIC_COMPLEX_DATA_TYPES = 0x0040,
  END_MANUFACTURER_SPECIFIC_COMPLEX_DATA_TYPES   = 0x005F,

  BEGIN_DEVICE_PROFILE_0_SPECIFIC_STANDARD_DATA_TYPES = 0x0060,
  END_DEVICE_PROFILE_0_SPECIFIC_STANDARD_DATA_TYPES   = 0x007F,
  BEGIN_DEVICE_PROFILE_0_SPECIFIC_COMPLEX_DATA_TYPES  = 0x0080,
  END_DEVICE_PROFILE_0_SPECIFIC_COMPLEX_DATA_TYPES    = 0x009F,

  BEGIN_DEVICE_PROFILE_1_SPECIFIC_STANDARD_DATA_TYPES = 0x00A0,
  END_DEVICE_PROFILE_1_SPECIFIC_STANDARD_DATA_TYPES   = 0x00BF,
  BEGIN_DEVICE_PROFILE_1_SPECIFIC_COMPLEX_DATA_TYPES  = 0x00C0,
  END_DEVICE_PROFILE_1_SPECIFIC_COMPLEX_DATA_TYPES    = 0x00DF,

  BEGIN_DEVICE_PROFILE_2_SPECIFIC_STANDARD_DATA_TYPES = 0x00E0,
  END_DEVICE_PROFILE_2_SPECIFIC_STANDARD_DATA_TYPES   = 0x00FF,
  BEGIN_DEVICE_PROFILE_2_SPECIFIC_COMPLEX_DATA_TYPES  = 0x0100,
  END_DEVICE_PROFILE_2_SPECIFIC_COMPLEX_DATA_TYPES    = 0x011F,

  BEGIN_DEVICE_PROFILE_3_SPECIFIC_STANDARD_DATA_TYPES = 0x0120,
  END_DEVICE_PROFILE_3_SPECIFIC_STANDARD_DATA_TYPES   = 0x013F,
  BEGIN_DEVICE_PROFILE_3_SPECIFIC_COMPLEX_DATA_TYPES  = 0x0140,
  END_DEVICE_PROFILE_3_SPECIFIC_COMPLEX_DATA_TYPES    = 0x015F,

  BEGIN_DEVICE_PROFILE_4_SPECIFIC_STANDARD_DATA_TYPES = 0x0160,
  END_DEVICE_PROFILE_4_SPECIFIC_STANDARD_DATA_TYPES   = 0x017F,
  BEGIN_DEVICE_PROFILE_4_SPECIFIC_COMPLEX_DATA_TYPES  = 0x0180,
  END_DEVICE_PROFILE_4_SPECIFIC_COMPLEX_DATA_TYPES    = 0x019F,

  BEGIN_DEVICE_PROFILE_5_SPECIFIC_STANDARD_DATA_TYPES = 0x01A0,
  END_DEVICE_PROFILE_5_SPECIFIC_STANDARD_DATA_TYPES   = 0x01BF,
  BEGIN_DEVICE_PROFILE_5_SPECIFIC_COMPLEX_DATA_TYPES  = 0x01C0,
  END_DEVICE_PROFILE_5_SPECIFIC_COMPLEX_DATA_TYPES    = 0x01DF,

  BEGIN_DEVICE_PROFILE_6_SPECIFIC_STANDARD_DATA_TYPES = 0x01E0,
  END_DEVICE_PROFILE_6_SPECIFIC_STANDARD_DATA_TYPES   = 0x01FF,
  BEGIN_DEVICE_PROFILE_6_SPECIFIC_COMPLEX_DATA_TYPES  = 0x0200,
  END_DEVICE_PROFILE_6_SPECIFIC_COMPLEX_DATA_TYPES    = 0x021F,

  BEGIN_DEVICE_PROFILE_7_SPECIFIC_STANDARD_DATA_TYPES = 0x0220,
  END_DEVICE_PROFILE_7_SPECIFIC_STANDARD_DATA_TYPES   = 0x023F,
  BEGIN_DEVICE_PROFILE_7_SPECIFIC_COMPLEX_DATA_TYPES  = 0x0240,
  END_DEVICE_PROFILE_7_SPECIFIC_COMPLEX_DATA_TYPES    = 0x025F,

  MAC_ADDRESS = 0x0401,
  IP_ADDRESS,
  NETTIME,

  PDO_CommParamRecord_TYPE = 0x0420,
  // SKIP
  SDO_ParameterRecord_TYPE = 0x0422,
  // SKIP
  DLL_ErrorCntRec_TYPE      = 0x0424,
  NWL_IpGroup_TYPE          = 0x0425,
  NWL_IpAddrTable_TYPE      = 0x0426,
  PDL_LocVerApplSw_TYPE     = 0x0427,
  INP_ProcessImage_TYPE     = 0x0428,
  NMT_ParameterStorage_TYPE = 0x0429,
  // SKIP
  NMT_InterfaceGroup_Xh_TYPE = 0x042B,
  NMT_CycleTiming_TYPE       = 0x042C,
  // SKIP
  NMT_BootTime_TYPE            = 0x042E,
  NMT_MNCycleTiming_TYPE       = 0x042F,
  RT1_EplRouter_TYPE           = 0x0430,
  RT1_IpRoutingTable_TYPE      = 0x0431,
  RT1_NatTable_TYPE            = 0x0432,
  RT1_SecurityGroup_TYPE       = 0x0433,
  RT1_AclTable_TYPE            = 0x0434,
  CFM_VerifyConfiguration_TYPE = 0x0435,
  // SKIP
  DIA_NMTTelegrCount_TYPE   = 0x0437,
  DIA_ERRStatistics_TYPE    = 0x0438,
  NMT_EPLNodeID_TYPE        = 0x0439,
  NMT_RequestCmd_TYPE       = 0x043A,
  DLL_MNRingRedundancy_TYPE = 0x043B,
};

/*!
 * \brief Describes which class to use
 */
enum class ObjectClassType {
  INTEGER,
  UNSIGNED,
  BOOL,
  REAL,
  STRING,
  ARRAY_INTEGER,
  ARRAY_UNSIGNED,
  ARRAY_BOOL,
  ARRAY_REAL,
  COMPLEX
};

/*!
 * \brief Describes how the Entry can be accessed
 */
enum class ObjectAccess {
  RW,    //!< \brief read, write access, value shall not be stored on writing sub-indices of NMT_StoreParam_REC
  RWS,   //!< \brief read, write access, value shall be stored on writing sub-indices of NMT_StoreParam_REC
  WO,    //!< \brief write only access, value shall not be stored on writing sub-indices of NMT_StoreParam_REC
  WOS,   //!< \brief write only access, value shall be stored on writing sub-indices of NMT_StoreParam_REC
  RO,    //!< \brief read only access
  CONST, //!< \brief read only access, value is constant
  COND   //!< \brief variable access controlled by the device. Further information is provided by the description
};

/*!
 * \brief Describes how the Entry can be mapped
 */
enum class ObjectPDOMapping {
  OPT,  //!< \brief Object shall be mappable into a PDO
  DEF,  //!< \brief Object is part of the default mapping (see device profile)
  NO,   //!< \brief Object shall not be mappable into a PDO
  TPDO, //!< \brief Object may be mapped into TPDO only
  RPDO  //!< \brief Object may be mapped into RPDO only
};



/*!
 * \brief The type of an event
 */
enum class EvType {
  PROTO_ERROR,
  ERROR,
  WARNING,
  INFO,
  DEBUG,
  PLUGIN_EV_TEXT,

  PLUGIN_OTHER = 0x1000,

  FRONTEND_OTHER = 0x2000,

  UNKNOWN = INT32_MAX
};

/*!
 * \brief The status of the node
 * \todo Check if more values are needed
 */
enum class NodeStatus { OK, ERROR, STARTING, UNKNOWN };

/*!
 * \brief The type of a packet
 */
enum class PacketType {
  UNDEF          = 0x00,
  START_OF_CYCLE = 0x01,
  POLL_REQUEST   = 0x03,
  POLL_RESPONSE  = 0x04,
  START_OF_ASYNC = 0x05,
  ASYNC_SEND     = 0x06,
  AMNI           = 0x07,
  AINV           = 0x0D
};

/*!
 * \brief ASnd Service ID
 */
enum class ASndServiceID {
  RESERVED_0                  = 0x00,
  IDENT_RESPONSE              = 0x01,
  STATUS_RESPONSE             = 0x02,
  NMT_REQUEST                 = 0x03,
  NMT_COMMAND                 = 0x04,
  SDO                         = 0x05,
  BEGIN_MANUFACTURER_SPECIFIC = 0xA0,
  END_MANUFACTURER_SPECIFIC   = 0xFE,
};

/*!
 * \brief ASnd Service ID
 */
enum class SoARequestServiceID {
  NO_SERVICE                  = 0x00,
  IDENT_REQUEST               = 0x01,
  STATUS_REQUEST              = 0x02,
  NMT_REQUEST_INVITE          = 0x03,
  BEGIN_MANUFACTURER_SPECIFIC = 0xA0,
  END_MANUFACTURER_SPECIFIC   = 0xFE,
  UNSPECIVIED_INVITE          = 0xFF
};

/*!
 * \brief NMT Status
 */
enum class NMTState {
  OFF                 = 0b00000000,
  INITIALISING        = 0b00011001,
  RESET_APPLICATION   = 0b00101001,
  RESET_COMMUNICATION = 0b00111001,
  RESET_CONFIGURATION = 0b01111001,
  NOT_ACTIVE          = 0b00011100,
  PRE_OPERATIONAL_1   = 0b00011101,
  PRE_OPERATIONAL_2   = 0b01011101,
  READY_TO_OPERATE    = 0b01101101,
  OPERATIONAL         = 0b11111101,
  STOPPED             = 0b01001101,
  BASIC_ETHERNET      = 0b00011110
};

enum class NMTCommand {
  IDENT_REQUEST              = 0x1,
  STATUS_REQUEST             = 0x2,
  START_NODE                 = 0x21,
  STOP_NODE                  = 0x22,
  ENTER_PRE_OP2              = 0x23,
  ENABLE_READY_TO_OPERATE    = 0x24,
  RESET_NODE                 = 0x28,
  RESET_COMMUNICATION        = 0x29,
  RESET_CONFIGURATION        = 0x2A,
  SW_RESET                   = 0x2B,
  GO_TO_STANDBY              = 0x2C,
  START_NODE_EX              = 0x41,
  STOP_NODE_EX               = 0x42,
  ENTER_PRE_OP2_EX           = 0x43,
  ENABLE_READY_TO_OPERATE_EX = 0x44,
  RESET_NODE_EX              = 0x48,
  RESET_COMMUNICATION_EX     = 0x49,
  RESET_CONFIGURATION_EX     = 0x4A,
  SW_RESET_EX                = 0x4B,
  NET_HOST_NAME_SET          = 0x62,
  FLUSH_ARP_ENTRY            = 0x63,
  PUBLISH_CONFIGURED_NODES   = 0x80,
  PUBLISH_ACTIVE_NODES       = 0x90,
  PUBLISH_PRE_OPERATIONAL1   = 0x91,
  PUBLISH_PRE_OPERATIONAL2   = 0x92,
  PUBLISH_READY_TO_OPERATE   = 0x93,
  PUBLISH_OPERATIONAL        = 0x94,
  PUBLISH_STOPPED            = 0x95,
  PUBLISH_NODE_STATES        = 0x96,
  PUBLISH_EMERGENCY_NEW      = 0xA0,
  PUBLISH_PUBLISHED_TIME     = 0xB0,
  INVALID_SERVICE            = 0xFF
};

/*!
 * \brief AsyncSend Request Prioities
 */
enum class AsyncSendPriority {
  HIGHEST = 0b111,
  HIGHER3 = 0b110,
  HIGHER2 = 0b101,
  HIGHER1 = 0b100,
  MEDIUM  = 0b011,
  LOWER2  = 0b010,
  LOWER1  = 0b001,
  LOWEST  = 0b000
};

enum class SDOResConnectionType { NO_CONNECTION = 0, INITIALISATION = 1, CONNECTION_VALID = 2, ERROR_RESPONSE = 3 };


enum class SDOSndConnectionType {
  NO_CONNECTION        = 0,
  INITIALISATION       = 1,
  CONNECTION_VALID     = 2,
  CONNECTION_VALID_WAR = 3
};

enum class SDOSegmentation { EXPEDITED_TRANSFER = 0, INITIATE_SGE_TRANSFER = 1, SEGMENT = 2, END_SEG_TRANSFER = 3 };

/*!
 * \brief Command ID
 */
enum class SDOCommandID {
  NIL                            = 0x00,
  WRITE_BY_INDEX                 = 0x01,
  READ_BY_INDEX                  = 0x02,
  WRITE_ALL_BY_INDEX             = 0x03,
  READ_ALL_BY_INDEX              = 0x04,
  WRITE_BY_NAME                  = 0x05,
  READ_BY_NAME                   = 0x06,
  FILE_WRITE                     = 0x20,
  FILE_READ                      = 0x21,
  WRITE_MULTIPLE_PARAMETER_INDEX = 0x31,
  READ_MULTIPLE_PARAMETER_INDEX  = 0x32,
  MAXIMUM_SEGMENT_SIZE           = 0x70,
  BEGIN_MANUFACTURER_SPECIFIC    = 0x80,
  END_MANUFACTURER_SPECIFIC      = 0xFF
};
}
