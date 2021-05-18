#pragma once
#include "../TypeDefinition.hpp"
#include "../ns0/AlarmCondition.hpp"
#include "Constants.hpp"

namespace machineTool {

struct Alert_t : public ns0::AlarmCondition_t {
  std::string ErrorCode;
};

}  // namespace machineTool

REFL_TYPE(
  machineTool::Alert_t,
  Bases<ns0::AlarmCondition_t>(),
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_ALERTTYPE)))
REFL_FIELD(ErrorCode)
REFL_END
