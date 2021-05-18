#pragma once
#include "../TypeDefinition.hpp"
#include "Constants.hpp"

namespace machineTool {

struct BaseTool_t {
  BindableMemberValue<std::string> Identifier;
  BindableMemberValue<std::string> Name;
};

}  // namespace machineTool

REFL_TYPE(
  machineTool::BaseTool_t,
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_BASETOOLTYPE)))
REFL_FIELD(
  Identifier,
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_BASETOOLTYPE_IDENTIFIER)),
  UmatiServerLib::attribute::PlaceholderOptional())
REFL_FIELD(
  Name,
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_BASETOOLTYPE_NAME)),
  UmatiServerLib::attribute::PlaceholderOptional())
REFL_END
