/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2020-2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 * Copyright 2020 (c) Tonja Heinemann, ISW University of Stuttgart (for umati and VDW e.V.)
 * Copyright 2021 (c) Christoph Ruckstetter, Michael Weinig AG
 * Copyright 2022 (c) Alen Galinec
 */

#include "Instantiation.hpp"

namespace {

void failReadNodeAttribute(open62541Cpp::UA_NodeId nodeId, UA_StatusCode status, std::string const &attributeName) {
  std::stringstream ss;
  ss << "Could not read " << attributeName << " from NodeId (" << static_cast<std::string>(nodeId) << "), Error: " << UA_StatusCode_name(status);
  throw std::runtime_error(ss.str());
}

UA_BrowseDescription createBrowseFilter(
  open62541Cpp::UA_NodeId const &nodeId,
  UA_NodeId const &referenceTypeId,
  UA_UInt32 classMask,
  UA_BrowseDirection const &browseDirection,
  UA_UInt32 resultMask) {
  UA_BrowseDescription browseDescription;
  UA_BrowseDescription_init(&browseDescription);
  UA_NodeId_copy(nodeId.NodeId, &browseDescription.nodeId);

  browseDescription.referenceTypeId = referenceTypeId;
  browseDescription.nodeClassMask = classMask;
  browseDescription.browseDirection = browseDirection;
  browseDescription.includeSubtypes = UA_TRUE;
  browseDescription.resultMask = resultMask;

  return browseDescription;
}

UA_ReferenceDescription readReference(UA_Server *pServer, UA_BrowseDescription browseDescription) {
  UA_ReferenceDescription result;
  UA_ReferenceDescription_init(&result);

  auto browseResult = UA_Server_browse(pServer, UA_UINT32_MAX, &browseDescription);

  if (browseResult.statusCode == UA_STATUSCODE_GOOD && browseResult.referencesSize > 0) {
    UA_ReferenceDescription_copy(&browseResult.references[0], &result);
  }

  UA_BrowseResult_clear(&browseResult);
  UA_BrowseDescription_clear(&browseDescription);

  return result;
}

}  // namespace

open62541Cpp::UA_QualifiedName readBrowseName(UA_Server *pServer, open62541Cpp::UA_NodeId nodeId) {
  open62541Cpp::UA_QualifiedName ret(0, "");
  UA_StatusCode status = UA_Server_readBrowseName(pServer, *nodeId.NodeId, ret.QualifiedName);
  if (status != UA_STATUSCODE_GOOD) failReadNodeAttribute(nodeId, status, "BrowseName");

  return ret;
}

UA_NodeClass readNodeClass(UA_Server *pServer, open62541Cpp::UA_NodeId nodeId) {
  UA_NodeClass ret = UA_NODECLASS_UNSPECIFIED;
  UA_StatusCode status = UA_Server_readNodeClass(pServer, *nodeId.NodeId, &ret);
  if (status != UA_STATUSCODE_GOOD) failReadNodeAttribute(nodeId, status, "NodeClass");

  return ret;
}

open62541Cpp::UA_NodeId readDataType(UA_Server *pServer, open62541Cpp::UA_NodeId nodeId) {
  open62541Cpp::UA_NodeId ret((std::uint16_t)0, 0);
  UA_StatusCode status = UA_Server_readDataType(pServer, *nodeId.NodeId, ret.NodeId);
  if (status != UA_STATUSCODE_GOOD) failReadNodeAttribute(nodeId, status, "DataType");

  return ret;
}

UA_Int32 readValueRank(UA_Server *pServer, open62541Cpp::UA_NodeId nodeId) {
  UA_Int32 ret;
  UA_StatusCode status = UA_Server_readValueRank(pServer, *nodeId.NodeId, &ret);
  if (status != UA_STATUSCODE_GOOD) failReadNodeAttribute(nodeId, status, "ValueRank");

  return ret;
}

