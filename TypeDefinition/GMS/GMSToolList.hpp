/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2022 (c) mindonwarp
 */

#pragma once

#include "../../UmatiServerLib/BindableMemberPlaceholder.hpp"
#include "../TypeDefinition.hpp"
#include "../MachineTool/ToolList.hpp"

#include "Constants.hpp"
#include "GMSSensor.hpp"

namespace GMS {

struct GMSToolList_t : public machineTool::ToolList_t {
  BindableMemberPlaceholder<BindableMember, std::variant<machineTool::Tool_t, machineTool::MultiTool_t, GMSSensor_t>> Tool;
};

}  // namespace GMS

REFL_TYPE(
  GMS::GMSToolList_t,
  Bases<machineTool::ToolList_t>(),
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_TOOLLISTTYPE)))
REFL_FIELD(
  Tool,
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_TOOLLISTTYPE_TOOL)),
  UmatiServerLib::attribute::PlaceholderOptional())
REFL_END