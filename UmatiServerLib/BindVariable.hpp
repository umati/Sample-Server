/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2020-2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 * Copyright 2021 (c) Christoph Ruckstetter, Michael Weinig AG
 * Copyright 2022 (c) Sebastian Friedl, ISW University of Stuttgart (for VDMA e.V.)
 */

#pragma once

#include <open62541/server.h>
#include <open62541/types.h>
#include <open62541/types_generated.h>

#include <Open62541Cpp/UA_BrowsePath.hpp>
#include <Open62541Cpp/UA_NodeId.hpp>
#include <Open62541Cpp/UA_RelativPathElement.hpp>
#include <exception>
#include <list>

#include "../OpcUaTypes/DateTime.hpp"
#include "../OpcUaTypes/StatusCode.hpp"
#include "../OpcUaTypes/LocalizedText.hpp"
#include "BindableMemberValue.hpp"
#include "ConvertSimpleValue.hpp"
#include "ConvertStructValue.hpp"
#include "NodesMaster.hpp"
#include "TypeToUaDatatype.hpp"
#include "Util.hpp"

namespace UmatiServerLib {
class BindVariable {
 public:
  template <class T>
  static void ToNode(UA_Server *pServer, const open62541Cpp::UA_NodeId &nodeId, NodesMaster &nodesMaster, T &value);

  template <class T>
  static void ToNode(UA_Server *pServer, const open62541Cpp::UA_NodeId &nodeId, NodesMaster &nodesMaster, BindableMemberValue<T> &variable);

 protected:
  // Primitive types including string
  template <typename T>
  static copyToVariantFunc GetToVariantFunc(T &value) {
    return ConvertSimpleValue::asVariantFunc(&value);
  }

  template <typename T>
  static copyToVariantFunc getToVariantFuncArray(std::vector<T> &value) {
    return ConvertSimpleValue::asVariantFuncArray(&value);
    return nullptr;
  }

  template <typename T>
  static copyToVariantFunc getToVariantFuncForEnum(T &value) {
    return GetToVariantFunc(*reinterpret_cast<std::int32_t *>(&value));
  }

  template <typename T>
  static copyToVariantFunc getToVariantFuncForEnumArray(std::vector<T> &value) {
    return getToVariantFuncArray(*reinterpret_cast<std::vector<std::int32_t> *>(&value));
  }

  // Handle structs
  template <typename T>
  static copyToVariantFunc bindStructuredValueByPath(UA_Server *pServer, const open62541Cpp::UA_NodeId &nodeId, T &value) {
    const UA_DataType *pDataType = UmatiServerLib::TypeToUaDatatype::GetType<std::remove_reference_t<T>>();

    if (pDataType == nullptr) {
      UA_NodeId typeNodeId;
      UA_NodeId_init(&typeNodeId);
      auto statusCode = UA_Server_readDataType(pServer, *nodeId.NodeId, &typeNodeId);
      if (statusCode != UA_STATUSCODE_GOOD) {
        std::cout << "UA_Server_readDataType(" << nodeId << "): Result not good: " << UA_StatusCode_name(statusCode) << std::endl;
        throw std::runtime_error("Type could not be be fetched, no variable node?");
      }
      pDataType = UA_Server_findDataType(pServer, &typeNodeId);
    }
    if (pDataType == nullptr) {
      throw std::runtime_error("Fetching Datatype failed.");
    }
    ///\todo check attribute and make sure this is a datatype!
    if constexpr (refl::trait::is_reflectable<T>::value) {
      return ConvertStructValue::GetToVariantFunc(value, pDataType);
    } else {
      return ConvertStructValue::GetToVariantFunc(reinterpret_cast<void *>(&value), pDataType);
    }
  }

