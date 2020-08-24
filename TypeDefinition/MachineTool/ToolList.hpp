#pragma once
#include "../TypeDefinition.hpp"
#include "Constants.hpp"
#include "../ns0/Constants.hpp"
#include "Tool.hpp"
#include "MultiTool.hpp"
#include "../../UmatiServerLib/BindableMemberPlaceholder.hpp"

namespace machineTool
{

  struct ToolList_t
  {
    BindableMemberValue<std::string> NodeVersion;
    BindableMemberPlaceholder<BindableMember, std::variant<Tool_t, MultiTool_t>> Tool;
  };

} // namespace machineTool

REFL_TYPE(machineTool::ToolList_t,
          UmatiServerLib::attribute::UaObjectType{
              .NodeId = UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_TOOLLISTTYPE)})
REFL_FIELD(NodeVersion,
           UmatiServerLib::attribute::MemberInTypeNodeId{
               .NodeId = UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_TOOLLISTTYPE_NODEVERSION)},
               UmatiServerLib::attribute::UaBrowseName{.NsURI = constants::Ns0Uri},
           UmatiServerLib::attribute::PlaceholderOptional())
REFL_FIELD(Tool,
           UmatiServerLib::attribute::MemberInTypeNodeId{.NodeId = UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_TOOLLISTTYPE_TOOL)},
           UmatiServerLib::attribute::PlaceholderOptional())
REFL_END
