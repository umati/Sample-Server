#pragma once
#include "../TypeDefiniton.hpp"
#include "Constants.hpp"
#include "Prognosis.hpp"

namespace machineTool
{

  struct PartLoadPrognosis_t : public Prognosis_t
  {
    BindableMemberValue<open62541Cpp::LocalizedText_t> Location;
    BindableMemberValue<std::string> PartIdentifier;
    BindableMemberValue<std::string> PartName;
    BindableMemberValue<UA_NodeId> PartNodeId;
  };

} // namespace machineTool

REFL_TYPE(
    machineTool::PartLoadPrognosis_t,
    Bases<machineTool::Prognosis_t>(),
    open62541Cpp::attribute::UaObjectType{
        .NodeId = open62541Cpp::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PARTLOADPROGNOSISTYPE)})
REFL_FIELD(Location)
REFL_FIELD(PartName)
REFL_FIELD(PartIdentifier,
           open62541Cpp::attribute::PlaceholderOptional(),
           open62541Cpp::attribute::MemberInTypeNodeId{
               .NodeId = open62541Cpp::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PARTLOADPROGNOSISTYPE_PARTIDENTIFIER)})
REFL_FIELD(PartNodeId,
           open62541Cpp::attribute::PlaceholderOptional(),
           open62541Cpp::attribute::MemberInTypeNodeId{
               .NodeId = open62541Cpp::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PARTLOADPROGNOSISTYPE_PARTNODEID)})
REFL_END
