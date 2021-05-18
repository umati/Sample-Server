#pragma once
#include "../TypeDefinition.hpp"
#include "Constants.hpp"
#include "ProductionActiveProgram.hpp"
#include "ProductionJobList.hpp"

namespace machineTool {

struct Production_t {
  BindableMember<ProductionActiveProgram_t> ActiveProgram;
  BindableMember<ProductionJobList_t> ProductionPlan;
};

}  // namespace machineTool

REFL_TYPE(
  machineTool::Production_t,
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PRODUCTIONTYPE)))
REFL_FIELD(
  ProductionPlan,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PRODUCTIONTYPE_PRODUCTIONPLAN)))
REFL_FIELD(ActiveProgram)
REFL_END
