#pragma once
#include "../TypeDefiniton.hpp"
#include "Constants.hpp"
#include "../ns0/Constants.hpp"
#include "Tool.hpp"
#include "MultiTool.hpp"
#include "../../BindableMemberPlaceholder.hpp"

namespace machineTool
{

  struct ToolList_t
  {
    BindableMemberValue<std::string> NodeVersion;
    BindableMemberPlaceholder<BindableMember, std::variant<Tool_t, MultiTool_t>> Tool;
  };

} // namespace machineTool

REFL_TYPE(machineTool::ToolList_t,
          open62541Cpp::attribute::UaObjectType{
              .NodeId = open62541Cpp::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_TOOLLISTTYPE)})
REFL_FIELD(NodeVersion,
           open62541Cpp::attribute::MemberInTypeNodeId{
               .NodeId = open62541Cpp::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_TOOLLISTTYPE_NODEVERSION)},
               open62541Cpp::attribute::UaBrowseName{.NsURI = constants::Ns0Uri},
           open62541Cpp::attribute::PlaceholderOptional())
REFL_FIELD(Tool,
           open62541Cpp::attribute::MemberInTypeNodeId{.NodeId = open62541Cpp::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_TOOLLISTTYPE_TOOL)},
           open62541Cpp::attribute::PlaceholderOptional())
REFL_END
