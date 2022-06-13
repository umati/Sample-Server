/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2020-2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 * Copyright 2020 (c) Tonja Heinemann, ISW University of Stuttgart (for umati and VDW e.V.)
 * Copyright 2021 (c) Christoph Ruckstetter, Michael Weinig AG
 */

#include "Instantiation.hpp"

open62541Cpp::UA_QualifiedName readBrowseName(UA_Server *pServer, open62541Cpp::UA_NodeId nodeId) {
  open62541Cpp::UA_QualifiedName ret(0, "");
  UA_StatusCode status = UA_Server_readBrowseName(pServer, *nodeId.NodeId, ret.QualifiedName);
  if (status != UA_STATUSCODE_GOOD) {
    std::stringstream ss;
    ss << "Could not read BrowseName from NodeId (" << static_cast<std::string>(nodeId) << "), Error: " << UA_StatusCode_name(status);
    throw std::runtime_error(ss.str());
  }

  return ret;
}

UA_NodeClass readNodeClass(UA_Server *pServer, open62541Cpp::UA_NodeId nodeId) {
  UA_NodeClass ret = UA_NODECLASS_UNSPECIFIED;
  UA_StatusCode status = UA_Server_readNodeClass(pServer, *nodeId.NodeId, &ret);
  if (status != UA_STATUSCODE_GOOD) {
    std::stringstream ss;
    ss << "Could not read NodeClass from NodeId (" << static_cast<std::string>(nodeId) << "), Error: " << UA_StatusCode_name(status);
    throw std::runtime_error(ss.str());
  }

  return ret;
}

open62541Cpp::UA_NodeId readDataType(UA_Server *pServer, open62541Cpp::UA_NodeId nodeId) {
  open62541Cpp::UA_NodeId ret((std::uint16_t)0, 0);
  UA_StatusCode status = UA_Server_readDataType(pServer, *nodeId.NodeId, ret.NodeId);
  if (status != UA_STATUSCODE_GOOD) {
    std::stringstream ss;
    ss << "Could not read DataType from NodeId (" << static_cast<std::string>(nodeId) << "), Error: " << UA_StatusCode_name(status);
    throw std::runtime_error(ss.str());
  }

  return ret;
}

UA_Int32 readValueRank(UA_Server *pServer, open62541Cpp::UA_NodeId nodeId) {
  UA_Int32 ret;
  UA_StatusCode status = UA_Server_readValueRank(pServer, *nodeId.NodeId, &ret);
  if (status != UA_STATUSCODE_GOOD) {
    std::stringstream ss;
    ss << "Could not read DataType from NodeId (" << static_cast<std::string>(nodeId) << "), Error: " << UA_StatusCode_name(status);
    throw std::runtime_error(ss.str());
  }

  return ret;
}

UA_Variant *readArrayDimensions(UA_Server *pServer, open62541Cpp::UA_NodeId nodeId) {
  UA_Variant *varArrayDims = UA_Variant_new();
  auto status = UA_Server_readArrayDimensions(pServer, *nodeId.NodeId, varArrayDims);
  if (status != UA_STATUSCODE_GOOD) {
    std::stringstream ss;
    ss << "Could not read ArrayDimensions attribute from NodeId (" << static_cast<std::string>(nodeId) << "), Error: " << UA_StatusCode_name(status);
    throw std::runtime_error(ss.str());
  }

  return varArrayDims;
}

open62541Cpp::UA_NodeId readTypeDefinition(UA_Server *pServer, open62541Cpp::UA_NodeId nodeId) {
  open62541Cpp::UA_NodeId ret;

  UA_BrowseDescription brDesc;
  UA_BrowseDescription_init(&brDesc);
  UA_NodeId_copy(nodeId.NodeId, &brDesc.nodeId);
  brDesc.referenceTypeId = UA_NODEID_NUMERIC(0, UA_NS0ID_HASTYPEDEFINITION);

  brDesc.nodeClassMask = UA_NODECLASS_OBJECTTYPE | UA_NODECLASS_VARIABLETYPE;
  brDesc.browseDirection = UA_BrowseDirection::UA_BROWSEDIRECTION_FORWARD;
  brDesc.includeSubtypes = UA_TRUE;
  brDesc.resultMask = UA_BROWSERESULTMASK_NONE;

  auto browseResult = UA_Server_browse(pServer, UA_UINT32_MAX, &brDesc);
  std::stringstream ss;
  if (browseResult.statusCode != UA_STATUSCODE_GOOD) {
    ss << "Result not good: " << UA_StatusCode_name(browseResult.statusCode) << std::endl;
  }

  if (browseResult.referencesSize > 1) {
    ss << "Too many references found, choosing a random one!" << std::endl;
  } else if (browseResult.referencesSize == 0) {
    std::cout << "No TypeDefinition References found.";
    auto nodeClass = readNodeClass(pServer, nodeId);
    if (nodeClass == UA_NODECLASS_VARIABLE) {
      ret = open62541Cpp::UA_NodeId((UA_UInt16)0, UA_NS0ID_BASEVARIABLETYPE);
    } else if (nodeClass == UA_NODECLASS_OBJECT) {
      ret = open62541Cpp::UA_NodeId((UA_UInt16)0, UA_NS0ID_BASEOBJECTTYPE);
    } else {
      ss << "Unexpected Node class";
    }
  } else {
    ret = open62541Cpp::UA_NodeId(browseResult.references[0].nodeId.nodeId);
  }

  UA_BrowseResult_clear(&browseResult);
  UA_BrowseDescription_clear(&brDesc);
  if (ss.str().length() != 0) {
    throw std::runtime_error(ss.str());
  }
  return ret;
}

