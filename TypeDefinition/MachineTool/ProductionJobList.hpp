/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2020-2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 * Copyright 2020 (c) Tonja Heinemann, ISW University of Stuttgart (for umati and VDW e.V.)
 */

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
