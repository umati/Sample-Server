#pragma once

#include <Open62541Cpp/UA_NodeId.hpp>
#include <open62541/server.h>
#include "../OpcUaTypes/LocalizedText.hpp"
#include <list>

namespace UmatiServerLib {
  class StateMachine{
    public:
    StateMachine(open62541Cpp::UA_NodeId stateMachineType, UA_Server *pServer);

    protected:
    struct State_t{
      LocalizedText_t Name;
      std::uint32_t Number;
      open62541Cpp::UA_NodeId Id;
    };

    struct Transition_t {
      open62541Cpp::UA_NodeId From;
      open62541Cpp::UA_NodeId To;
      std::uint32_t Number;
    };
    void readStates();
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
}
