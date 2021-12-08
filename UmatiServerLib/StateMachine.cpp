/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2020-2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 */

#include "StateMachine.hpp"

#include <Open62541Cpp/UA_QualifiedName.hpp>
#include <iostream>
#include <map>
#include <sstream>

#include "Util.hpp"

namespace UmatiServerLib {

StateMachine::StateMachine(open62541Cpp::UA_NodeId stateMachineType, UA_Server *pServer) : m_pServer(pServer), m_stateMachineType(stateMachineType) {
  readStatesAndTransitions();
}

void StateMachine::readStatesAndTransitions() {
  open62541Cpp::UA_NodeId stateMachineType = m_stateMachineType;
  auto finiteStateMachineNsId = UA_NODEID_NUMERIC(0, UA_NS0ID_FINITESTATEMACHINETYPE);

  while (!UA_NodeId_equal(stateMachineType.NodeId, &finiteStateMachineNsId)) {
    readStates(stateMachineType);
    readTransitions(stateMachineType);
    auto parent = browseForParent(stateMachineType, open62541Cpp::UA_NodeId(UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE)), m_pServer);
    if (UA_NodeId_isNull(parent.NodeId)) {
      break;
    }
    stateMachineType = parent;
  }
  mergeTransitions();
}

void StateMachine::readStates(open62541Cpp::UA_NodeId stateMachineType) {
  UA_BrowseDescription brDesc;
  UA_BrowseDescription_init(&brDesc);
  UA_NodeId_copy(stateMachineType.NodeId, &brDesc.nodeId);
  brDesc.referenceTypeId = UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT);

  open62541Cpp::UA_NodeId typeNodeId(UA_NODEID_NUMERIC(0, UA_NS0ID_STATETYPE));
  brDesc.nodeClassMask = UA_NODECLASS_OBJECT;

  brDesc.browseDirection = UA_BrowseDirection::UA_BROWSEDIRECTION_FORWARD;
  brDesc.includeSubtypes = UA_TRUE;
  brDesc.resultMask = UA_BROWSERESULTMASK_ALL;

  auto browseResult = UA_Server_browse(m_pServer, UA_UINT32_MAX, &brDesc);
  if (browseResult.statusCode != UA_STATUSCODE_GOOD) {
    std::cout << "Resutl not good: " << UA_StatusCode_name(browseResult.statusCode) << std::endl;
    std::cout << "Could not read States from statemachine." << std::endl;
  }

  for (std::size_t i = 0; i < browseResult.referencesSize; ++i) {
    if (isTypeOrSubtype(m_pServer, typeNodeId, open62541Cpp::UA_NodeId(browseResult.references[i].typeDefinition.nodeId))) {
      m_states.push_back(readStateValues(open62541Cpp::UA_NodeId(browseResult.references[i].nodeId.nodeId)));
    }
  }

  UA_BrowseResult_clear(&browseResult);
  UA_BrowseDescription_clear(&brDesc);
}

StateMachine::State_t StateMachine::readStateValues(open62541Cpp::UA_NodeId stateObj) {
  State_t ret{{}, UA_UINT32_MAX, {}};
  open62541Cpp::UA_QualifiedName StateNumber_qn(0, "StateNumber");
  UA_Variant stateNumber;
  UA_Variant_init(&stateNumber);
  auto status = UA_Server_readObjectProperty(m_pServer, *stateObj.NodeId, *StateNumber_qn.QualifiedName, &stateNumber);
  if (status != UA_STATUSCODE_GOOD) {
    std::cout << "Could not read stateNumber from state." << std::endl;
  }
  if (UA_Variant_isEmpty(&stateNumber) || !UA_Variant_isScalar(&stateNumber) || stateNumber.type != &UA_TYPES[UA_TYPES_UINT32]) {
    std::cout << "Invalid stateNumber" << std::endl;
  } else {
    ret.Number = *((std::uint32_t *)stateNumber.data);
  }
  UA_Variant_clear(&stateNumber);
  UA_LocalizedText dispName = UA_LOCALIZEDTEXT(NULL, NULL);
  status = UA_Server_readDisplayName(m_pServer, *stateObj.NodeId, &dispName);
  if (status != UA_STATUSCODE_GOOD) {
    std::cout << "Could not read display name from state." << std::endl;
  }

  ret.DispName.locale = dispName.locale.length != 0 ? std::string(dispName.locale.data, dispName.locale.data + dispName.locale.length) : "en";
  ret.DispName.text = std::string(dispName.text.data, dispName.text.data + dispName.text.length);
  ret.Id = stateObj;
  UA_LocalizedText_clear(&dispName);
  std::cout << "State: " << ret.DispName.text << std::endl;
  return ret;
}

