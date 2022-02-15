/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2020-2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 * Copyright 2021 (c) Christoph Ruckstetter, Michael Weinig AG
 * Copyright 2022 (c) Sebastian Friedl, ISW University of Stuttgart (for VDMA e.V.)
 */

#pragma once

#include <open62541/server.h>

#include <Open62541Cpp/UA_QualifiedName.hpp>
#include <exception>
#include <refl.hpp>
#include <sstream>

#include "BindRefl.hpp"
#include "BindableMember.hpp"
#include "NodesMaster.hpp"
#include "Util.hpp"

open62541Cpp::UA_QualifiedName readBrowseName(UA_Server *pServer, open62541Cpp::UA_NodeId nodeId);
UA_NodeClass readNodeClass(UA_Server *pServer, open62541Cpp::UA_NodeId nodeId);
open62541Cpp::UA_NodeId readDataType(UA_Server *pServer, open62541Cpp::UA_NodeId nodeId);
open62541Cpp::UA_NodeId readTypeDefinition(UA_Server *pServer, open62541Cpp::UA_NodeId nodeId);
UA_Int32 readValueRank(UA_Server *pServer, open62541Cpp::UA_NodeId nodeId);
open62541Cpp::UA_NodeId getReferenceTypeFromMemberNode(UA_Server *pServer, open62541Cpp::UA_NodeId nodeId, open62541Cpp::UA_NodeId parentNodeId);

/// Look for interfaces and instantiate them
void instantiateInterfaces(UA_Server *pServer, open62541Cpp::UA_NodeId member, open62541Cpp::UA_NodeId memberInType);

template <typename T>
UA_StatusCode InstantiateVariable(
  UA_Server pServer,
  const open62541Cpp::UA_NodeId &memberInTypeNodeId,
  const open62541Cpp::UA_QualifiedName &browseName,
  const open62541Cpp::UA_QualifiedName &referenceType,
  const open62541Cpp::UA_NodeId &parentNodeId,
  open62541Cpp::UA_NodeId &outNodeId);

/**
 * @brief Instantiate an optional member, which parent is already bind.
 *
 * Checks the node in the type definition and create a variabe/object
 *
 * @param memberPar The member that should be instantiated
 * @param pServer Pointer to UA_Server
 * @param nodesMaster A nodesmaster to manage the binding
 */
