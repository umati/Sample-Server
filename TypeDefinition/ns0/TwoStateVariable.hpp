#pragma once
#include "../TypeDefinition.hpp"
#include "Constants.hpp"
#include "StateVariable.hpp"

namespace ns0 {

struct TwoStateVariable_t : public StateVariable_t<bool> {
  UmatiServerLib::DateTime_t EffectiveTransitionTime;
  UmatiServerLib::DateTime_t TransitionTime;
  UmatiServerLib::LocalizedText_t FalseState;
  UmatiServerLib::LocalizedText_t TrueState;
};

}  // namespace ns0

REFL_TYPE(
  ns0::TwoStateVariable_t,
  Bases<ns0::StateVariable_t<bool>>(),
  UmatiServerLib::attribute::UaVariableType(UmatiServerLib::constexp::NodeId(constants::Ns0Uri, UA_NS0ID_TWOSTATEVARIABLETYPE)))
REFL_FIELD(Id)
REFL_FIELD(EffectiveTransitionTime, UmatiServerLib::attribute::PlaceholderOptional())
REFL_FIELD(TransitionTime, UmatiServerLib::attribute::PlaceholderOptional())
REFL_FIELD(FalseState, UmatiServerLib::attribute::PlaceholderOptional())
REFL_FIELD(TrueState, UmatiServerLib::attribute::PlaceholderOptional())
REFL_END
