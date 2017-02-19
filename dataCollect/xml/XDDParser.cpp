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
 * \file XDDParser.cpp
 * \brief Contains class XDDParser
 */

#include "XDDParser.hpp"
#include "EPLEnum2Str.hpp"
#include "OD.hpp"
#include <StringHash.hpp>
#include <iostream>
#include <tinyxml2.h>

#if __cplusplus <= 201402L
#include <experimental/filesystem>
namespace fs = std::experimental::filesystem;
#else
#include <filesystem>
namespace fs = std::filesystem;
#endif

using namespace tinyxml2;

namespace EPL_DataCollect {

class XDDVisitorTypes : public XMLVisitor {
 private:
  std::string          path;
  std::stack<uint32_t> strSize;
  OD *                 od;
  ODDescription *      desc;

  std::string currentDTID;

  bool error = false;

 public:
  XDDVisitorTypes() = delete;
  XDDVisitorTypes(OD *odPTR);

  bool VisitEnter(const XMLElement &el, const XMLAttribute *attr) override;
  bool VisitExit(const XMLElement &el) override;

  inline bool getError() const noexcept { return error; }
};


class XDDVisitorEntries : public XMLVisitor {
 private:
  std::string          path;
  std::stack<uint32_t> strSize;
  OD *                 od;
  ODDescription *      desc;

  std::string currentDTID;

  uint16_t lastIndex = 0;
  bool     error     = false;

 public:
  XDDVisitorEntries() = delete;
  XDDVisitorEntries(OD *odPTR);

  void setMiscData(ODEntryDescription *newEntry,
                   const XMLAttribute *accessType,
                   const XMLAttribute *PDOmapping,
                   const XMLAttribute *defaultValue = nullptr,
                   const XMLAttribute *lowLimit     = nullptr,
                   const XMLAttribute *highLimit    = nullptr);

  bool VisitEnter(const XMLElement &el, const XMLAttribute *attr) override;
  bool VisitExit(const XMLElement &el) override;

