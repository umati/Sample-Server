#pragma once
#include "../TypeDefiniton.hpp"
#include "Constants.hpp"

namespace machineTool
{

struct BaseTool_t
{
  BindableMemberValue<std::string> Identifier;
  BindableMemberValue<open62541Cpp::LocalizedText_t> Name;
};

} // namespace machineTool

REFL_TYPE(machineTool::BaseTool_t,
          open62541Cpp::attribute::UaObjectType{
              .NodeId = open62541Cpp::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_BASETOOLTYPE)})
REFL_FIELD(Identifier, open62541Cpp::attribute::MemberInTypeNodeId{
               .NodeId = open62541Cpp::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_BASETOOLTYPE_IDENTIFIER)},
           open62541Cpp::attribute::PlaceholderOptional())
REFL_FIELD(Name, open62541Cpp::attribute::MemberInTypeNodeId{
               .NodeId = open62541Cpp::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_BASETOOLTYPE_NAME)},
           open62541Cpp::attribute::PlaceholderOptional())
REFL_END
