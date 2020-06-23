#pragma once
#include "../TypeDefiniton.hpp"
#include "Constants.hpp"
#include "Prognosis.hpp"

namespace machineTool
{

  struct PartUnLoadPrognosis_t : public Prognosis_t
  {
    BindableMemberValue<UmatiServerLib::LocalizedText_t> Location;
    BindableMemberValue<std::string> PartIdentifier;
    BindableMemberValue<std::string> PartName;
    BindableMemberValue<UA_NodeId> PartNodeId;
  };

} // namespace machineTool

REFL_TYPE(
    machineTool::PartUnLoadPrognosis_t,
    Bases<machineTool::Prognosis_t>(),
    UmatiServerLib::attribute::UaObjectType{
        .NodeId = UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PARTUNLOADPROGNOSISTYPE)})
REFL_FIELD(Location)
REFL_FIELD(PartName)
REFL_FIELD(PartIdentifier,
           UmatiServerLib::attribute::PlaceholderOptional(),
           UmatiServerLib::attribute::MemberInTypeNodeId{
               .NodeId = UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PARTUNLOADPROGNOSISTYPE_PARTIDENTIFIER)})
REFL_FIELD(PartNodeId,
           UmatiServerLib::attribute::PlaceholderOptional(),
           UmatiServerLib::attribute::MemberInTypeNodeId{
               .NodeId = UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PARTUNLOADPROGNOSISTYPE_PARTNODEID)})
REFL_END
