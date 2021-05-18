#pragma once
#include "../TypeDefinition.hpp"
#include "Constants.hpp"
#include "Prognosis.hpp"

namespace machineTool {

struct ToolChangePrognosis_t : public Prognosis_t {
  BindableMemberValue<UmatiServerLib::LocalizedText_t> Location;
  BindableMemberValue<std::string> ToolIdentifier;
  BindableMemberValue<std::string> ToolName;
  BindableMemberValue<UA_NodeId> ToolNodeId;
};

}  // namespace machineTool

REFL_TYPE(
  machineTool::ToolChangePrognosis_t,
  Bases<machineTool::Prognosis_t>(),
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_TOOLCHANGEPROGNOSISTYPE)))
REFL_FIELD(Location)
REFL_FIELD(
  ToolName,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(
    UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_TOOLCHANGEPROGNOSISTYPE_TOOLNAME)))
REFL_FIELD(
  ToolIdentifier,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(
    UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_TOOLCHANGEPROGNOSISTYPE_TOOLIDENTIFIER)))
REFL_FIELD(
  ToolNodeId,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(
    UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_TOOLCHANGEPROGNOSISTYPE_TOOLNODEID)))
REFL_END
