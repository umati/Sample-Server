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
#include "Prognosis.hpp"

namespace machineTool {

struct ToolUnloadPrognosis_t : public Prognosis_t {
  BindableMemberValue<UmatiServerLib::LocalizedText_t> Location;
  BindableMemberValue<std::string> ToolIdentifier;
  BindableMemberValue<std::string> ToolName;
  BindableMemberValue<UA_NodeId> ToolNodeId;
};

}  // namespace machineTool

REFL_TYPE(
  machineTool::ToolUnloadPrognosis_t,
  Bases<machineTool::Prognosis_t>(),
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_TOOLUNLOADPROGNOSISTYPE)))
REFL_FIELD(Location)
REFL_FIELD(
  ToolName,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(
    UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_TOOLUNLOADPROGNOSISTYPE_TOOLNAME)))
REFL_FIELD(
  ToolIdentifier,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(
    UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_TOOLUNLOADPROGNOSISTYPE_TOOLIDENTIFIER)))
REFL_FIELD(
  ToolNodeId,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(
    UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_TOOLUNLOADPROGNOSISTYPE_TOOLNODEID)))
REFL_END
