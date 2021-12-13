/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2020-2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 * Copyright 2020 (c) Tonja Heinemann, ISW University of Stuttgart (for umati and VDW e.V.)
 */

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