  // Handle struct arrays
  template <typename T>
  static copyToVariantFunc bindStructuredValueByPathArray(UA_Server *pServer, const open62541Cpp::UA_NodeId &nodeId, std::vector<T> &value) {
    UA_NodeId typeNodeId;
    UA_NodeId_init(&typeNodeId);
    auto statusCode = UA_Server_readDataType(pServer, *nodeId.NodeId, &typeNodeId);
    if (statusCode != UA_STATUSCODE_GOOD) {
      std::cout << "UA_Server_readDataType: Result not good: " << UA_StatusCode_name(statusCode) << std::endl;
      throw std::runtime_error("Type could not be be fetched, no variable node?");
    }

    const UA_DataType *pDataType = UA_findDataType(&typeNodeId);
    if (pDataType == nullptr) {
      std::cout << "NodeId to Datatype failed." << std::endl;
      throw std::runtime_error("NodeId to Datatype failed.");
    }
    if constexpr (refl::trait::is_reflectable<T>::value) {
      return bindStructReflArray(value, pDataType);
    } else {
      return ConvertStructValue::GetToVariantFunc(&value, pDataType);
    }
  }

  template <class T>
  static copyToVariantFunc getToVariantFunc2(UA_Server *pServer, const open62541Cpp::UA_NodeId &nodeId, std::vector<T> &value) {
    if constexpr (std::is_enum<T>::value) {
      return getToVariantFuncForEnumArray(value);
    } else if constexpr (
      std::is_class<T>::value && !(std::is_same<std::string, T>::value || std::is_same<UmatiServerLib::StatusCode_t, T>::value ||
                                   std::is_same<UmatiServerLib::DateTime_t, T>::value || std::is_same<UmatiServerLib::LocalizedText_t, T>::value)) {
      return bindStructuredValueByPathArray(pServer, nodeId, value);
    } else {
      return getToVariantFuncArray(value);
    }
  }

  template <class T>
  static copyToVariantFunc getToVariantFunc2(UA_Server *pServer, const open62541Cpp::UA_NodeId &nodeId, T &value) {
    if constexpr (std::is_enum<T>::value) {
      return getToVariantFuncForEnum(value);
    } else if constexpr (
      std::is_class<T>::value &&
      !(std::is_same<std::string, T>::value || std::is_same<UmatiServerLib::StatusCode_t, T>::value || std::is_same<UmatiServerLib::DateTime_t, T>::value)) {
      return bindStructuredValueByPath(pServer, nodeId, value);
    } else {
      return GetToVariantFunc(value);
    }
  }
};

template <class T>
void BindVariable::ToNode(UA_Server *pServer, const open62541Cpp::UA_NodeId &nodeId, NodesMaster &nodesMaster, T &value) {
  copyToVariantFunc toVariantFunc = getToVariantFunc2(pServer, nodeId, value);

  nodesMaster(nodeId) = [toVariantFunc] {
    UA_DataValue dataValue;
    UA_DataValue_init(&dataValue);
    toVariantFunc(&dataValue.value);
    dataValue.hasValue = UA_TRUE;
    dataValue.status = UA_STATUSCODE_GOOD;
    dataValue.hasStatus = UA_TRUE;
    return dataValue;
  };
}

template <class T>
void BindVariable::ToNode(UA_Server *pServer, const open62541Cpp::UA_NodeId &nodeId, NodesMaster &nodesMaster, BindableMemberValue<T> &variable) {
  copyToVariantFunc toVariantFunc = getToVariantFunc2(pServer, nodeId, variable.value);
  auto pVariable = &variable;
  nodesMaster(nodeId) = [toVariantFunc, pVariable] {
    UA_DataValue dataValue;
    UA_DataValue_init(&dataValue);
    toVariantFunc(&dataValue.value);
    dataValue.hasValue = UA_TRUE;
    dataValue.status = pVariable->StatusCode;
    dataValue.hasStatus = UA_TRUE;
    if (pVariable->SourceTimestamp) {
      dataValue.hasSourceTimestamp = UA_TRUE;
      dataValue.sourceTimestamp = pVariable->SourceTimestamp.value();
    }
    if (pVariable->ServerTimestamp) {
      dataValue.hasServerTimestamp = UA_TRUE;
      dataValue.serverTimestamp = pVariable->ServerTimestamp.value();
    }
    return dataValue;
  };
}

}  // namespace UmatiServerLib
