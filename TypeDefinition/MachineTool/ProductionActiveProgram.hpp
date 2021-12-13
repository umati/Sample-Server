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
#include "ProductionProgram.hpp"

namespace machineTool {

struct ProductionActiveProgram_t : public ProductionProgram_t {
  BindableMemberValue<std::string> JobIdentifier;
  BindableMemberValue<UA_NodeId> JobNodeId;
};

}  // namespace machineTool

REFL_TYPE(
  machineTool::ProductionActiveProgram_t,
  Bases<machineTool::ProductionProgram_t>(),
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PRODUCTIONACTIVEPROGRAMTYPE)))
// REFL_FIELD(State) // Override mandatory ///\todo requires fix in nodeset, i=5030 needs to be ProductionActiveProgramType not ProductionProgramType
REFL_FIELD(
  JobIdentifier,
  UmatiServerLib::attribute::MemberInTypeNodeId(
    UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PRODUCTIONACTIVEPROGRAMTYPE_JOBIDENTIFIER)),
  UmatiServerLib::attribute::PlaceholderOptional())
REFL_FIELD(
  JobNodeId,
  UmatiServerLib::attribute::MemberInTypeNodeId(
    UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PRODUCTIONACTIVEPROGRAMTYPE_JOBNODEID)),
  UmatiServerLib::attribute::PlaceholderOptional())
REFL_END
