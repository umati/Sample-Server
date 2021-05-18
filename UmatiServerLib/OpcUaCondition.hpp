/**
 * @file OpcUaCondition.hpp
 * @author Christian von Arnim
 * @date 2020-06-08
 *
 * @copyright Copyright (c) 2020
 *
 */

#pragma once
#include <open62541/server.h>

#include <Open62541Cpp/UA_NodeId.hpp>
#include <Open62541Cpp/UA_QualifiedName.hpp>
#include <iostream>
#include <refl.hpp>

#include "BindRefl.hpp"
#include "NodesMaster.hpp"

///\todo limit to subtypes of Condition_t by std::enable_if_t<>
// template <typename T, typename = std::enable_if_t<is_base_of_template<Condition_t, T>::value>>
template <typename T>
class OpcUaCondition {
 public:
  T Data;
  open62541Cpp::UA_NodeId ConditionNodeId;
  open62541Cpp::UA_NodeId Source;
  OpcUaCondition(UA_Server *pServer, open62541Cpp::UA_NodeId source) : pServer(pServer), nodesMaster(pServer), Source(source) { addConditionToAddressSpace(); }

  void Trigger() { UA_Server_triggerConditionEvent(pServer, *ConditionNodeId.NodeId, *Source.NodeId, nullptr); }

  ~OpcUaCondition() {
    if (UA_StatusCode retValConditionDelete = UA_Server_deleteCondition(pServer, *ConditionNodeId.NodeId, *Source.NodeId);
        retValConditionDelete != UA_STATUSCODE_GOOD) {
      std::cerr << "Could not delete conditionNode node: " << UA_StatusCode_name(retValConditionDelete) << std::endl;
    }
  }

 protected:
  UA_Server *pServer;
  NodesMaster nodesMaster;

  void addConditionToAddressSpace() {
    // Get Event Type from reflection Information
    static_assert(
      refl::descriptor::has_attribute<UmatiServerLib::attribute::UaObjectType>(refl::reflect<T>()), "Event must have an UaObjectType-attribute with node ID.");
    auto objTypeAttr = refl::descriptor::get_attribute<UmatiServerLib::attribute::UaObjectType>(refl::reflect<T>());
    auto conditionTypeNodeId = objTypeAttr.NodeId.UANodeId(pServer);
    auto name = refl::reflect<T>().name;
    UA_StatusCode retvalCreateCondition = UA_Server_createCondition(
      pServer,
      UA_NODEID_NUMERIC(Source.NodeId->namespaceIndex, 0),
      *conditionTypeNodeId.NodeId,
      *open62541Cpp::UA_QualifiedName(conditionTypeNodeId.NodeId->namespaceIndex, static_cast<std::string>(name)).QualifiedName,
      *Source.NodeId,
      UA_NODEID_NULL,
      ConditionNodeId.NodeId);

    if (retvalCreateCondition != UA_STATUSCODE_GOOD) {
      std::cerr << "Could not create condition: " << UA_StatusCode_name(retvalCreateCondition) << std::endl;
      throw std::runtime_error("Could not create condition.");
    }

    UmatiServerLib::Bind::MembersRefl(Data, pServer, ConditionNodeId, nodesMaster);
  }
};
