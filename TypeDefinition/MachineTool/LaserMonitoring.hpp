#pragma once
#include "../../src_generated/types_machinetool_generated.h"
#include "../TypeDefinition.hpp"
#include "Constants.hpp"
#include "WorkingUnitMonitoring.hpp"

namespace machineTool {
struct LaserMonitoring_t : public WorkingUnitMonitoring_t {
  BindableMemberValue<UA_LaserState> LaserState;
  BindableMemberValue<bool> ControllerIsOn;
};
}  // namespace machineTool

REFL_TYPE(
  machineTool::LaserMonitoring_t,
  Bases<machineTool::WorkingUnitMonitoring_t>(),
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_LASERMONITORINGTYPE)))
REFL_FIELD(LaserState)
REFL_FIELD(ControllerIsOn)
REFL_END
