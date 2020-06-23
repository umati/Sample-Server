#pragma once
#include "../TypeDefiniton.hpp"
#include "Constants.hpp"

namespace machineTool
{

struct Prognosis_t
{
  BindableMemberValue<UmatiServerLib::DateTime_t> PredictedTime;
};

} // namespace machineTool

REFL_TYPE(machineTool::Prognosis_t,
          UmatiServerLib::attribute::UaObjectType{
              .NodeId = UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PROGNOSISTYPE)})
REFL_FIELD(PredictedTime)
REFL_END
