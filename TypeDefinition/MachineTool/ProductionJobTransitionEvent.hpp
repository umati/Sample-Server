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
};

} // namespace machineTool

REFL_TYPE(machineTool::ProductionJobTransitionEvent_t,
          Bases<ns0::TransitionEvent_t>(),
          UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PRODUCTIONJOBTRANSITIONEVENTTYPE)))
REFL_FIELD(Identifier)
REFL_FIELD(RunsCompleted)
REFL_FIELD(RunsPlanned)
REFL_END
