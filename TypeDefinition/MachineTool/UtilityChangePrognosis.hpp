#pragma once
#include "../TypeDefiniton.hpp"
#include "Constants.hpp"
#include "Prognosis.hpp"

namespace machineTool
{

struct UtilityChangePrognosis_t : public Prognosis_t
{
  BindableMemberValue<UmatiServerLib::LocalizedText_t> UtilityName;
};

} // namespace machineTool

REFL_TYPE(
    machineTool::UtilityChangePrognosis_t,
    Bases<machineTool::Prognosis_t>(),
    UmatiServerLib::attribute::UaObjectType{
        .NodeId = UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_UTILITYCHANGEPROGNOSISTYPE)})
REFL_FIELD(UtilityName)
REFL_END
