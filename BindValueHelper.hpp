///
/// \file bindValueHelper.hpp
/// \author Christian von Arnim
/// \date 15.01.2020
///

#pragma once

#include <open62541/types.h>
#include <open62541/server.h>
#include <open62541/types_generated.h>
#include <Open62541Cpp/UA_RelativPathElement.hpp>
#include <list>
#include <Open62541Cpp/UA_BrowsePath.hpp>
#include "OpcUaTypes/DateTime.hpp"
#include <exception>

#include "BindValue.hpp"
#include "BindStruct.hpp"
#include "BindableMember.hpp"

// Internal implementation for different cases
struct bindValueByPathInternal
{

  template <class T>
  friend void bindValueByPath(
      UA_Server *pServer,
      const open62541Cpp::UA_NodeId &nodeId,
      NodesMaster &nodesMaster,
      T &value);

  template <class T>
  friend void bindVariableByPath(
      UA_Server *pServer,
      const open62541Cpp::UA_NodeId &nodeId,
      NodesMaster &nodesMaster,
      BindableMember<T> &variable);

private:
  // Primitive types including string
  template <typename T>
  static copyToVariantFunc getToVariantFunc(T &value)
  {

    return asVariantFunc(&value);
  }

  template <typename T>
  static copyToVariantFunc getToVariantFuncForEnum(T &value)
  {
    return getToVariantFunc(*reinterpret_cast<std::int32_t *>(&value));
  }

  // capture structs
  template <typename T>
  static copyToVariantFunc bindStructuredValueByPath(
      UA_Server *pServer,
      const open62541Cpp::UA_NodeId &nodeId,
      T &value)
  {
    UA_NodeId typeNodeId;
    UA_NodeId_init(&typeNodeId);
    auto statusCode = UA_Server_readDataType(pServer, *nodeId.NodeId, &typeNodeId);
    if (statusCode != UA_STATUSCODE_GOOD)
    {
      std::cout << "UA_Server_readDataType: Resutl not good: " << UA_StatusCode_name(statusCode) << std::endl;
      throw std::runtime_error("Type could be be fetched, no variable node?");
    }

    const UA_DataType *pDataType = UA_findDataType(&typeNodeId);
    if (pDataType == nullptr)
    {
      std::cout << "NodeId to Datatype failed." << std::endl;
      throw std::runtime_error("NodeId to Datatype failed.");
    }
    if constexpr (refl::trait::is_reflectable<T>::value)
    {
      return bindStructRefl(value, pDataType);
    }
    else
    {
      return bindStruct(
          reinterpret_cast<void *>(&value),
          pDataType);
    }
  }

  template <class T>
  static copyToVariantFunc getToVariantFunc2(
      UA_Server *pServer,
      const open62541Cpp::UA_NodeId &nodeId,
      T &value)
  {
    if constexpr (std::is_enum<T>::value)
    {
      return bindValueByPathInternal::getToVariantFuncForEnum(value);
    }
    else if constexpr (
        std::is_class<T>::value &&
        !(
            std::is_same<std::string, T>::value ||
            std::is_same<open62541Cpp::DateTime_t, T>::value))
    {
      return bindValueByPathInternal::bindStructuredValueByPath(pServer, nodeId, value);
    }
    else
    {
      return bindValueByPathInternal::getToVariantFunc(value);
    }
  }
};

template <class T>
void bindValueByPath(
    UA_Server *pServer,
    const open62541Cpp::UA_NodeId &nodeId,
    NodesMaster &nodesMaster,
    T &value)
{
  copyToVariantFunc toVariantFunc = bindValueByPathInternal::getToVariantFunc2(pServer, nodeId, value);

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
void bindVariableByPath(
    UA_Server *pServer,
    const open62541Cpp::UA_NodeId &nodeId,
    NodesMaster &nodesMaster,
    BindableMember<T> &variable)
{
  copyToVariantFunc toVariantFunc = bindValueByPathInternal::getToVariantFunc2(pServer, nodeId, variable.value);
  auto pVariable = &variable;
  nodesMaster(nodeId) = [toVariantFunc, pVariable] {
    UA_DataValue dataValue;
    UA_DataValue_init(&dataValue);
    toVariantFunc(&dataValue.value);
    dataValue.hasValue = UA_TRUE;
    dataValue.status = pVariable->StatusCode;
    dataValue.hasStatus = UA_TRUE;
    if (pVariable->SourceTimestamp)
    {
      dataValue.hasSourceTimestamp = UA_TRUE;
      dataValue.sourceTimestamp = pVariable->SourceTimestamp.value();
    }
    if (pVariable->ServerTimestamp)
    {
      dataValue.hasServerTimestamp = UA_TRUE;
      dataValue.serverTimestamp = pVariable->ServerTimestamp.value();
    }
    return dataValue;
  };
}
