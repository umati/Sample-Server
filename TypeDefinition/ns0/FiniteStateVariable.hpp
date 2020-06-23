#pragma once
#include "../TypeDefiniton.hpp"
#include "Constants.hpp"
#include "StateVariable.hpp"

namespace ns0
{

//\todo correct inheritance
struct FiniteStateVariable_t : public StateVariable_t<UA_NodeId>
{
};

} // namespace ns0

REFL_TYPE(
    ns0::FiniteStateVariable_t,
    Bases<ns0::StateVariable_t<UA_NodeId>>(),
    UmatiServerLib::attribute::UaVariableType{
        .NodeId = UmatiServerLib::constexp::NodeId(constants::Ns0Uri, UA_NS0ID_FINITESTATEVARIABLETYPE)})
REFL_FIELD(Value, UmatiServerLib::attribute::UaVariableTypeValue())
REFL_END
