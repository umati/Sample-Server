#pragma once
#include "../TypeDefiniton.hpp"
#include "Constants.hpp"

namespace ns0
{

///\todo correc inheritance
struct FiniteStateMachine_t
{
  BindableMember<FiniteStateVariable_t> CurrentState;
  BindableMember<FiniteTransitionVariable_t> LastTransition;
};

} // namespace ns0

REFL_TYPE(ns0::FiniteStateMachine_t,
          open62541Cpp::attribute::UaObjectType{
              .NodeId = open62541Cpp::constexp::NodeId(constants::Ns0Uri, UA_NS0ID_FINITESTATEMACHINETYPE)})
REFL_FIELD(CurrentState)
REFL_FIELD(LastTransition, open62541Cpp::attribute::PlaceholderOptional())
REFL_END
