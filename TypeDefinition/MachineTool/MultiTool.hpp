#pragma once
#include "../TypeDefinition.hpp"
#include "Constants.hpp"
#include "../../UmatiServerLib/BindableMemberPlaceholder.hpp"
#include "Tool.hpp"

namespace machineTool
{

  struct MultiTool_t : public BaseTool_t
  {
    BindableMemberPlaceholder<BindableMember, Tool_t> Tool;
  };

} // namespace machineTool

REFL_TYPE(machineTool::MultiTool_t,
          Bases<machineTool::BaseTool_t>(),
          UmatiServerLib::attribute::UaObjectType{
              .NodeId = UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_MULTITOOLTYPE)})
REFL_FIELD(Tool,
           UmatiServerLib::attribute::PlaceholderOptional(),
           UmatiServerLib::attribute::MemberInTypeNodeId{
               .NodeId = UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_MULTITOOLTYPE_TOOL)})

REFL_END
