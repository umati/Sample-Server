/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2020-2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 * Copyright 2020 (c) Tonja Heinemann, ISW University of Stuttgart (for umati and VDW e.V.)
 */

#pragma once
#include "../TypeDefinition.hpp"
#include "../ns0/IOrderedObject.hpp"
#include "Constants.hpp"
#include "ProductionStateMachine.hpp"

namespace machineTool {

struct ProductionProgram_t : public ns0::IOrderedObject_t<std::uint16_t> {
  BindableMemberValue<std::string> Name;
  BindableMember<ProductionStateMachine_t> State;
};

}  // namespace machineTool

REFL_TYPE(
  machineTool::ProductionProgram_t,
  Bases<ns0::IOrderedObject_t<std::uint16_t>>(),
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PRODUCTIONPROGRAMTYPE)))
REFL_FIELD(Name)
REFL_FIELD(
  State,
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PRODUCTIONPROGRAMTYPE_STATE)),
  UmatiServerLib::attribute::PlaceholderOptional())
REFL_END
