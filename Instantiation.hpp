#pragma once

#include "BindableMember.hpp"
#include "NodesMaster.hpp"
#include "BindRefl.hpp"
#include <exception>
#include <refl.hpp>
#include <open62541/server.h>
#include <Open62541Cpp/UA_QualifiedName.hpp>
#include "Util.hpp"
#include <sstream>

open62541Cpp::UA_QualifiedName readBrowseName(UA_Server *pServer, open62541Cpp::UA_NodeId nodeId)
{
  open62541Cpp::UA_QualifiedName ret(0, "");
  UA_StatusCode status = UA_Server_readBrowseName(pServer, *nodeId.NodeId, ret.QualifiedName);
  if (status != UA_STATUSCODE_GOOD)
  {
    std::stringstream ss;
    ss << "Could not read BrowseName from NodeId, Error: " << UA_StatusCode_name(status);
    throw std::runtime_error(ss.str());
  }

  return ret;
}

UA_NodeClass readNodeClass(UA_Server *pServer, open62541Cpp::UA_NodeId nodeId)
{
  UA_NodeClass ret = UA_NODECLASS_UNSPECIFIED;
  UA_StatusCode status = UA_Server_readNodeClass(pServer, *nodeId.NodeId, &ret);
  if (status != UA_STATUSCODE_GOOD)
  {
    std::stringstream ss;
    ss << "Could not read NodeClass from NodeId, Error: " << UA_StatusCode_name(status);
    throw std::runtime_error(ss.str());
  }

  return ret;
}

open62541Cpp::UA_NodeId readTypeDefinition(UA_Server *pServer, open62541Cpp::UA_NodeId nodeId)
{
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
  if (browseResult.statusCode != UA_STATUSCODE_GOOD)
  {
    ss << "Resutl not good: " << UA_StatusCode_name(browseResult.statusCode) << std::endl;
  }

  if (browseResult.referencesSize > 1)
  {
    ss << "Too many references found, choosing a random one!" << std::endl;
  }
  else if (browseResult.referencesSize == 0)
  {
    std::cout << "No TypeDefinition References found.";
    auto nodeClass = readNodeClass(pServer, nodeId);
    if (nodeClass == UA_NODECLASS_VARIABLE)
    {
      ret = open62541Cpp::UA_NodeId((UA_UInt16)0, UA_NS0ID_BASEVARIABLETYPE);
    }
    else if (nodeClass == UA_NODECLASS_OBJECT)
    {
      ret = open62541Cpp::UA_NodeId((UA_UInt16)0, UA_NS0ID_BASEOBJECTTYPE);
    }
    else
    {
      ss << "Unexpected Node class";
    }
  }
  else
  {
    ret = open62541Cpp::UA_NodeId(browseResult.references[0].nodeId.nodeId);
  }

  UA_BrowseResult_deleteMembers(&browseResult);
  UA_BrowseDescription_deleteMembers(&brDesc);
  if (ss.str().length() != 0)
  {
    throw std::runtime_error(ss.str());
  }
  return ret;
}

open62541Cpp::UA_NodeId getReferenceTypeFromMemberNode(UA_Server *pServer, open62541Cpp::UA_NodeId nodeId, open62541Cpp::UA_NodeId parentNodeId)
{

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
  if (browseResult.statusCode != UA_STATUSCODE_GOOD)
  {
    ss << "Resutl not good: " << UA_StatusCode_name(browseResult.statusCode) << std::endl;
  }

  if (browseResult.referencesSize > 1)
  {
    std::cout << "Too many references found, choosing a random one!" << std::endl;
    /// \TODO distiguish correct parent by compating with type of parent node?
    ret = open62541Cpp::UA_NodeId(browseResult.references[0].referenceTypeId);
  }
  else if (browseResult.referencesSize == 0)
  {
    ss << "No References found.";
  }
  else
  {
    ret = open62541Cpp::UA_NodeId(browseResult.references[0].referenceTypeId);
  }

  UA_BrowseResult_deleteMembers(&browseResult);
  UA_BrowseDescription_deleteMembers(&brDesc);
  if (ss.str().length() != 0)
  {
    throw std::runtime_error(ss.str());
  }
  return ret;
}

