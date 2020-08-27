#include "StateMachine.hpp"

#include <Open62541Cpp/UA_QualifiedName.hpp>
#include <iostream>
#include <sstream>
#include "Util.hpp"

namespace UmatiServerLib
{

  StateMachine::StateMachine(open62541Cpp::UA_NodeId stateMachineType, UA_Server *pServer)
      : m_pServer(pServer), m_stateMachineType(stateMachineType)
  {
    readStates();
  }

  void StateMachine::readStates()
  {
    open62541Cpp::UA_NodeId stateMachineType = m_stateMachineType;
    auto finiteStateMachineNsId = UA_NODEID_NUMERIC(0, UA_NS0ID_FINITESTATEMACHINETYPE);
    
    while(!UA_NodeId_equal(stateMachineType.NodeId, &finiteStateMachineNsId))
    {
      readStates(stateMachineType);
      auto parent = browseForParent(stateMachineType, open62541Cpp::UA_NodeId(UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE)), m_pServer);
      if(UA_NodeId_isNull(parent.NodeId))
      {
        break;
      }
      stateMachineType = parent;
    }

  }

  void StateMachine::readStates(open62541Cpp::UA_NodeId stateMachineType)
  {
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
    if (browseResult.statusCode != UA_STATUSCODE_GOOD)
    {
      std::cout << "Resutl not good: " << UA_StatusCode_name(browseResult.statusCode) << std::endl;
      std::cout << "Could not read States from statemachine." << std::endl;
    }

    for (std::size_t i = 0; i < browseResult.referencesSize; ++i)
    {
      if (UA_NodeId_equal(&browseResult.references[i].typeDefinition.nodeId, typeNodeId.NodeId))
      {
        m_states.push_back(readStateValues(open62541Cpp::UA_NodeId(browseResult.references[i].nodeId.nodeId)));
      }
    }

    UA_BrowseResult_deleteMembers(&browseResult);
    UA_BrowseDescription_deleteMembers(&brDesc);
  }

  StateMachine::State_t StateMachine::readStateValues(open62541Cpp::UA_NodeId stateObj)
  {
    State_t ret {{}, UA_UINT32_MAX, {} };
    open62541Cpp::UA_QualifiedName StateNumber_qn(0, "StateNumber");
    UA_Variant stateNumber;
    UA_Variant_init(&stateNumber);
    auto status = UA_Server_readObjectProperty(m_pServer, *stateObj.NodeId, *StateNumber_qn.QualifiedName, &stateNumber);
    if(status != UA_STATUSCODE_GOOD)
    {
      std::cout << "Could not read stateNumber from state." << std::endl;
    }
    if (UA_Variant_isEmpty(&stateNumber) || !UA_Variant_isScalar(&stateNumber) || stateNumber.type != &UA_TYPES[UA_TYPES_UINT32])
    {
      std::cout << "Invalid stateNumber" << std::endl;
    }
    else
    {
      ret.Number = *((std::uint32_t*) stateNumber.data);
    }
    UA_Variant_clear(&stateNumber);
    UA_LocalizedText dispName = UA_LOCALIZEDTEXT(NULL, NULL);
    status = UA_Server_readDisplayName(m_pServer, *stateObj.NodeId, &dispName);
    if(status != UA_STATUSCODE_GOOD)
    {
      std::cout << "Could not read display name from state." << std::endl;
    }
    ret.Name.locale = std::string(dispName.locale.data, dispName.locale.data + dispName.locale.length);
    ret.Name.text = std::string(dispName.text.data, dispName.text.data + dispName.text.length);
    UA_LocalizedText_clear(&dispName);

    return ret;
  }

  void StateMachine::readTransitions(open62541Cpp::UA_NodeId transitionObj)
  {
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
    if (browseResult.statusCode != UA_STATUSCODE_GOOD)
    {
      std::cout << "Resutl not good: " << UA_StatusCode_name(browseResult.statusCode) << std::endl;
      std::cout << "Could not read Transitions from statemachine." << std::endl;
    }

    for (std::size_t i = 0; i < browseResult.referencesSize; ++i)
    {
      if (UA_NodeId_equal(&browseResult.references[i].typeDefinition.nodeId, typeNodeId.NodeId))
      {
        m_transitions.push_back(readTransitionValues(open62541Cpp::UA_NodeId(browseResult.references[i].nodeId.nodeId)));
        //m_states.push_back(readStateValues(open62541Cpp::UA_NodeId(browseResult.references[i].nodeId.nodeId)));
      }
    }

    UA_BrowseResult_deleteMembers(&browseResult);
    UA_BrowseDescription_deleteMembers(&brDesc);
  }

} // namespace UmatiServerLib
