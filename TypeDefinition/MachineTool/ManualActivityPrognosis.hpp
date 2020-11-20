#pragma once
#include "../TypeDefinition.hpp"
#include "Constants.hpp"
#include "Prognosis.hpp"

namespace machineTool
{

struct ManualActivityPrognosis_t : public Prognosis_t
{
  BindableMemberValue<UmatiServerLib::LocalizedText_t> Activity;
};

} // namespace machineTool

REFL_TYPE(
    machineTool::ManualActivityPrognosis_t,
    Bases<machineTool::Prognosis_t>(),
    UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_MANUALACTIVITYPROGNOSISTYPE)))
REFL_FIELD(Activity)
REFL_END