  inline bool getError() const noexcept { return error; }
};

const std::string pathStrings[] = {
      "ISO15745ProfileContainer.ISO15745Profile.ProfileBody.ApplicationLayers.DataTypeList.defType",
      "ISO15745ProfileContainer.ISO15745Profile.ProfileBody.ApplicationLayers.ObjectList.Object",
      "ISO15745ProfileContainer.ISO15745Profile.ProfileBody.ApplicationLayers.ObjectList.Object.SubObject"};


XDDVisitorTypes::XDDVisitorTypes(OD *odPTR) {
  od   = odPTR;
  desc = od->getODDesc();
  strSize.push(0);
}

XDDVisitorEntries::XDDVisitorEntries(OD *odPTR) {
  od   = odPTR;
  desc = od->getODDesc();
  strSize.push(0);
}

bool XDDVisitorTypes::VisitEnter(const XMLElement &el, const XMLAttribute *) {
  if (error)
    return false;

  std::string t = el.Value();

  if (path == pathStrings[0]) {
    ObjectDataType dt = ObjectDataType::UNDEF;
    switch (hashFunc(t.c_str(), t.size())) {
      case "Boolean"_h: dt        = ObjectDataType::BOOLEAN; break;
      case "Integer8"_h: dt       = ObjectDataType::INTEGER8; break;
      case "Integer16"_h: dt      = ObjectDataType::INTEGER16; break;
      case "Integer32"_h: dt      = ObjectDataType::INTEGER32; break;
      case "Unsigned8"_h: dt      = ObjectDataType::UNSIGNED8; break;
      case "Unsigned16"_h: dt     = ObjectDataType::UNSIGNED16; break;
      case "Unsigned32"_h: dt     = ObjectDataType::UNSIGNED32; break;
      case "Real32"_h: dt         = ObjectDataType::REAL32; break;
      case "Visible_String"_h: dt = ObjectDataType::VISIBLE_STRING; break;
      case "Integer24"_h: dt      = ObjectDataType::INTEGER24; break;
      case "Real64"_h: dt         = ObjectDataType::REAL64; break;
      case "Integer40"_h: dt      = ObjectDataType::INTEGER40; break;
      case "Integer48"_h: dt      = ObjectDataType::INTEGER48; break;
      case "Integer56"_h: dt      = ObjectDataType::INTEGER56; break;
      case "Integer64"_h: dt      = ObjectDataType::INTEGER64; break;
      case "Octet_String"_h: dt   = ObjectDataType::OCTET_STRING; break;
      case "Unicode_String"_h: dt = ObjectDataType::UNICODE_STRING; break;
      case "Time_of_Day"_h: dt    = ObjectDataType::TIME_OF_DAY; break;
      case "Time_Diff"_h: dt      = ObjectDataType::TIME_DIFFERENCE; break;
      case "Domain"_h: dt         = ObjectDataType::DOMAIN_ODT; break;
      case "Unsigned24"_h: dt     = ObjectDataType::UNSIGNED24; break;
      case "Unsigned40"_h: dt     = ObjectDataType::UNSIGNED40; break;
      case "Unsigned48"_h: dt     = ObjectDataType::UNSIGNED48; break;
      case "Unsigned56"_h: dt     = ObjectDataType::UNSIGNED56; break;
      case "Unsigned64"_h: dt     = ObjectDataType::UNSIGNED64; break;
      case "MAC_ADDRESS"_h: dt    = ObjectDataType::MAC_ADDRESS; break;
      case "IP_ADDRESS"_h: dt     = ObjectDataType::IP_ADDRESS; break;
      case "NETTIME"_h: dt        = ObjectDataType::NETTIME; break;
    }

    uint16_t id = static_cast<uint16_t>(std::stoul(currentDTID, nullptr, 16));

    auto *map  = desc->getTypesMap();
    (*map)[id] = dt;
  }

  if (!path.empty()) {
    path.push_back('.');
  }

  path.append(t);
  strSize.push(static_cast<uint32_t>(path.size()));

  if (t == "defType") {
    const XMLAttribute *at = el.FindAttribute("dataType");
    if (!at) {
      error = true;
      return false;
    }
    currentDTID = at->Value();
  }

  return true;
}

void XDDVisitorEntries::setMiscData(ODEntryDescription *newEntry,
                                    const XMLAttribute *accessType,
                                    const XMLAttribute *PDOmapping,
                                    const XMLAttribute *defaultValue,
                                    const XMLAttribute *lowLimit,
                                    const XMLAttribute *highLimit) {
  if (accessType) {
    switch (hashFunc(accessType->Value())) {
      case "const"_h: newEntry->access = ObjectAccess::CONST; break;
      case "ro"_h: newEntry->access    = ObjectAccess::RO; break;
      case "wo"_h: newEntry->access    = ObjectAccess::WO; break;
      case "rw"_h: newEntry->access    = ObjectAccess::RW; break;
      case "wos"_h: newEntry->access   = ObjectAccess::WOS; break;
      case "rws"_h: newEntry->access   = ObjectAccess::RWS; break;
      case "cond"_h: newEntry->access  = ObjectAccess::COND; break;
    }
  }

  if (PDOmapping) {
    switch (hashFunc(PDOmapping->Value())) {
      case "no"_h: newEntry->pdo       = ObjectPDOMapping::NO; break;
      case "default"_h: newEntry->pdo  = ObjectPDOMapping::OPT; break;
      case "optional"_h: newEntry->pdo = ObjectPDOMapping::DEF; break;
      case "TPDO"_h: newEntry->pdo     = ObjectPDOMapping::TPDO; break;
      case "RPDO"_h: newEntry->pdo     = ObjectPDOMapping::RPDO; break;
    }
  }

  if (defaultValue)
    newEntry->defaultValue->setFromString(defaultValue->Value());

  if (lowLimit)
    newEntry->valueMin->setFromString(lowLimit->Value());

  if (highLimit)
    newEntry->valueMax->setFromString(highLimit->Value());

  if (!newEntry->isSubEntry) {
    if (!desc->exists(newEntry->index)) {
      desc->setEntry(newEntry->index, *newEntry);
    } else {
      desc->overrideEntry(newEntry->index, *newEntry);
    }
  } else {
    if (!desc->exists(lastIndex)) {
      std::cerr << "Parent index does not exist" << std::endl;
      error = true;
      return;
    }

    ODEntryDescription *parent = desc->getEntry(lastIndex);
    if (parent->subEntries.size() <= newEntry->index) {
      parent->subEntries.resize(newEntry->index + 1, ODEntryDescription(ObjectType::VAR, newEntry->dataType));
    }

    parent->subEntries[newEntry->index] = (*newEntry);
  }
}

bool XDDVisitorEntries::VisitEnter(const XMLElement &el, const XMLAttribute *) {
  if (error)
    return false;

  std::string t = el.Value();

  if (!path.empty()) {
    path.push_back('.');
  }

  path.append(t);
  strSize.push(static_cast<uint32_t>(path.size()));

  if (path == pathStrings[1]) {
    const XMLAttribute *index        = el.FindAttribute("index");
    const XMLAttribute *name         = el.FindAttribute("name");
    const XMLAttribute *objectType   = el.FindAttribute("objectType");
    const XMLAttribute *dataType     = el.FindAttribute("dataType");
    const XMLAttribute *lowLimit     = el.FindAttribute("lowLimit");
    const XMLAttribute *highLimit    = el.FindAttribute("highLimit");
    const XMLAttribute *accessType   = el.FindAttribute("accessType");
    const XMLAttribute *defaultValue = el.FindAttribute("defaultValue");
    const XMLAttribute *actualValue  = el.FindAttribute("actualValue");
    const XMLAttribute *PDOmapping   = el.FindAttribute("PDOmapping");

    if (!index || !name || !objectType) {
      error = true;
      return false;
    }

    uint16_t       ODIndex    = static_cast<uint16_t>(std::stoul(index->Value(), nullptr, 16));
    std::string    ODName     = name->Value();
    ObjectType     ODObjType  = static_cast<ObjectType>(std::stoul(objectType->Value(), nullptr, 16));
    ObjectDataType ODDataType = ObjectDataType::UNSIGNED64;
    lastIndex                 = ODIndex;

    if (ODObjType == ObjectType::VAR) {
      if (!dataType) {
        error = true;
        return false;
      }

      uint16_t tempIndex = static_cast<uint16_t>(std::stoul(dataType->Value(), nullptr, 16));
      ODDataType         = (*desc->getTypesMap())[tempIndex];
      ODEntryDescription newEntry(ObjectType::VAR, ODDataType);

      newEntry.index = ODIndex;
      newEntry.name  = ODName;

      setMiscData(&newEntry, accessType, PDOmapping, defaultValue, lowLimit, highLimit);

      if (actualValue) {
        ODEntry *entry = od->getEntry(ODIndex);
        if (!entry) {
          error = true;
          return false;
        }
        entry->setFromString(actualValue->Value());
      }

    } else if (ODObjType == ObjectType::ARRAY) {
      if (dataType) {
        uint16_t tempIndex = static_cast<uint16_t>(std::stoul(dataType->Value(), nullptr, 16));
        ODDataType         = (*desc->getTypesMap())[tempIndex];
      }

      ODEntryDescription newEntry(ObjectType::ARRAY, ODDataType);

      newEntry.index = ODIndex;
      newEntry.name  = ODName;

      setMiscData(&newEntry, accessType, PDOmapping);

    } else if (ODObjType == ObjectType::RECORD) {
      ODEntryDescription newEntry(ObjectType::RECORD, ODDataType);

      newEntry.index = ODIndex;
      newEntry.name  = ODName;

      setMiscData(&newEntry, accessType, PDOmapping);
    }


  } else if (path == pathStrings[2]) {
    const XMLAttribute *subIndex     = el.FindAttribute("subIndex");
    const XMLAttribute *name         = el.FindAttribute("name");
    const XMLAttribute *objectType   = el.FindAttribute("objectType");
    const XMLAttribute *dataType     = el.FindAttribute("dataType");
    const XMLAttribute *lowLimit     = el.FindAttribute("lowLimit");
    const XMLAttribute *highLimit    = el.FindAttribute("highLimit");
    const XMLAttribute *accessType   = el.FindAttribute("accessType");
    const XMLAttribute *defaultValue = el.FindAttribute("defaultValue");
    const XMLAttribute *actualValue  = el.FindAttribute("actualValue");
    const XMLAttribute *PDOmapping   = el.FindAttribute("PDOmapping");

    if (!subIndex || !name || !objectType || !dataType) {
      error = true;
      return false;
    }

    uint8_t     ODIndex   = static_cast<uint8_t>(std::stoul(subIndex->Value(), nullptr, 16));
    std::string ODName    = name->Value();
    ObjectType  ODObjType = static_cast<ObjectType>(std::stoul(objectType->Value(), nullptr, 16));

    if (ODObjType != ObjectType::VAR) {
      error = true;
      return false;
    }

    uint16_t           tempIndex  = static_cast<uint16_t>(std::stoul(dataType->Value(), nullptr, 16));
    ObjectDataType     ODDataType = (*desc->getTypesMap())[tempIndex];
    ODEntryDescription newEntry(ObjectType::VAR, ODDataType);

    newEntry.index      = ODIndex;
    newEntry.name       = ODName;
    newEntry.isSubEntry = true;

    setMiscData(&newEntry, accessType, PDOmapping, defaultValue, lowLimit, highLimit);

    if (actualValue) {
      ODEntry *entry = od->getEntry(ODIndex);
      if (!entry) {
        error = true;
        return false;
      }
      entry->setFromString(actualValue->Value(), ODIndex);
    }
  }

  return true;
}

bool XDDVisitorTypes::VisitExit(const tinyxml2::XMLElement &) {
  if (error)
    return false;

  strSize.pop();
  path = path.substr(0, strSize.top());
  return true;
}

bool XDDVisitorEntries::VisitExit(const tinyxml2::XMLElement &) {
  if (error)
    return false;

  strSize.pop();
  path = path.substr(0, strSize.top());
  return true;
}

XDDParser::ErrorCode XDDParser::parseXDD(OD *od, std::string str) {
  fs::path filePath(str);

  if (!fs::exists(filePath))
    return FILE_DOES_NOT_EXIST;

  if (!fs::is_regular_file(filePath))
    return FILE_IS_NOT_A_REGUAR_FILE;


  XMLDocument doc;
  if (doc.LoadFile(str.c_str()) != XML_SUCCESS)
    return FAILED_TO_PARSE_XML;

  XDDVisitorTypes   vis(od);
  XDDVisitorEntries visData(od);
  doc.Accept(&vis);
  doc.Accept(&visData);

  return !(vis.getError() || visData.getError()) ? SUCCESS : FILE_IS_NOT_A_VALID_XDD;
}
}
