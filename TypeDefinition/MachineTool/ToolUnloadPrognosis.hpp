#pragma once
#include "../TypeDefiniton.hpp"
#include "Constants.hpp"
#include "Prognosis.hpp"

namespace machineTool
{

  struct ToolUnloadPrognosis_t : public Prognosis_t
  {
    BindableMemberValue<UmatiServerLib::LocalizedText_t> Location;
    BindableMemberValue<std::string> ToolIdentifier;
    BindableMemberValue<std::string> ToolName;
    BindableMemberValue<UA_NodeId> ToolNodeId;
  };

} // namespace machineTool

REFL_TYPE(
    machineTool::ToolUnloadPrognosis_t,
    Bases<machineTool::Prognosis_t>(),
    UmatiServerLib::attribute::UaObjectType{
        .NodeId = UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_TOOLUNLOADPROGNOSISTYPE)})
REFL_FIELD(Location)
REFL_FIELD(ToolName,
           UmatiServerLib::attribute::PlaceholderOptional(),
           UmatiServerLib::attribute::MemberInTypeNodeId{
               .NodeId = UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_TOOLUNLOADPROGNOSISTYPE_TOOLNAME)})
REFL_FIELD(ToolIdentifier,
           UmatiServerLib::attribute::PlaceholderOptional(),
           UmatiServerLib::attribute::MemberInTypeNodeId{
               .NodeId = UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_TOOLUNLOADPROGNOSISTYPE_TOOLIDENTIFIER)})
REFL_FIELD(ToolNodeId,
           UmatiServerLib::attribute::PlaceholderOptional(),
           UmatiServerLib::attribute::MemberInTypeNodeId{
               .NodeId = UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_TOOLUNLOADPROGNOSISTYPE_TOOLNODEID)})
REFL_END
