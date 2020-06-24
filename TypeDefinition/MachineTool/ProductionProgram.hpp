#pragma once
#include "../TypeDefiniton.hpp"
#include "Constants.hpp"
#include "ProductionStateMachine.hpp"
#include "../ns0/IOrderedObject.hpp"

namespace machineTool
{

  struct ProductionProgram_t : public ns0::IOrderedObject_t<std::uint16_t>
  {
    BindableMemberValue<std::string> Name;
    BindableMember<ProductionStateMachine_t> State;
  };

} // namespace machineTool

REFL_TYPE(machineTool::ProductionProgram_t,
          Bases<ns0::IOrderedObject_t<std::uint16_t>>(),
          UmatiServerLib::attribute::UaObjectType{
              .NodeId = UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PRODUCTIONPROGRAMTYPE)})
REFL_FIELD(Name)
REFL_FIELD(State,
           UmatiServerLib::attribute::MemberInTypeNodeId{
               .NodeId = UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PRODUCTIONPROGRAMTYPE_STATE)},
           UmatiServerLib::attribute::PlaceholderOptional())
REFL_END
