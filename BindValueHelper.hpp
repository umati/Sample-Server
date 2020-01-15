///
/// \file bindValueHelper.hpp
/// \author Christian von Arnim
/// \date 15.01.2020
///

#pragma once

#include <open62541/types.h>
#include <open62541/server.h>
#include <open62541/types_generated.h>
#include "RelativPathElement.hpp"
#include <list>
#include "BindValue.hpp"
#include "BrowsePath.hpp"
#include <exception>

template<typename T>
void bindValueByPath(UA_Server *pServer, const BrowsePath &brPath, NodesMaster &nodesMaster, T &value) {
  auto trResult =
      UA_Server_translateBrowsePathToNodeIds(
          pServer,
          brPath.BPath
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
