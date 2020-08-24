#pragma once
#include "../TypeDefinition.hpp"
#include "Constants.hpp"
#include "Prognosis.hpp"

namespace machineTool
{

struct ProductionJobEndPrognosis_t : public Prognosis_t
{
  BindableMemberValue<UA_NodeId> JobNodeId;
  BindableMemberValue<std::string> SourceIdentifier;
};

} // namespace machineTool

REFL_TYPE(
    machineTool::ProductionJobEndPrognosis_t,
    Bases<machineTool::Prognosis_t>(),
    UmatiServerLib::attribute::UaObjectType{
        .NodeId = UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PRODUCTIONJOBENDPROGNOSISTYPE)})
REFL_FIELD(JobNodeId,
           UmatiServerLib::attribute::PlaceholderOptional(),
           UmatiServerLib::attribute::MemberInTypeNodeId{
               .NodeId = UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PRODUCTIONJOBENDPROGNOSISTYPE_JOBNODEID)})
REFL_FIELD(SourceIdentifier)
REFL_END
