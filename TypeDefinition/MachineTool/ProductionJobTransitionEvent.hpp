/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2020-2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 * Copyright 2020 (c) Tonja Heinemann, ISW University of Stuttgart (for umati and VDW e.V.)
 */

#pragma once
#include "../MachineTool/ProductionJob.hpp"
#include "../TypeDefinition.hpp"
#include "../ns0/BaseEvent.hpp"
#include "../ns0/TransitionEvent.hpp"
#include "Constants.hpp"

namespace machineTool {

struct ProductionJobTransitionEvent_t : public ns0::TransitionEvent_t {
  BindableMemberValue<std::string> Identifier;
  //    std::string CustomerOrderIdentifier; //optional
  //    std::string OrderIdentifier; //optional
  BindableMemberValue<std::uint32_t> RunsCompleted;
  BindableMember<ProductionJob_RunsPlanned_t> RunsPlanned;
};

}  // namespace machineTool

REFL_TYPE(
  machineTool::ProductionJobTransitionEvent_t,
  Bases<ns0::TransitionEvent_t>(),
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PRODUCTIONJOBTRANSITIONEVENTTYPE)))
REFL_FIELD(Identifier)
REFL_FIELD(RunsCompleted)
REFL_FIELD(RunsPlanned)
REFL_END
