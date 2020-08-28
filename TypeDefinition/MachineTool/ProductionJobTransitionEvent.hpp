#pragma once
#include "../TypeDefinition.hpp"
#include "Constants.hpp"
#include "../ns0/BaseEvent.hpp"
#include "../ns0/TransitionEvent.hpp"
#include "../MachineTool/ProductionJob.hpp" 

namespace machineTool
{

struct ProductionJobTransitionEvent_t: public ns0::TransitionEvent_t
{
    BindableMemberValue<std::string> Identifier;
//    std::string CustomerOrderIdentifier; //optional
//    std::string OrderIdentifier; //optional
    BindableMemberValue<std::uint32_t> RunsCompleted;
    BindableMember<ProductionJob_RunsPlanned_t> RunsPlanned;

    BindableMember<Production_FiniteTransitionVariable_t> Transition;
    BindableMember<Production_FiniteStateVariable_t> FromState;
    BindableMember<Production_FiniteStateVariable_t> ToState;

};

} // namespace machineTool

REFL_TYPE(machineTool::ProductionJobTransitionEvent_t,
          Bases<ns0::TransitionEvent_t>(),
          UmatiServerLib::attribute::UaObjectType{
              .NodeId = UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PRODUCTIONJOBTRANSITIONEVENTTYPE)})
REFL_FIELD(Identifier)
REFL_FIELD(RunsCompleted)
REFL_FIELD(RunsPlanned)
REFL_FIELD(Transition)
REFL_FIELD(FromState)
REFL_FIELD(ToState)
REFL_END
