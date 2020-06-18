#pragma once
#include "../TypeDefiniton.hpp"
#include "Constants.hpp"
#include "StateVariable.hpp"

namespace ns0
{

struct TwoStateVariable_t : public StateVariable_t<bool>
{
  open62541Cpp::DateTime_t EffectiveTransitionTime;
  open62541Cpp::DateTime_t TransitionTime;
  open62541Cpp::LocalizedText_t FalseState;
  open62541Cpp::LocalizedText_t TrueState;
};

} // namespace ns0

REFL_TYPE(ns0::TwoStateVariable_t,
          Bases<ns0::StateVariable_t<bool>>(),
          open62541Cpp::attribute::UaVariableType{
              .NodeId = open62541Cpp::constexp::NodeId(constants::Ns0Uri, UA_NS0ID_TWOSTATEVARIABLETYPE)})
REFL_FIELD(Id)
REFL_FIELD(EffectiveTransitionTime, open62541Cpp::attribute::PlaceholderOptional())
REFL_FIELD(TransitionTime, open62541Cpp::attribute::PlaceholderOptional())
REFL_FIELD(FalseState, open62541Cpp::attribute::PlaceholderOptional())
REFL_FIELD(TrueState, open62541Cpp::attribute::PlaceholderOptional())
REFL_END