template <template <typename> class BINDABLEMEMBER_T, typename T, typename = std::enable_if_t<is_base_of_template<BindableMember, BINDABLEMEMBER_T<T>>::value>>
void InstantiateOptional(BINDABLEMEMBER_T<T> &memberPar, UA_Server *pServer, NodesMaster &nodesMaster) {
  // Help the IDE for auto completion
  BindableMember<T> &member = memberPar;
  UA_StatusCode status = -1;

  if (member.IsBind()) {
    // Already bind to an instance, nothing to do
    return;
  }

  if (member.ParentNodeId.NodeId == nullptr) {
    throw std::runtime_error("Parent not bind. ParentNodeId is null");
  }
  if (UA_NodeId_isNull(member.MemberInTypeNodeId.NodeId)) {
    throw std::runtime_error("Parent not bind. MemberInTypeNodeId is null");
  }

  // Initialize nodeid, so memory is allocated and the resulting nodeid can be written into it.
  member.NodeId = open62541Cpp::UA_NodeId((UA_UInt16)0, 0);
  auto browseName = readBrowseName(pServer, member.MemberInTypeNodeId);
  auto referenceType = getReferenceTypeFromMemberNode(pServer, member.MemberInTypeNodeId, member.ParentNodeId);

  auto nodeClass = readNodeClass(pServer, member.MemberInTypeNodeId);
  switch (nodeClass) {
    case UA_NODECLASS_OBJECT: {
      auto typeDef = readTypeDefinition(pServer, member.MemberInTypeNodeId);
      // Check for Object type, so subsequent classes are possible.
      if constexpr (hasAttributeIfReflectable<UmatiServerLib::attribute::UaObjectType, T>()) {
        auto objTypeAttr = refl::descriptor::get_attribute<UmatiServerLib::attribute::UaObjectType>(refl::reflect<T>());
        open62541Cpp::UA_NodeId typeDefFromObjTypeAttr = objTypeAttr.NodeId.UANodeId(pServer);
        UA_Boolean isAbstract;
        status = UA_Server_readIsAbstract(pServer, *typeDefFromObjTypeAttr.NodeId, &isAbstract);
        if (status != UA_STATUSCODE_GOOD) {
          std::stringstream ss;
          ss << "Could not read isAbstract from node (" << static_cast<std::string>(typeDefFromObjTypeAttr) << "), Error: " << UA_StatusCode_name(status);
          throw std::runtime_error(ss.str());
        }
        if (!isAbstract) {
          // Might be an interface type
          typeDef = typeDefFromObjTypeAttr;
        }
      }

      UA_ObjectAttributes objAttr = UA_ObjectAttributes_default;
      UA_String_copy(&browseName.QualifiedName->name, &objAttr.displayName.text);

      status = UA_Server_addNode_begin(
        pServer,
        UA_NodeClass::UA_NODECLASS_OBJECT,
        UA_NODEID_NUMERIC(member.ParentNodeId.NodeId->namespaceIndex, 0),
        *member.ParentNodeId.NodeId,
        *referenceType.NodeId,
        *browseName.QualifiedName,
        *typeDef.NodeId,
        &objAttr,
        &UA_TYPES[UA_TYPES_OBJECTATTRIBUTES],
        nullptr,
        member.NodeId.NodeId);

      if (status == UA_STATUSCODE_GOOD) {
        instantiateInterfaces(pServer, member.NodeId, member.MemberInTypeNodeId);
      }

      status = UA_Server_addNode_finish(pServer, *member.NodeId.NodeId);

      UA_ObjectAttributes_clear(&objAttr);
    } break;
    case UA_NODECLASS_VARIABLE: {
      status = InstantiateVariable<T>(pServer, member.MemberInTypeNodeId, browseName, referenceType, member.ParentNodeId, member.NodeId);
    } break;
    default: {
      throw std::runtime_error("Unexpected NodeClass.");
    } break;
  }

  if (status != UA_STATUSCODE_GOOD) {
    std::stringstream ss;
    ss << "Could not create optional node, Error: " << UA_StatusCode_name(status);
    throw std::runtime_error(ss.str());
  }

  UmatiServerLib::Bind::MemberRefl(memberPar, pServer, member.NodeId, nodesMaster);
}

template <typename T>
UA_StatusCode InstantiateVariable(
  UA_Server *pServer,
  const open62541Cpp::UA_NodeId &memberInTypeNodeId,
  const open62541Cpp::UA_QualifiedName &browseName,
  const open62541Cpp::UA_NodeId &referenceType,
  const open62541Cpp::UA_NodeId &parentNodeId,
  open62541Cpp::UA_NodeId &outNodeId) {
  UA_StatusCode status;
  auto typeDef = readTypeDefinition(pServer, memberInTypeNodeId);
  if constexpr (hasAttributeIfReflectable<UmatiServerLib::attribute::UaVariableType, T>()) {
    // Instantiate Variable Type
    auto varTypeAttr = refl::descriptor::get_attribute<UmatiServerLib::attribute::UaVariableType>(refl::reflect<T>());
    typeDef = varTypeAttr.NodeId.UANodeId(pServer);
  }
  UA_VariableAttributes varAttr = UA_VariableAttributes_default;
  varAttr.valueRank = readValueRank(pServer, memberInTypeNodeId);

  // Add Array Dimensions for Arrays
  if (varAttr.valueRank == 1) {
    varAttr.arrayDimensionsSize = 1;
    UA_UInt32 arrayDims[1] = {1};
    varAttr.arrayDimensions = arrayDims;
    varAttr.value.arrayDimensionsSize = 1;
    varAttr.value.arrayDimensions = arrayDims;
  }

  auto dataType = readDataType(pServer, memberInTypeNodeId);
  UA_String_copy(&browseName.QualifiedName->name, &varAttr.displayName.text);
  UA_NodeId_copy(dataType.NodeId, &varAttr.dataType);
  {
    status = UA_Server_addVariableNode(
      pServer,
      UA_NODEID_NUMERIC(parentNodeId.NodeId->namespaceIndex, 0),
      *parentNodeId.NodeId,
      *referenceType.NodeId,
      *browseName.QualifiedName,
      *typeDef.NodeId,
      varAttr,
      nullptr,
      outNodeId.NodeId);
  }
  UA_VariableAttributes_clear(&varAttr);
  return status;
}
