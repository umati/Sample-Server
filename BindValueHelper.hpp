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
#include "BindValue.hpp"
#include <Open62541Cpp/UA_BrowsePath.hpp>
#include <exception>

// Internal implementation for different cases
struct bindValueByPathInternal {

 private:
  template<class T>
  friend void bindValueByPath(UA_Server *pServer,
                              const open62541Cpp::UA_BrowsePath &brPath,
                              NodesMaster &nodesMaster,
                              T &value);
  template<typename T>
  static void bindValueByPath(UA_Server *pServer,
                              const open62541Cpp::UA_BrowsePath &brPath,
                              NodesMaster &nodesMaster,
                              T &value,
                              std::false_type /*is_enum*/,
                              std::false_type /*is_class*/
  ) {
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

    bindValue(nodesMaster(trResult.targets[0].targetId.nodeId), &value);
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
  }

};

template<class T>
void bindValueByPath(UA_Server *pServer,
                     const open62541Cpp::UA_BrowsePath &brPath,
                     NodesMaster &nodesMaster,
                     T &value) {
  bindValueByPathInternal::bindValueByPath(pServer, brPath, nodesMaster, value, std::is_enum<T>{}, std::is_class<T>{});
}
