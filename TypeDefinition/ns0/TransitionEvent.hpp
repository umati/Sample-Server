#pragma once
#include "../TypeDefinition.hpp"
#include "BaseEvent.hpp"
#include "Constants.hpp"
#include "StateVariable.hpp"
#include "TransitionVariable.hpp"

namespace ns0 {

struct TransitionEvent_t : public BaseEvent_t {
  BindableMember<TransitionVariable_t<UA_NodeId>> Transition;
  BindableMember<StateVariable_t<UA_NodeId>> FromState;
  BindableMember<StateVariable_t<UA_NodeId>> ToState;
};

}  // namespace ns0

REFL_TYPE(
  ns0::TransitionEvent_t,
  Bases<ns0::BaseEvent_t>(),
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::Ns0Uri, UA_NS0ID_TRANSITIONEVENTTYPE)))
REFL_FIELD(Transition)
REFL_FIELD(FromState)
REFL_FIELD(ToState)
REFL_END
