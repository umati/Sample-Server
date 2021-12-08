/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2020-2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 * Copyright 2020 (c) Tonja Heinemann, ISW University of Stuttgart (for umati and VDW e.V.)
 */

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
