#pragma once
#include "../TypeDefiniton.hpp"
#include "Constants.hpp"
#include "WorkingUnitMonitoring.hpp"
#include "../../src_generated/types_machinetool_generated.h"

namespace machineTool
{
  struct EDMGeneratorMonitoring_t : public WorkingUnitMonitoring_t
  {
    BindableMemberValue<UA_EDMGeneratorState> EDMGeneratorState;
    BindableMemberValue<bool> IsOn;
  };
} // namespace machineTool

REFL_TYPE(machineTool::EDMGeneratorMonitoring_t,
          Bases<machineTool::WorkingUnitMonitoring_t>(),
          open62541Cpp::attribute::UaObjectType{
              .NodeId = open62541Cpp::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_EDMGENERATORMONITORINGTYPE)})
REFL_FIELD(EDMGeneratorState)
REFL_FIELD(IsOn)
REFL_END
