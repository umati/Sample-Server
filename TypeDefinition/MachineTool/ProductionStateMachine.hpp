/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2020-2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 * Copyright 2020 (c) Tonja Heinemann, ISW University of Stuttgart (for umati and VDW e.V.)
 */

#pragma once
#include "../TypeDefinition.hpp"
#include "../ns0/FiniteStateMachine.hpp"
#include "../ns0/FiniteStateVariable.hpp"
#include "../ns0/FiniteTransitionVariable.hpp"
#include "Constants.hpp"

namespace machineTool {

/// Virtual definition, not a UA type
struct Production_FiniteStateVariable_t : public ns0::FiniteStateVariable_t {};

/// Virtual definition, not a UA type
struct Production_FiniteTransitionVariable_t : public ns0::FiniteTransitionVariable_t {};

struct ProductionStateMachine_t : public ns0::FiniteStateMachine_t {
  BindableMember<Production_FiniteStateVariable_t> CurrentState;         // Override
  BindableMember<Production_FiniteTransitionVariable_t> LastTransition;  // Override
};

}  // namespace machineTool

REFL_TYPE(
  machineTool::Production_FiniteStateVariable_t,
  Bases<ns0::FiniteStateVariable_t>(),
  UmatiServerLib::attribute::UaVariableType(UmatiServerLib::constexp::NodeId(constants::Ns0Uri, UA_NS0ID_FINITESTATEVARIABLETYPE)))
REFL_FIELD(Number)  // Override mandatory
REFL_END

REFL_TYPE(
  machineTool::Production_FiniteTransitionVariable_t,
  Bases<ns0::FiniteTransitionVariable_t>(),
  UmatiServerLib::attribute::UaVariableType(UmatiServerLib::constexp::NodeId(constants::Ns0Uri, UA_NS0ID_FINITETRANSITIONVARIABLETYPE)))
REFL_FIELD(Number)  // Override mandatory
REFL_END

REFL_TYPE(
  machineTool::ProductionStateMachine_t,
  Bases<ns0::FiniteStateMachine_t>(),
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PRODUCTIONSTATEMACHINETYPE)))
REFL_FIELD(CurrentState, UmatiServerLib::attribute::UaBrowseName(constants::Ns0Uri))
REFL_FIELD(LastTransition, UmatiServerLib::attribute::UaBrowseName(constants::Ns0Uri), UmatiServerLib::attribute::PlaceholderOptional())
REFL_END
