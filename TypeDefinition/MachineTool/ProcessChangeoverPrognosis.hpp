#pragma once
#include "../TypeDefiniton.hpp"
#include "Constants.hpp"
#include "Prognosis.hpp"

namespace machineTool
{

struct ProcessChangeoverPrognosis_t : public Prognosis_t
{
  BindableMemberValue<UmatiServerLib::LocalizedText_t> Activity;
  BindableMemberValue<UmatiServerLib::LocalizedText_t> Location;
};

} // namespace machineTool

REFL_TYPE(
    machineTool::ProcessChangeoverPrognosis_t,
    Bases<machineTool::Prognosis_t>(),
    UmatiServerLib::attribute::UaObjectType{
        .NodeId = UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PROCESSCHANGEOVERPROGNOSISTYPE)})
REFL_FIELD(Activity)
REFL_FIELD(Location)
REFL_END
