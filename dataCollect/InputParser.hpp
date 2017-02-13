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
  Packet::TIME_POINT  tp          = std::chrono::system_clock::now();
  uint8_t             transactID  = 0;
  uint32_t            numSegments = 0;

  // (mostly) SoA specific
  uint8_t requestedServiceTarget   = UINT8_MAX;
  uint8_t eplVersion               = UINT8_MAX;
  uint8_t syncControl              = UINT8_MAX;
  bool    exceptionReset           = false;
  bool    exceptionAcknoledged     = false;
  bool    ANGlobal                 = false;
  bool    ANLocal                  = false;
  bool    destMacAddressValid      = false;
  bool    destMacAddress           = false;
  bool    PResFallBackTimeoutValid = false;
  bool    SyncMNDelaySecondValid   = false;
  bool    SyncMNDelayFirstValid    = false;
  bool    PResTimeSecondValid      = false;
  bool    PResTimeFirstValid       = false;
  bool    PResModeSet              = false;
  bool    PResModeReset            = false;

  // (mostly) PReq and PRes
  bool multiplexedSlot = false;

  // (mostly) PRes data
  bool              exceptionNew = false;
  AsyncSendPriority sendPriority = ASSP_LOWEST; // also ASnd-->IdentResponse

  // ASnd header
  ASndServiceID sID = ASND_RESERVED_0;

  // ASnd-->IdentResponse
  bool exceptionClear = false;
};

#if ENABLE_DEBUG_PRINT
void debugFN(parserData *d, field_info *fi, std::string str);
#endif

void foreachFunc(proto_tree *node, gpointer data);
void foreachEPLFunc(proto_tree *node, gpointer data);

void bindUINT8(parserData *d, field_info *fi, uint8_t &val);
void bindBOOL(parserData *d, field_info *fi, bool &val);

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
