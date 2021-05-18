#pragma once

#include <open62541/server.h>

#include <Open62541Cpp/UA_NodeId.hpp>
#include <list>

#include "../OpcUaTypes/LocalizedText.hpp"

namespace UmatiServerLib {
/**
 * @brief Read a state machine from the address space
 *
 * This class reads all states and transitions from a state machine type with all information needed for setting them later on.
 */
class StateMachine {
 public:
  StateMachine(open62541Cpp::UA_NodeId stateMachineType, UA_Server *pServer);

 protected:
  struct State_t {
    LocalizedText_t DispName;
    std::uint32_t Number;
    open62541Cpp::UA_NodeId Id;
  };

  struct Transition_t {
    LocalizedText_t DispName;
    open62541Cpp::UA_NodeId From;
    open62541Cpp::UA_NodeId To;
    std::uint32_t Number = UA_UINT32_MAX;
  };
  void readStatesAndTransitions();
  void mergeTransitions();
  void readStates(open62541Cpp::UA_NodeId stateMachineType);
  State_t readStateValues(open62541Cpp::UA_NodeId stateObj);

  void readTransitions();
  void readTransitions(open62541Cpp::UA_NodeId transitionObj);
  Transition_t readTransitionValues(open62541Cpp::UA_NodeId transition);

  std::list<State_t> m_states;
  std::list<Transition_t> m_transitions;
  UA_Server *m_pServer;
  open62541Cpp::UA_NodeId m_stateMachineType;
};
}  // namespace UmatiServerLib