template <template <typename> class BINDABLEMEMBER_T, typename T, typename = std::enable_if_t<is_base_of_template<BindableMember, BINDABLEMEMBER_T<T>>::value>>
void InstatiateOptional(BINDABLEMEMBER_T<T> &memberPar, UA_Server *pServer, NodesMaster &nodesMaster)
{
  // Help the IDE for auto completion
  BindableMember<T> &member = memberPar;
  UA_StatusCode status = -1;

  if (member.IsBind())
  {
    // Already bind to an instance, nothing to do
    return;
  }

  if (member.ParentNodeId.NodeId == nullptr || member.MemerInTypeNodeId.NodeId == nullptr)
  {
    throw std::runtime_error("Parent not bind.");
  }
  // Initialize nodeid, so memory is allocated and the resulting nodeid can be written into it.
  member.NodeId = open62541Cpp::UA_NodeId((UA_UInt16)0, 0);
  auto browseName = readBrowseName(pServer, member.MemerInTypeNodeId);
  auto referenceType = getReferenceTypeFromMemberNode(pServer, member.MemerInTypeNodeId, member.ParentNodeId);

  auto nodeClass = readNodeClass(pServer, member.MemerInTypeNodeId);
  switch (nodeClass)
  {
  case UA_NODECLASS_OBJECT:
  {
    auto typeDef = readTypeDefinition(pServer, member.MemerInTypeNodeId);
    if constexpr (hasAttributeIfReflectable<open62541Cpp::attribute::UaObjectType, T>())
    {
      auto objTypeAttr = refl::descriptor::get_attribute<open62541Cpp::attribute::UaObjectType>(refl::reflect<T>());
      typeDef = objTypeAttr.NodeId.UANodeId(pServer);
    }

    UA_ObjectAttributes objAttr = UA_ObjectAttributes_default;
    objAttr.displayName.text = browseName.QualifiedName->name;
    {
      status = UA_Server_addObjectNode(
          pServer,
          UA_NODEID_NULL,
          *member.ParentNodeId.NodeId,
          *referenceType.NodeId,
          *browseName.QualifiedName,
          *typeDef.NodeId,
          objAttr,
          nullptr,
          member.NodeId.NodeId);
    }
    UA_ObjectAttributes_clear(&objAttr);
  }
  break;
  case UA_NODECLASS_VARIABLE:
  {
    auto typeDef = readTypeDefinition(pServer, member.MemerInTypeNodeId);
    if constexpr (hasAttributeIfReflectable<open62541Cpp::attribute::UaVariableType, T>())
    {
      // Instantiate Variable Type
      auto varTypeAttr = refl::descriptor::get_attribute<open62541Cpp::attribute::UaVariableType>(refl::reflect<T>());
      typeDef = varTypeAttr.NodeId.UANodeId(pServer);
    }
      UA_VariableAttributes varAttr = UA_VariableAttributes_default;
      varAttr.displayName.text = browseName.QualifiedName->name;
      {
        status = UA_Server_addVariableNode(
            pServer,
            UA_NODEID_NULL,
            *member.ParentNodeId.NodeId,
            *referenceType.NodeId,
            *browseName.QualifiedName,
            *typeDef.NodeId,
            varAttr,
            nullptr,
            member.NodeId.NodeId);
      }
      UA_VariableAttributes_clear(&varAttr);
  }
  break;
  default:
  {
    throw std::runtime_error("Unexpected NodeClass.");
  }
  break;
  }

  if (status != UA_STATUSCODE_GOOD)
  {
    std::stringstream ss;
    ss << "Could create node, Error: " << UA_StatusCode_name(status);
    throw std::runtime_error(ss.str());
  }

  bindMemberRefl(memberPar, pServer, member.NodeId, nodesMaster);
}
