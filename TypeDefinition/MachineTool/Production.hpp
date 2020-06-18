#pragma once
#include "../TypeDefiniton.hpp"
#include "Constants.hpp"
#include "ProductionActiveProgram.hpp"
#include "ProductionJobList.hpp"

namespace machineTool
{

struct Production_t
{
  BindableMember<ProductionActiveProgram_t> ActiveProgram;
  BindableMember<ProductionJobList_t> ProductionPlan;
};

} // namespace machineTool

REFL_TYPE(machineTool::Production_t,
          open62541Cpp::attribute::UaObjectType{
              .NodeId = open62541Cpp::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PRODUCTIONTYPE)})
REFL_FIELD(ProductionPlan,
           open62541Cpp::attribute::PlaceholderOptional(),
           open62541Cpp::attribute::MemberInTypeNodeId{
               .NodeId = open62541Cpp::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PRODUCTIONTYPE_PRODUCTIONPLAN)})
REFL_FIELD(ActiveProgram)
REFL_END
