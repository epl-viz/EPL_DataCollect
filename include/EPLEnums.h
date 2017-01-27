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

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \brief The type of an OD entry
 * \note This is NOT the data type (\sa ObjectDataType)
 */
enum ObjectType {
  OT_NULL      = 0, //!< \brief A dictionary entry with no data fields
  OT_DEFTYPE   = 5, //!< \brief Denotes a static data type definition such as a Boolean, UNSIGNED16, float and so on
  OT_DEFSTRUCT = 6, //!< \brief Defines a record type
  OT_VAR       = 7, //!< \brief A single value such as an UNSIGNED8, Boolean, float, Integer16, visible string etc.

  /*
   * A multiple data field object where each data field is a simple
   * variable of the same basic or extended data type e.g. array of
   * UNSIGNED16 etc. Sub-index 0 is of UNSIGNED8 and therefore
   * not part of the ARRAY data
   */
  OT_ARRAY = 8,

  /*!
   * A multiple data field object where the data fields may be any
   * combination of simple variables. Sub-index 0 is of UNSIGNED8
   * and therefore not part of the RECORD data
   */
  OT_RECORD = 9
};

/*!
 * \brief The category of an OD entry
 */
enum ObjectCategory {
  OC_MANDATORY,   //!< \brief The device MUST implement this entry
  OC_OPTIONAL,    //!< \brief The device MAY implement this entry
  OC_CONDITIONAL, //!< \brief The device MUST implement this entry based on some condition
  OC_NOT_RELEVANT //!< \brief The entry is not relevant for the MN AND the CN
};

/*!
 * \brief The DATA type of the entry
 */
enum ObjectDataType {
  ODT_BOOLEAN = 0x0001,
  ODT_INTEGER8,
  ODT_INTEGER16,
  ODT_INTEGER32,
  ODT_UNSIGNED8,
  ODT_UNSIGNED16,
  ODT_UNSIGNED32,
  ODT_REAL32,
  ODT_VISIBLE_STRING,
  ODT_OCTET_STRING,
  ODT_UNICODE_STRING,
  ODT_TIME_OF_DAY,
  ODT_TIME_DIFFERENCE,

  ODT_DOMAIN = 0x000F,
  ODT_INTEGER24,
  ODT_REAL64,
  ODT_INTEGER40,
  ODT_INTEGER48,
  ODT_INTEGER56,
  ODT_INTEGER64,
  ODT_UNSIGNED24,
  ODT_UNSIGNED40 = 0x0018,
  ODT_UNSIGNED48,
  ODT_UNSIGNED56,
  ODT_UNSIGNED64,

  ODT_IDENTITY                                       = 0x0023,
  ODT_BEGIN_MANUFACTURER_SPECIFIC_COMPLEX_DATA_TYPES = 0x0040,
  ODT_END_MANUFACTURER_SPECIFIC_COMPLEX_DATA_TYPES   = 0x005F,

  ODT_BEGIN_DEVICE_PROFILE_0_SPECIFIC_STANDARD_DATA_TYPES = 0x0060,
  ODT_END_DEVICE_PROFILE_0_SPECIFIC_STANDARD_DATA_TYPES   = 0x007F,
  ODT_BEGIN_DEVICE_PROFILE_0_SPECIFIC_COMPLEX_DATA_TYPES  = 0x0080,
  ODT_END_DEVICE_PROFILE_0_SPECIFIC_COMPLEX_DATA_TYPES    = 0x009F,

  ODT_BEGIN_DEVICE_PROFILE_1_SPECIFIC_STANDARD_DATA_TYPES = 0x00A0,
  ODT_END_DEVICE_PROFILE_1_SPECIFIC_STANDARD_DATA_TYPES   = 0x00BF,
  ODT_BEGIN_DEVICE_PROFILE_1_SPECIFIC_COMPLEX_DATA_TYPES  = 0x00C0,
  ODT_END_DEVICE_PROFILE_1_SPECIFIC_COMPLEX_DATA_TYPES    = 0x00DF,

  ODT_BEGIN_DEVICE_PROFILE_2_SPECIFIC_STANDARD_DATA_TYPES = 0x00E0,
  ODT_END_DEVICE_PROFILE_2_SPECIFIC_STANDARD_DATA_TYPES   = 0x00FF,
  ODT_BEGIN_DEVICE_PROFILE_2_SPECIFIC_COMPLEX_DATA_TYPES  = 0x0100,
  ODT_END_DEVICE_PROFILE_2_SPECIFIC_COMPLEX_DATA_TYPES    = 0x011F,

  ODT_BEGIN_DEVICE_PROFILE_3_SPECIFIC_STANDARD_DATA_TYPES = 0x0120,
  ODT_END_DEVICE_PROFILE_3_SPECIFIC_STANDARD_DATA_TYPES   = 0x013F,
  ODT_BEGIN_DEVICE_PROFILE_3_SPECIFIC_COMPLEX_DATA_TYPES  = 0x0140,
  ODT_END_DEVICE_PROFILE_3_SPECIFIC_COMPLEX_DATA_TYPES    = 0x015F,

  ODT_BEGIN_DEVICE_PROFILE_4_SPECIFIC_STANDARD_DATA_TYPES = 0x0160,
  ODT_END_DEVICE_PROFILE_4_SPECIFIC_STANDARD_DATA_TYPES   = 0x017F,
  ODT_BEGIN_DEVICE_PROFILE_4_SPECIFIC_COMPLEX_DATA_TYPES  = 0x0180,
  ODT_END_DEVICE_PROFILE_4_SPECIFIC_COMPLEX_DATA_TYPES    = 0x019F,

