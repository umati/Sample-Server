#pragma once
#include "../TypeDefinition.hpp"
#include "Constants.hpp"
#include "Prognosis.hpp"

namespace machineTool {

struct ToolLoadPrognosis_t : public Prognosis_t {
  BindableMemberValue<UmatiServerLib::LocalizedText_t> Location;
  BindableMemberValue<std::string> ToolIdentifier;
  BindableMemberValue<std::string> ToolName;
};

}  // namespace machineTool

REFL_TYPE(
  machineTool::ToolLoadPrognosis_t,
  Bases<machineTool::Prognosis_t>(),
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_TOOLLOADPROGNOSISTYPE)))
REFL_FIELD(Location)
REFL_FIELD(
  ToolName,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_TOOLLOADPROGNOSISTYPE_TOOLNAME)))
REFL_FIELD(
  ToolIdentifier,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(
    UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_TOOLLOADPROGNOSISTYPE_TOOLIDENTIFIER)))
REFL_END
