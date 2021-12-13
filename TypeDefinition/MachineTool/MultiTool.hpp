/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2020-2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 * Copyright 2020 (c) Tonja Heinemann, ISW University of Stuttgart (for umati and VDW e.V.)
 */

#pragma once
#include "../../UmatiServerLib/BindableMemberPlaceholder.hpp"
#include "../TypeDefinition.hpp"
#include "Constants.hpp"
#include "Tool.hpp"

namespace machineTool {

struct MultiTool_t : public BaseTool_t {
  BindableMemberPlaceholder<BindableMember, Tool_t> Tool;
};

}  // namespace machineTool

REFL_TYPE(
  machineTool::MultiTool_t,
  Bases<machineTool::BaseTool_t>(),
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_MULTITOOLTYPE)))
REFL_FIELD(
  Tool,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_MULTITOOLTYPE_TOOL)))

REFL_END
