#pragma once
#include "../TypeDefiniton.hpp"
#include "Constants.hpp"

namespace machineTool
{

struct Prognosis_t
{
  BindableMemberValue<open62541Cpp::DateTime_t> PredictedTime;
};

} // namespace machineTool

REFL_TYPE(machineTool::Prognosis_t,
          open62541Cpp::attribute::UaObjectType{
              .NodeId = open62541Cpp::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PROGNOSISTYPE)})
REFL_FIELD(PredictedTime)
REFL_END
