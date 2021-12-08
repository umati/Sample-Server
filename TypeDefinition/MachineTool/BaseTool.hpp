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

namespace machineTool {

struct BaseTool_t {
  BindableMemberValue<std::string> Identifier;
  BindableMemberValue<std::string> Name;
};

}  // namespace machineTool

REFL_TYPE(
  machineTool::BaseTool_t,
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_BASETOOLTYPE)))
REFL_FIELD(
  Identifier,
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_BASETOOLTYPE_IDENTIFIER)),
  UmatiServerLib::attribute::PlaceholderOptional())
REFL_FIELD(
  Name,
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_BASETOOLTYPE_NAME)),
  UmatiServerLib::attribute::PlaceholderOptional())
REFL_END
