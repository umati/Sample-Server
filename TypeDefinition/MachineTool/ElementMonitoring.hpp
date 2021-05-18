#pragma once
#include "../TypeDefinition.hpp"
#include "Constants.hpp"

namespace machineTool {
struct ElementMonitoring_t {
  BindableMemberValue<std::string> Name;
};
}  // namespace machineTool

REFL_TYPE(
  machineTool::ElementMonitoring_t,
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_ELEMENTMONITORINGTYPE)))
REFL_FIELD(Name)
REFL_END
