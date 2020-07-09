#pragma once
#include "../TypeDefiniton.hpp"
#include "Constants.hpp"
#include "../ns0/IOrderedObject.hpp"

namespace machineTool
{

struct ProductionJob_t : public ns0::IOrderedObject_t<std::uint16_t>
{
  BindableMemberValue<std::string> Identifier;
  BindableMemberValue<std::uint32_t> RunsCompleted;
  BindableMemberValue<std::uint32_t> RunsPlanned;
  BindableMember<ProductionStateMachine_t> State;
  //Member Programs Liste (Liste v. ProductionProgram_T)
};

} // namespace machineTool

REFL_TYPE(machineTool::ProductionJob_t,
          Bases<ns0::IOrderedObject_t<std::uint16_t>>(),
          UmatiServerLib::attribute::UaObjectType{
              .NodeId = UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PRODUCTIONJOBTYPE)})
REFL_FIELD(Identifier)
REFL_FIELD(RunsCompleted)
REFL_FIELD(RunsPlanned)
REFL_FIELD(State)
REFL_END
