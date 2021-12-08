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

struct PartLoadPrognosis_t : public Prognosis_t {
  BindableMemberValue<UmatiServerLib::LocalizedText_t> Location;
  BindableMemberValue<std::string> PartIdentifier;
  BindableMemberValue<std::string> PartName;
  BindableMemberValue<UA_NodeId> PartNodeId;
};

}  // namespace machineTool

REFL_TYPE(
  machineTool::PartLoadPrognosis_t,
  Bases<machineTool::Prognosis_t>(),
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PARTLOADPROGNOSISTYPE)))
REFL_FIELD(Location)
REFL_FIELD(PartName)
REFL_FIELD(
  PartIdentifier,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(
    UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PARTLOADPROGNOSISTYPE_PARTIDENTIFIER)))
REFL_FIELD(
  PartNodeId,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(
    UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PARTLOADPROGNOSISTYPE_PARTNODEID)))
REFL_END
