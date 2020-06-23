#pragma once
#include "../TypeDefiniton.hpp"
#include "Constants.hpp"
#include "ToolList.hpp"

namespace machineTool
{

  struct Equipment_t
  {
    BindableMember<ToolList_t> Tools;
  };

} // namespace machineTool

REFL_TYPE(machineTool::Equipment_t,
          UmatiServerLib::attribute::UaObjectType{
              .NodeId = UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_EQUIPMENTTYPE)})
REFL_FIELD(Tools,
           UmatiServerLib::attribute::MemberInTypeNodeId{
               .NodeId = UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_EQUIPMENTTYPE_TOOLS)},
           UmatiServerLib::attribute::PlaceholderOptional())
REFL_END
