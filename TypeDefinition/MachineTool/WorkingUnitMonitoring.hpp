#pragma once
#include "../TypeDefinition.hpp"
#include "Constants.hpp"
#include "ElementMonitoring.hpp"

namespace machineTool
{
  struct WorkingUnitMonitoring_t : public ElementMonitoring_t
  {
  };
} // namespace machineTool

REFL_TYPE(machineTool::WorkingUnitMonitoring_t,
          Bases<machineTool::ElementMonitoring_t>(),
          UmatiServerLib::attribute::UaObjectType{
              .NodeId = UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_WORKINGUNITMONITORINGTYPE)})
REFL_END