open62541Cpp::UA_NodeId getReferenceTypeFromMemberNode(UA_Server *pServer, open62541Cpp::UA_NodeId nodeId, open62541Cpp::UA_NodeId parentNodeId) {
  open62541Cpp::UA_NodeId ret;

  UA_BrowseDescription brDesc;
  UA_BrowseDescription_init(&brDesc);
  UA_NodeId_copy(nodeId.NodeId, &brDesc.nodeId);
  brDesc.referenceTypeId = UA_NODEID_NUMERIC(0, UA_NS0ID_HIERARCHICALREFERENCES);

  brDesc.nodeClassMask = UA_NODECLASS_OBJECT | UA_NODECLASS_VARIABLE | UA_NODECLASS_OBJECTTYPE | UA_NODECLASS_VARIABLETYPE;
  brDesc.browseDirection = UA_BrowseDirection::UA_BROWSEDIRECTION_INVERSE;
  brDesc.includeSubtypes = UA_TRUE;
  brDesc.resultMask = UA_BROWSERESULTMASK_REFERENCETYPEID;

  auto browseResult = UA_Server_browse(pServer, UA_UINT32_MAX, &brDesc);

  std::stringstream ss;
  if (browseResult.statusCode != UA_STATUSCODE_GOOD) {
    ss << "Result not good: " << UA_StatusCode_name(browseResult.statusCode) << std::endl;
  }

  if (browseResult.referencesSize > 1) {
    std::cout << "Too many references found, choosing a random one!" << std::endl;
    /// \TODO distiguish correct parent by compating with type of parent node?
    ret = open62541Cpp::UA_NodeId(browseResult.references[0].referenceTypeId);
  } else if (browseResult.referencesSize == 0) {
    ss << "No References found.";
  } else {
    ret = open62541Cpp::UA_NodeId(browseResult.references[0].referenceTypeId);
  }

  UA_BrowseResult_clear(&browseResult);
  UA_BrowseDescription_clear(&brDesc);
  if (ss.str().length() != 0) {
    throw std::runtime_error(ss.str());
  }
  return ret;
}

void instantiateInterfaces(UA_Server *pServer, open62541Cpp::UA_NodeId member, open62541Cpp::UA_NodeId memberInType) {
  UA_BrowseDescription brDesc;
  UA_BrowseDescription_init(&brDesc);
  UA_NodeId_copy(memberInType.NodeId, &brDesc.nodeId);
  brDesc.referenceTypeId = UA_NODEID_NUMERIC(0, UA_NS0ID_HASINTERFACE);

  brDesc.nodeClassMask = UA_NODECLASS_OBJECTTYPE | UA_NODECLASS_VARIABLETYPE;
  brDesc.browseDirection = UA_BrowseDirection::UA_BROWSEDIRECTION_FORWARD;
  brDesc.includeSubtypes = UA_TRUE;
  brDesc.resultMask = UA_BROWSERESULTMASK_NONE;

  auto browseResult = UA_Server_browse(pServer, UA_UINT32_MAX, &brDesc);

  std::stringstream ss;
  if (browseResult.statusCode != UA_STATUSCODE_GOOD) {
    ss << "Result not good: " << UA_StatusCode_name(browseResult.statusCode) << std::endl;
  }

  for (std::size_t i = 0; i < browseResult.referencesSize; ++i) {
    UA_StatusCode status =
      UA_Server_addReference(pServer, *member.NodeId, UA_NODEID_NUMERIC(0, UA_NS0ID_HASINTERFACE), browseResult.references[0].nodeId, UA_TRUE);
    if (status != UA_STATUSCODE_GOOD) {
      ss << "Failed to add HasInterface references. Code: " << UA_StatusCode_name(browseResult.statusCode) << std::endl;
      break;
    }
  }

  UA_BrowseResult_clear(&browseResult);
  UA_BrowseDescription_clear(&brDesc);
  if (ss.str().length() != 0) {
    throw std::runtime_error(ss.str());
  }
}