  ODT_BEGIN_DEVICE_PROFILE_5_SPECIFIC_STANDARD_DATA_TYPES = 0x01A0,
  ODT_END_DEVICE_PROFILE_5_SPECIFIC_STANDARD_DATA_TYPES   = 0x01BF,
  ODT_BEGIN_DEVICE_PROFILE_5_SPECIFIC_COMPLEX_DATA_TYPES  = 0x01C0,
  ODT_END_DEVICE_PROFILE_5_SPECIFIC_COMPLEX_DATA_TYPES    = 0x01DF,

  ODT_BEGIN_DEVICE_PROFILE_6_SPECIFIC_STANDARD_DATA_TYPES = 0x01E0,
  ODT_END_DEVICE_PROFILE_6_SPECIFIC_STANDARD_DATA_TYPES   = 0x01FF,
  ODT_BEGIN_DEVICE_PROFILE_6_SPECIFIC_COMPLEX_DATA_TYPES  = 0x0200,
  ODT_END_DEVICE_PROFILE_6_SPECIFIC_COMPLEX_DATA_TYPES    = 0x021F,

  ODT_BEGIN_DEVICE_PROFILE_7_SPECIFIC_STANDARD_DATA_TYPES = 0x0220,
  ODT_END_DEVICE_PROFILE_7_SPECIFIC_STANDARD_DATA_TYPES   = 0x023F,
  ODT_BEGIN_DEVICE_PROFILE_7_SPECIFIC_COMPLEX_DATA_TYPES  = 0x0240,
  ODT_END_DEVICE_PROFILE_7_SPECIFIC_COMPLEX_DATA_TYPES    = 0x025F,

  ODT_MAC_ADDRESS = 0x0401,
  ODT_IP_ADDRESS,
  ODT_NETTIME,

  ODT_PDO_CommParamRecord_TYPE = 0x0420,
  // SKIP
  ODT_SDO_ParameterRecord_TYPE = 0x0422,
  // SKIP
  ODT_DLL_ErrorCntRec_TYPE      = 0x0424,
  ODT_NWL_IpGroup_TYPE          = 0x0425,
  ODT_NWL_IpAddrTable_TYPE      = 0x0426,
  ODT_PDL_LocVerApplSw_TYPE     = 0x0427,
  ODT_INP_ProcessImage_TYPE     = 0x0428,
  ODT_NMT_ParameterStorage_TYPE = 0x0429,
  // SKIP
  ODT_NMT_InterfaceGroup_Xh_TYPE = 0x042B,
  ODT_NMT_CycleTiming_TYPE       = 0x042C,
  // SKIP
  ODT_NMT_BootTime_TYPE            = 0x042E,
  ODT_NMT_MNCycleTiming_TYPE       = 0x042F,
  ODT_RT1_EplRouter_TYPE           = 0x0430,
  ODT_RT1_IpRoutingTable_TYPE      = 0x0431,
  ODT_RT1_NatTable_TYPE            = 0x0432,
  ODT_RT1_SecurityGroup_TYPE       = 0x0433,
  ODT_RT1_AclTable_TYPE            = 0x0434,
  ODT_CFM_VerifyConfiguration_TYPE = 0x0435,
  // SKIP
  ODT_DIA_NMTTelegrCount_TYPE   = 0x0437,
  ODT_DIA_ERRStatistics_TYPE    = 0x0438,
  ODT_NMT_EPLNodeID_TYPE        = 0x0439,
  ODT_NMT_RequestCmd_TYPE       = 0x043A,
  ODT_DLL_MNRingRedundancy_TYPE = 0x043B,
};

/*!
 * \brief Describes how the Entry can be accessed
 */
enum ObjectAccess {
  OACS_RW,    //!< \brief read, write access, value shall not be stored on writing sub-indices of NMT_StoreParam_REC
  OACS_RWS,   //!< \brief read, write access, value shall be stored on writing sub-indices of NMT_StoreParam_REC
  OACS_WO,    //!< \brief write only access, value shall not be stored on writing sub-indices of NMT_StoreParam_REC
  OACS_WOS,   //!< \brief write only access, value shall be stored on writing sub-indices of NMT_StoreParam_REC
  OACS_RO,    //!< \brief read only access
  OACS_CONST, //!< \brief read only access, value is constant
  OACS_COND   //!< \brief variable access controlled by the device. Further information is provided by the description
};

/*!
 * \brief Describes how the Entry can be mapped
 */
enum ObjectPDOMapping {
  OPDO_OPT, //!< \brief Object shall be mappable into a PDO
  OPDO_DEF, //!< \brief Object is part of the default mapping (see device profile)
  OPDO_NO   //!< \brief Object shall not be mappable into a PDO
};



/*!
 * \brief The type of an event
 */
enum EvType {
  EVT_PROTO_ERROR,
  EVT_ERROR,
  EVT_WARNING,
  EVT_INFO,
  EVT_DEBUG,
  EVT_PLUGIN_EV_TEXT,

  EVT_PLUGIN_OTHER = 0x1000,

  EVT_FRONTEND_OTHER = 0x2000,

  EVT_UNKNOWN = INT32_MAX
};

#ifdef __cplusplus
}
#endif