void StateMachine::readTransitions(open62541Cpp::UA_NodeId transitionObj) {
  UA_BrowseDescription brDesc;
  UA_BrowseDescription_init(&brDesc);
  UA_NodeId_copy(transitionObj.NodeId, &brDesc.nodeId);
  brDesc.referenceTypeId = UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT);

  open62541Cpp::UA_NodeId typeNodeId(UA_NODEID_NUMERIC(0, UA_NS0ID_TRANSITIONTYPE));
  brDesc.nodeClassMask = UA_NODECLASS_OBJECT;

  brDesc.browseDirection = UA_BrowseDirection::UA_BROWSEDIRECTION_FORWARD;
  brDesc.includeSubtypes = UA_TRUE;
  brDesc.resultMask = UA_BROWSERESULTMASK_ALL;

  auto browseResult = UA_Server_browse(m_pServer, UA_UINT32_MAX, &brDesc);
  if (browseResult.statusCode != UA_STATUSCODE_GOOD) {
    std::cout << "Resutl not good: " << UA_StatusCode_name(browseResult.statusCode) << std::endl;
    std::cout << "Could not read Transitions from statemachine." << std::endl;
  }

  for (std::size_t i = 0; i < browseResult.referencesSize; ++i) {
    if (isTypeOrSubtype(m_pServer, typeNodeId, open62541Cpp::UA_NodeId(browseResult.references[i].typeDefinition.nodeId))) {
      m_transitions.push_back(readTransitionValues(open62541Cpp::UA_NodeId(browseResult.references[i].nodeId.nodeId)));
    }
  }

  UA_BrowseResult_clear(&browseResult);
  UA_BrowseDescription_clear(&brDesc);
}

StateMachine::Transition_t StateMachine::readTransitionValues(open62541Cpp::UA_NodeId transition) {
  Transition_t ret{{}, {}, {}, {}};
  open62541Cpp::UA_QualifiedName transitionNumber_qn(0, "TransitionNumber");
  UA_Variant transitionNumber;
  UA_Variant_init(&transitionNumber);
  auto status = UA_Server_readObjectProperty(m_pServer, *transition.NodeId, *transitionNumber_qn.QualifiedName, &transitionNumber);
  if (status != UA_STATUSCODE_GOOD) {
    std::cout << "Could not read transitionNumber from transition." << std::endl;
  }
  if (UA_Variant_isEmpty(&transitionNumber) || !UA_Variant_isScalar(&transitionNumber) || transitionNumber.type != &UA_TYPES[UA_TYPES_UINT32]) {
    std::cout << "Invalid transitionNumber" << std::endl;
  } else {
    ret.Number = *((std::uint32_t *)transitionNumber.data);
  }
  UA_Variant_clear(&transitionNumber);
  UA_LocalizedText dispName = UA_LOCALIZEDTEXT(NULL, NULL);
  status = UA_Server_readDisplayName(m_pServer, *transition.NodeId, &dispName);
  if (status != UA_STATUSCODE_GOOD) {
    std::cout << "Could not read display name from state." << std::endl;
  }

  ret.DispName.locale = dispName.locale.length != 0 ? std::string(dispName.locale.data, dispName.locale.data + dispName.locale.length) : "en";
  ret.DispName.text = std::string(dispName.text.data, dispName.text.data + dispName.text.length);
  UA_LocalizedText_clear(&dispName);

  auto fromStates = browseForChilds(
    m_pServer,
    transition,
    open62541Cpp::UA_NodeId(UA_NODEID_NUMERIC(0, UA_NS0ID_FROMSTATE)),
    open62541Cpp::UA_NodeId(UA_NODEID_NUMERIC(0, UA_NS0ID_STATETYPE)));
  if (fromStates.size() != 1) {
    std::cout << "No single from state found for transition." << std::endl;
  } else {
    ret.From = fromStates.front();
  }

  auto toStates = browseForChilds(
    m_pServer, transition, open62541Cpp::UA_NodeId(UA_NODEID_NUMERIC(0, UA_NS0ID_TOSTATE)), open62541Cpp::UA_NodeId(UA_NODEID_NUMERIC(0, UA_NS0ID_STATETYPE)));
  if (toStates.size() != 1) {
    std::cout << "No single to state found for transition." << std::endl;
  } else {
    ret.To = toStates.front();
  }

  return ret;
}

void StateMachine::mergeTransitions() {
  std::map<std::uint32_t, Transition_t> transitions;
  m_transitions.reverse();  // So later types will override base types
  for (auto &transition : m_transitions) {
    if (transition.Number == UA_UINT32_MAX) {
      // No real transition
      continue;
    }
    auto &trans = transitions[transition.Number];
    if (!UA_NodeId_isNull(transition.From.NodeId)) {
      trans.From = transition.From;
    }

    if (!UA_NodeId_isNull(transition.To.NodeId)) {
      trans.To = transition.To;
    }

    if (transition.DispName.text.size() != 0) {
      trans.DispName = transition.DispName;
    }
  }

  m_transitions.clear();

  for (auto &transition : transitions) {
    m_transitions.push_back(transition.second);
  }
}

}  // namespace UmatiServerLib
