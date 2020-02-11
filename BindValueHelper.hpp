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
#include <exception>

#include "BindValue.hpp"
#include "BindStruct.hpp"

// Internal implementation for different cases
struct bindValueByPathInternal {

 private:
  template<class T>
  friend void bindValueByPath(UA_Server *pServer,
                              const open62541Cpp::UA_BrowsePath &brPath,
                              NodesMaster &nodesMaster,
                              T &value);


  static open62541Cpp::UA_NodeId resolveBrowsePath(UA_Server *pServer,
                                            const open62541Cpp::UA_BrowsePath &brPath)
  {
    auto trResult =
        UA_Server_translateBrowsePathToNodeIds(
            pServer,
            brPath.BrowsePath
        );

    if (trResult.statusCode != UA_STATUSCODE_GOOD) {
      std::cout << "Resutl not good: " <<
                UA_StatusCode_name(trResult
                                       .statusCode) <<
                std::endl;
      throw std::invalid_argument("Path not found.");
    }

    if (trResult.targetsSize != 1) {
      std::cout << "Unexpected number of results, expect 1, got: " << trResult.targetsSize <<
                std::endl;
      throw std::invalid_argument("Unexpected number of results.");
    }
    open62541Cpp::UA_NodeId ret(trResult.targets[0].targetId.nodeId);
    UA_BrowsePathResult_deleteMembers(&trResult);
    return ret;
  }

  // Primitive types including string
  template<typename T>
  static void bindValueByPath(UA_Server *pServer,
                              const open62541Cpp::UA_BrowsePath &brPath,
                              NodesMaster &nodesMaster,
                              T &value,
                              std::false_type /*is_enum*/,
                              std::false_type /*is_class*/
  ) {
    bindValue(
        nodesMaster(resolveBrowsePath(pServer, brPath)),
        &value
        );
  }

  template<typename T>
  static void bindValueByPath(UA_Server *pServer,
                              const open62541Cpp::UA_BrowsePath &brPath,
                              NodesMaster &nodesMaster,
                              T &value,
                              std::true_type /*is_enum*/,
                              std::false_type /*is_class*/
  ) {
    bindValueByPath(pServer, brPath, nodesMaster, *reinterpret_cast<std::int32_t *>(&value), std::false_type{}, std::false_type{});
  }

  // capture structs
  template<typename T>
  static void bindValueByPath(UA_Server *pServer,
                              const open62541Cpp::UA_BrowsePath &brPath,
                              NodesMaster &nodesMaster,
                              T &value,
                              std::false_type /*is_enum*/,
                              std::true_type /*is_class*/
  ) {
    //bindValueByPath(pServer, brPath, nodesMaster, reinterpret_cast<void *>(&value), std::false_type{}, std::false_type{});

    auto nodeId = resolveBrowsePath(pServer, brPath);
    UA_NodeId typeNodeId;
    UA_NodeId_init(&typeNodeId);
    auto statusCode = UA_Server_readDataType(pServer, *nodeId.NodeId, &typeNodeId);
    if(statusCode != UA_STATUSCODE_GOOD)
    {
      std::cout << "UA_Server_readDataType: Resutl not good: " <<
                UA_StatusCode_name(statusCode) <<
                std::endl;
      throw std::runtime_error("Type could be be fetched, no variable node?");
    }

    const UA_DataType* pDataType = UA_findDataType(&typeNodeId);
    if(pDataType == nullptr)
    {
      std::cout << "NodeId to Datatype failed." << std::endl;
      throw std::runtime_error("NodeId to Datatype failed.");
    }

    bindStruct(
        nodesMaster(nodeId),
        reinterpret_cast<void *>(&value),
        pDataType
    );
  }

};

template<class T>
void bindValueByPath(UA_Server *pServer,
                     const open62541Cpp::UA_BrowsePath &brPath,
                     NodesMaster &nodesMaster,
                     T &value) {
  bindValueByPathInternal::bindValueByPath(pServer, brPath, nodesMaster, value, std::is_enum<T>{}, std::integral_constant<bool, std::is_class<T>::value && !std::is_same<T, std::string>::value>{} );
}
