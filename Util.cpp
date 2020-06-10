
#include "Util.hpp"
#include <Open62541Cpp/UA_String.hpp>
#include <iostream>

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
