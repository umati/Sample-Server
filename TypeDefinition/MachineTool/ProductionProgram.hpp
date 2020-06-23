#pragma once
#include "../TypeDefiniton.hpp"
#include "Constants.hpp"
#include "ProductionStateMachine.hpp"

namespace machineTool
{

struct ProductionProgram_t
{
  BindableMemberValue<std::string> Name;
  BindableMember<ProductionStateMachine_t> State;
};

} // namespace machineTool

REFL_TYPE(machineTool::ProductionProgram_t,
          UmatiServerLib::attribute::UaObjectType{
              .NodeId = UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PRODUCTIONPROGRAMTYPE)})
REFL_FIELD(Name)
REFL_FIELD(State,
           UmatiServerLib::attribute::MemberInTypeNodeId{
               .NodeId = UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PRODUCTIONPROGRAMTYPE_STATE)},
           UmatiServerLib::attribute::PlaceholderOptional())
REFL_END
