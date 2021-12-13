/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2020-2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 */

#pragma once
#include <string>

#include "../../src_generated/types_machinetool_generated.h"
#include "../TypeDefinition.hpp"
#include "Constants.hpp"

namespace machineTool {

struct ProductionPart_t {
  BindableMemberValue<std::string> CustomerOrderIdentifier;
  BindableMemberValue<std::string> Identifier;
  BindableMemberValue<std::string> Name;
  BindableMemberValue<UA_PartQuality> PartQuality;
  BindableMemberValue<UA_ProcessIrregularity> ProcessIrregularity;
};

}  // namespace machineTool

REFL_TYPE(
  machineTool::ProductionPart_t,
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PRODUCTIONPARTTYPE)))
REFL_FIELD(
  CustomerOrderIdentifier,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(
    UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PRODUCTIONPARTTYPE_CUSTOMERORDERIDENTIFIER)))
REFL_FIELD(
  Identifier,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PRODUCTIONPARTTYPE_IDENTIFIER)))
REFL_FIELD(Name)
REFL_FIELD(PartQuality)
REFL_FIELD(ProcessIrregularity)
REFL_END
