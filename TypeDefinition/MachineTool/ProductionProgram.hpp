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
          open62541Cpp::attribute::UaObjectType{
              .NodeId = open62541Cpp::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PRODUCTIONPROGRAMTYPE)})
REFL_FIELD(Name)
REFL_FIELD(State,
           open62541Cpp::attribute::MemberInTypeNodeId{
               .NodeId = open62541Cpp::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PRODUCTIONPROGRAMTYPE_STATE)},
           open62541Cpp::attribute::PlaceholderOptional())
REFL_END
