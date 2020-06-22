#pragma once
#include "../TypeDefiniton.hpp"
#include "Constants.hpp"
#include "../../BindableMemberPlaceholder.hpp"
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
          open62541Cpp::attribute::UaObjectType{
              .NodeId = open62541Cpp::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_MULTITOOLTYPE)})
REFL_FIELD(Tool,
           open62541Cpp::attribute::PlaceholderOptional(),
           open62541Cpp::attribute::MemberInTypeNodeId{
               .NodeId = open62541Cpp::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_MULTITOOLTYPE_TOOL)})

REFL_END
