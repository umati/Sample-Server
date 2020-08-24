#pragma once
#include "../TypeDefinition.hpp"
#include "Constants.hpp"

namespace machineTool
{

struct BaseTool_t
{
  BindableMemberValue<std::string> Identifier;
  BindableMemberValue<UmatiServerLib::LocalizedText_t> Name;
};

} // namespace machineTool

REFL_TYPE(machineTool::BaseTool_t,
          UmatiServerLib::attribute::UaObjectType{
              .NodeId = UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_BASETOOLTYPE)})
REFL_FIELD(Identifier, UmatiServerLib::attribute::MemberInTypeNodeId{
               .NodeId = UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_BASETOOLTYPE_IDENTIFIER)},
           UmatiServerLib::attribute::PlaceholderOptional())
REFL_FIELD(Name, UmatiServerLib::attribute::MemberInTypeNodeId{
               .NodeId = UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_BASETOOLTYPE_NAME)},
           UmatiServerLib::attribute::PlaceholderOptional())
REFL_END
