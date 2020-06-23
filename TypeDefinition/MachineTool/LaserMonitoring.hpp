#pragma once
#include "../TypeDefiniton.hpp"
#include "Constants.hpp"
#include "WorkingUnitMonitoring.hpp"
#include "../../src_generated/types_machinetool_generated.h"

namespace machineTool
{
  struct LaserMonitoring_t : public WorkingUnitMonitoring_t
  {
    BindableMemberValue<UA_LaserState> LaserState;
    BindableMemberValue<bool> IsOn;
  };
} // namespace machineTool

REFL_TYPE(machineTool::LaserMonitoring_t,
          Bases<machineTool::WorkingUnitMonitoring_t>(),
          UmatiServerLib::attribute::UaObjectType{
              .NodeId = UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_LASERMONITORINGTYPE)})
REFL_FIELD(LaserState)
REFL_FIELD(IsOn)
REFL_END
