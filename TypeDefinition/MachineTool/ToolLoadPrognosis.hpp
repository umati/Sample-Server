#pragma once
#include "../TypeDefiniton.hpp"
#include "Constants.hpp"
#include "Prognosis.hpp"

namespace machineTool
{

  struct ToolLoadPrognosis_t : public Prognosis_t
  {
    BindableMemberValue<open62541Cpp::LocalizedText_t> Location;
    BindableMemberValue<std::string> ToolIdentifier;
    BindableMemberValue<std::string> ToolName;
  };

} // namespace machineTool

REFL_TYPE(
    machineTool::ToolLoadPrognosis_t,
    Bases<machineTool::Prognosis_t>(),
    open62541Cpp::attribute::UaObjectType{
        .NodeId = open62541Cpp::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_TOOLLOADPROGNOSISTYPE)})
REFL_FIELD(Location)
REFL_FIELD(ToolName,
           open62541Cpp::attribute::PlaceholderOptional(),
           open62541Cpp::attribute::MemberInTypeNodeId{
               .NodeId = open62541Cpp::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_TOOLLOADPROGNOSISTYPE_TOOLNAME)})
REFL_FIELD(ToolIdentifier,
           open62541Cpp::attribute::PlaceholderOptional(),
           open62541Cpp::attribute::MemberInTypeNodeId{
               .NodeId = open62541Cpp::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_TOOLLOADPROGNOSISTYPE_TOOLIDENTIFIER)})
REFL_END
