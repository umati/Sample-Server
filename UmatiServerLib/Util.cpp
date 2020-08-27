
#include "Util.hpp"
#include <Open62541Cpp/UA_String.hpp>
#include <iostream>
#include <open62541/server.h>

UA_Int16 nsFromUri(UA_Server *pServer, std::string uri)
{
  std::size_t nsIndex = 0;
  open62541Cpp::UA_String uastr(uri);
  auto ret = UA_Server_getNamespaceByName(pServer, *uastr.String, &nsIndex);
  if (ret != UA_STATUSCODE_GOOD)
  {
    std::cout << "Could not resolve namespace URI: " << uri << std::endl;
    throw std::runtime_error("NsUri resolution failed.");
  }

  return nsIndex;
}

bool isSubtypeOf(UA_Server *pServer, const open62541Cpp::UA_NodeId &typeNodeId, const open62541Cpp::UA_NodeId &baseNodeId)
{
  if (UA_NodeId_equal(typeNodeId.NodeId, baseNodeId.NodeId))
  {
    return true;
  }
  open62541Cpp::UA_NodeId cuNodeId(typeNodeId);
  UA_BrowseDescription brd;
  brd.browseDirection = UA_BROWSEDIRECTION_INVERSE;
  brd.includeSubtypes = UA_TRUE;
  brd.nodeClassMask = UA_NODECLASS_OBJECTTYPE | UA_NODECLASS_VARIABLETYPE;
  brd.referenceTypeId = UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE);
  brd.nodeId = *typeNodeId.NodeId;
  size_t resLen;
  UA_ExpandedNodeId *pResult;
  auto statuscode = UA_Server_browseRecursive(pServer, &brd, &resLen, &pResult);

  if (statuscode != UA_STATUSCODE_GOOD)
  {
    std::cout << "Error while browsing. " << UA_StatusCode_name(statuscode) << std::endl;
    return false;
  }

  bool found = false;
  for (decltype(resLen) iResult = 0; iResult < resLen; ++iResult)
  {
    if (UA_NodeId_equal(&pResult[iResult].nodeId, baseNodeId.NodeId))
    {
      found = true;
      break;
    }
  }

  UA_Array_delete(pResult, resLen, &UA_TYPES[UA_TYPES_EXPANDEDNODEID]);

  return found;
}

open62541Cpp::UA_NodeId browseForParent(open62541Cpp::UA_NodeId node, open62541Cpp::UA_NodeId referenceType, UA_Server *pServer)
{
  open62541Cpp::UA_NodeId ret;
  UA_BrowseDescription brDesc;
  UA_BrowseDescription_init(&brDesc);
  UA_NodeId_copy(node.NodeId, &brDesc.nodeId);
  UA_NodeId_copy(referenceType.NodeId, &brDesc.referenceTypeId);

  brDesc.nodeClassMask = UA_NODECLASS_OBJECTTYPE;

  brDesc.browseDirection = UA_BrowseDirection::UA_BROWSEDIRECTION_INVERSE;
  brDesc.includeSubtypes = UA_TRUE;
  brDesc.resultMask = UA_BROWSERESULTMASK_NONE;

  auto browseResult = UA_Server_browse(pServer, UA_UINT32_MAX, &brDesc);
  if (browseResult.statusCode != UA_STATUSCODE_GOOD)
  {
    std::cout << "Resutl not good for browsing parent node. " << UA_StatusCode_name(browseResult.statusCode) << std::endl;
  }

  if(browseResult.referencesSize == 0)
  {
    std::cout << "No parent node found." << std::endl;
  }

  if(browseResult.referencesSize > 1)
  {
    std::cout << "Multiple parent nodes found, choosing a random one." << std::endl;
    ///\todo Exception?
  }

  if(browseResult.referencesSize > 0)
  {
    UA_NodeId_copy(&browseResult.references[0].nodeId.nodeId, ret.NodeId);
  }

  UA_BrowseResult_deleteMembers(&browseResult);
  UA_BrowseDescription_deleteMembers(&brDesc);
  return ret;
}
