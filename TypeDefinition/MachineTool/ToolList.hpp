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
#include "../ns0/Constants.hpp"
#include "Constants.hpp"
#include "MultiTool.hpp"
#include "Tool.hpp"

namespace machineTool {

struct ToolList_t {
  BindableMemberValue<std::string> NodeVersion;
  BindableMemberPlaceholder<BindableMember, std::variant<Tool_t, MultiTool_t>> Tool;
};

}  // namespace machineTool

REFL_TYPE(
  machineTool::ToolList_t,
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_TOOLLISTTYPE)))
REFL_FIELD(
  NodeVersion,
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_TOOLLISTTYPE_NODEVERSION)),
  UmatiServerLib::attribute::UaBrowseName(constants::Ns0Uri),
  UmatiServerLib::attribute::PlaceholderOptional())
REFL_FIELD(
  Tool,
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_TOOLLISTTYPE_TOOL)),
  UmatiServerLib::attribute::PlaceholderOptional())
REFL_END
