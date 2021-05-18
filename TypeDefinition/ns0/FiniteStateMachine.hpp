#pragma once
#include "../TypeDefinition.hpp"
#include "Constants.hpp"
#include "FiniteStateVariable.hpp"
#include "FiniteTransitionVariable.hpp"

namespace ns0 {

///\todo correc inheritance
struct FiniteStateMachine_t {
  BindableMember<FiniteStateVariable_t> CurrentState;
  BindableMember<FiniteTransitionVariable_t> LastTransition;
};

}  // namespace ns0

REFL_TYPE(
  ns0::FiniteStateMachine_t, UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::Ns0Uri, UA_NS0ID_FINITESTATEMACHINETYPE)))
REFL_FIELD(CurrentState)
REFL_FIELD(LastTransition, UmatiServerLib::attribute::PlaceholderOptional())
REFL_END
