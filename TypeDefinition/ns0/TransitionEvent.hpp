#pragma once
#include "../TypeDefinition.hpp"
#include "Constants.hpp"
#include "BaseEvent.hpp"
#include "Transitionvariable.hpp"
#include "Statevariable.hpp"

namespace ns0
{

struct TransitionEvent_t : public BaseEvent_t
{
     BindableMemberValue<TransitionVariable_t<UmatiServerLib::LocalizedText_t>> Transition;
     BindableMemberValue<StateVariable_t<UmatiServerLib::LocalizedText_t>> FromState;
     BindableMemberValue<StateVariable_t<UmatiServerLib::LocalizedText_t>> ToState;
};

} // namespace ns0

REFL_TYPE(ns0::TransitionEvent_t,
        UmatiServerLib::attribute::UaObjectType{
          Bases<ns0::BaseEvent_t>(),
            .NodeId = UmatiServerLib::constexp::NodeId(constants::Ns0Uri, UA_NS0ID_TRANSITIONEVENTTYPE)})
REFL_FIELD(Transition)
REFL_FIELD(FromState)
REFL_FIELD(ToState)
REFL_END
