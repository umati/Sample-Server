#pragma once
#include "../TypeDefinition.hpp"
#include "Constants.hpp"
#include "TransitionVariable.hpp"

namespace ns0
{

  struct FiniteTransitionVariable_t : public TransitionVariable_t<UA_NodeId>
  {
  };

} // namespace ns0

REFL_TYPE(ns0::FiniteTransitionVariable_t,
          Bases<ns0::TransitionVariable_t<UA_NodeId>>(),
          UmatiServerLib::attribute::UaVariableType(UmatiServerLib::constexp::NodeId(constants::Ns0Uri, UA_NS0ID_FINITETRANSITIONVARIABLETYPE)))
REFL_END
