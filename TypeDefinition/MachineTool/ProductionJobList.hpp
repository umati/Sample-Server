#pragma once
#include "../TypeDefinition.hpp"
#include "../ns0/OrderedList.hpp"
#include "Constants.hpp"
#include "ProductionJob.hpp"

namespace machineTool {

struct ProductionJobList_t : public ns0::OrderedList_t<ProductionJob_t> {};

}  // namespace machineTool

REFL_TYPE(
  machineTool::ProductionJobList_t,
  Bases<ns0::OrderedList_t<machineTool::ProductionJob_t>>(),
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PRODUCTIONJOBLISTTYPE)))
REFL_FIELD(
  OrderedObjects,
  UmatiServerLib::attribute::MemberInTypeNodeId(
    UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PRODUCTIONJOBLISTTYPE_ORDEREDOBJECT_PLACEHOLDER)),
  UmatiServerLib::attribute::PlaceholderOptional())
REFL_END
