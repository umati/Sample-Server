/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2020-2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 */

#pragma once
#include "../TypeDefinition.hpp"
#include "Condition.hpp"
#include "Constants.hpp"

namespace ns0 {

class AcknowledgeableCondition_t : public Condition_t {
 public:
  BindableMember<TwoStateVariable_t> EnabledState;
  BindableMember<TwoStateVariable_t> AckedState;
  BindableMember<TwoStateVariable_t> ConfirmedState;
};

}  // namespace ns0

REFL_TYPE(
  ns0::AcknowledgeableCondition_t,
  Bases<ns0::Condition_t>(),
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::Ns0Uri, UA_NS0ID_ACKNOWLEDGEABLECONDITIONTYPE)))
REFL_FIELD(
  EnabledState,
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::Ns0Uri, UA_NS0ID_ACKNOWLEDGEABLECONDITIONTYPE_ENABLEDSTATE)))
REFL_FIELD(
  AckedState,
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::Ns0Uri, UA_NS0ID_ACKNOWLEDGEABLECONDITIONTYPE_ACKEDSTATE)))
REFL_FIELD(
  ConfirmedState,
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::Ns0Uri, UA_NS0ID_ACKNOWLEDGEABLECONDITIONTYPE_CONFIRMEDSTATE)),
  UmatiServerLib::attribute::PlaceholderOptional())
REFL_END
