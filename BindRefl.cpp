#include "BindRefl.hpp"

open62541Cpp::UA_NodeId resolveBrowsePath(
    UA_Server *pServer,
    const open62541Cpp::UA_BrowsePath &brPath)
{
  // No Elements in browse path.
  if (brPath.BrowsePath->relativePath.elementsSize == 0)
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