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

open62541Cpp::UA_QualifiedName readBrowseName(UA_Server *pServer, open62541Cpp::UA_NodeId nodeId);
UA_NodeClass readNodeClass(UA_Server *pServer, open62541Cpp::UA_NodeId nodeId);
open62541Cpp::UA_NodeId readTypeDefinition(UA_Server *pServer, open62541Cpp::UA_NodeId nodeId);
open62541Cpp::UA_NodeId getReferenceTypeFromMemberNode(UA_Server *pServer, open62541Cpp::UA_NodeId nodeId, open62541Cpp::UA_NodeId parentNodeId);

template <template <typename> class BINDABLEMEMBER_T, typename T, typename = std::enable_if_t<is_base_of_template<BindableMember, BINDABLEMEMBER_T<T>>::value>>
void InstantiateOptional(BINDABLEMEMBER_T<T> &memberPar, UA_Server *pServer, NodesMaster &nodesMaster)
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
    UA_String_copy(&browseName.QualifiedName->name, &objAttr.displayName.text);
    {
      status = UA_Server_addObjectNode(
          pServer,
          UA_NODEID_NUMERIC(member.ParentNodeId.NodeId->namespaceIndex, 0),
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
      UA_String_copy(&browseName.QualifiedName->name, &varAttr.displayName.text);
      {
        status = UA_Server_addVariableNode(
            pServer,
            UA_NODEID_NUMERIC(member.ParentNodeId.NodeId->namespaceIndex, 0),
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
