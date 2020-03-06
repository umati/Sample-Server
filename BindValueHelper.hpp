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

// Internal implementation for different cases
struct bindValueByPathInternal
{

  template <class T>
  friend void bindValueByPath(
      UA_Server *pServer,
      const open62541Cpp::UA_BrowsePath &brPath,
      NodesMaster &nodesMaster,
      T &value);

  static open62541Cpp::UA_NodeId resolveBrowsePath(
      UA_Server *pServer,
      const open62541Cpp::UA_BrowsePath &brPath)
  {
    // No Elements in browse path.
    if(brPath.BrowsePath->relativePath.elementsSize == 0)
    {
      return open62541Cpp::UA_NodeId(brPath.BrowsePath->startingNode);
    }

    auto trResult =
        UA_Server_translateBrowsePathToNodeIds(
            pServer,
            brPath.BrowsePath);

    if (trResult.statusCode != UA_STATUSCODE_GOOD)
    {
      std::cout << "Resutl not good: " << UA_StatusCode_name(trResult.statusCode) << std::endl;
      std::cout << "Could not resolve Path: " << static_cast<std::string>(brPath) << std::endl;

      throw std::invalid_argument("Path not found.");
    }

    if (trResult.targetsSize != 1)
    {
      std::cout << "Unexpected number of results, expect 1, got: " << trResult.targetsSize << std::endl;
      throw std::invalid_argument("Unexpected number of results.");
    }
    open62541Cpp::UA_NodeId ret(trResult.targets[0].targetId.nodeId);
    UA_BrowsePathResult_deleteMembers(&trResult);
    return ret;
  }

private:
  // Primitive types including string
  template <typename T>
  static void bindValueByPath(
      UA_Server *pServer,
      const open62541Cpp::UA_BrowsePath &brPath,
      NodesMaster &nodesMaster,
      T &value)
  {
    bindValue(
        nodesMaster(resolveBrowsePath(pServer, brPath)),
        &value);
  }

  template <typename T>
  static void bindEnumValueByPath(
      UA_Server *pServer,
      const open62541Cpp::UA_BrowsePath &brPath,
      NodesMaster &nodesMaster,
      T &value)
  {
    bindValueByPath(pServer, brPath, nodesMaster, *reinterpret_cast<std::int32_t *>(&value));
  }

  // capture structs
  template <typename T>
  static void bindStructuredValueByPath(UA_Server *pServer,
                                        const open62541Cpp::UA_BrowsePath &brPath,
                                        NodesMaster &nodesMaster,
                                        T &value)
  {
    auto nodeId = resolveBrowsePath(pServer, brPath);
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
      bindStructRefl(nodesMaster(nodeId), value, pDataType);
    }
    else
    {
      bindStruct(
          nodesMaster(nodeId),
          reinterpret_cast<void *>(&value),
          pDataType);
    }
  }
};

template <class T>
void bindValueByPath(UA_Server *pServer,
                     const open62541Cpp::UA_BrowsePath &brPath,
                     NodesMaster &nodesMaster,
                     T &value)
{
  if constexpr (std::is_enum<T>::value)
  {
    bindValueByPathInternal::bindEnumValueByPath(pServer, brPath, nodesMaster, value);
  }
  else if constexpr (
      std::is_class<T>::value &&
      !(
          std::is_same<std::string, T>::value ||
          std::is_same<open62541Cpp::DateTime_t, T>::value))
  {
    bindValueByPathInternal::bindStructuredValueByPath(pServer, brPath, nodesMaster, value);
  }
  else
  {
    bindValueByPathInternal::bindValueByPath(pServer, brPath, nodesMaster, value);
  }
}
