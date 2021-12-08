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

struct ProductionJobEndPrognosis_t : public Prognosis_t {
  BindableMemberValue<UA_NodeId> JobNodeId;
  BindableMemberValue<std::string> SourceIdentifier;
};

}  // namespace machineTool

REFL_TYPE(
  machineTool::ProductionJobEndPrognosis_t,
  Bases<machineTool::Prognosis_t>(),
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PRODUCTIONJOBENDPROGNOSISTYPE)))
REFL_FIELD(
  JobNodeId,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(
    UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PRODUCTIONJOBENDPROGNOSISTYPE_JOBNODEID)))
REFL_FIELD(SourceIdentifier)
REFL_END