UA_Variant *readArrayDimensions(UA_Server *pServer, open62541Cpp::UA_NodeId nodeId) {
  UA_Variant *varArrayDims = UA_Variant_new();
  auto status = UA_Server_readArrayDimensions(pServer, *nodeId.NodeId, varArrayDims);
  if (status != UA_STATUSCODE_GOOD) failReadNodeAttribute(nodeId, status, "ArrayDimensions");

  return varArrayDims;
}

open62541Cpp::UA_NodeId readTypeDefinition(UA_Server *pServer, open62541Cpp::UA_NodeId nodeId) {
  open62541Cpp::UA_NodeId ret;

  auto brDesc = createBrowseFilter(
    nodeId,
    UA_NODEID_NUMERIC(0, UA_NS0ID_HASTYPEDEFINITION),
    UA_NODECLASS_OBJECTTYPE | UA_NODECLASS_VARIABLETYPE,
    UA_BrowseDirection::UA_BROWSEDIRECTION_FORWARD,
    UA_BROWSERESULTMASK_NONE);

  auto reference = readReference(pServer, brDesc);

  std::stringstream ss;

  if (UA_NodeId_equal(&reference.nodeId.nodeId, &UA_NODEID_NULL)) {
    auto nodeClass = readNodeClass(pServer, nodeId);
    if (nodeClass == UA_NODECLASS_VARIABLE) {
      ret = open62541Cpp::UA_NodeId((UA_UInt16)0, UA_NS0ID_BASEVARIABLETYPE);
    } else if (nodeClass == UA_NODECLASS_OBJECT) {
      ret = open62541Cpp::UA_NodeId((UA_UInt16)0, UA_NS0ID_BASEOBJECTTYPE);
    } else {
      ss << "TypeDefinition for NodeId " << static_cast<std::string>(nodeId) << " not found. Unexpected Node class." << std::endl;
    }
  } else {
    ret = open62541Cpp::UA_NodeId(reference.nodeId.nodeId);
  }

  UA_BrowseDescription_clear(&brDesc);
  UA_ReferenceDescription_clear(&reference);

  if (!ss.str().empty()) {
    throw std::runtime_error(ss.str());
  }

  return ret;
}

open62541Cpp::UA_NodeId getReferenceTypeFromMemberNode(UA_Server *pServer, open62541Cpp::UA_NodeId nodeId, open62541Cpp::UA_NodeId parentNodeId) {
  open62541Cpp::UA_NodeId ret;

  auto brDesc = createBrowseFilter(
    nodeId,
    UA_NODEID_NUMERIC(0, UA_NS0ID_HIERARCHICALREFERENCES),
    UA_NODECLASS_OBJECTTYPE | UA_NODECLASS_VARIABLETYPE | UA_NODECLASS_OBJECT | UA_NODECLASS_VARIABLE,
    UA_BrowseDirection::UA_BROWSEDIRECTION_INVERSE,
    UA_BROWSERESULTMASK_REFERENCETYPEID);

  auto reference = readReference(pServer, brDesc);

  std::stringstream ss;
  if (UA_NodeId_equal(&reference.nodeId.nodeId, &UA_NODEID_NULL)) {
    ss << "ReferenceType for NodeId " << static_cast<std::string>(nodeId) << " not found." << std::endl;
  } else {
    ret = open62541Cpp::UA_NodeId(reference.referenceTypeId);
  }

  UA_BrowseDescription_clear(&brDesc);
  UA_ReferenceDescription_clear(&reference);

  if (!ss.str().empty()) {
    throw std::runtime_error(ss.str());
  }

  return ret;
}

void instantiateInterfaces(UA_Server *pServer, open62541Cpp::UA_NodeId member, open62541Cpp::UA_NodeId memberInType) {
  auto brDesc = createBrowseFilter(
    memberInType,
    UA_NODEID_NUMERIC(0, UA_NS0ID_HASINTERFACE),
    UA_NODECLASS_OBJECTTYPE | UA_NODECLASS_VARIABLETYPE,
    UA_BROWSEDIRECTION_FORWARD,
    UA_BROWSERESULTMASK_NONE);

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
  if (!ss.str().empty()) {
    throw std::runtime_error(ss.str());
  }
}
