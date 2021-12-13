/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2020-2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 */

#pragma once
#include "../../OpcUaTypes/StatusCode.hpp"
#include "../TypeDefinition.hpp"
#include "BaseEvent.hpp"
#include "ConditionVariable.hpp"
#include "Constants.hpp"
#include "TwoStateVariable.hpp"

namespace ns0 {

struct Condition_t : public BaseEvent_t {
  UA_NodeId BranchId;
  std::string ClientUserId;
  ConditionVariable_t<UmatiServerLib::LocalizedText_t> Comment;
  UA_NodeId ConditionClassId;
  UmatiServerLib::LocalizedText_t ConditionClassName;
  std::string ConditionName;
  UA_NodeId ConditionSubClassId;
  UmatiServerLib::LocalizedText_t ConditionSubClassName;
  TwoStateVariable_t EnabledState;
  ConditionVariable_t<std::uint16_t> LastSeverity;
  ConditionVariable_t<UmatiServerLib::StatusCode_t> Quality;
  bool Retain;
};

}  // namespace ns0

REFL_TYPE(
  ns0::Condition_t,
  Bases<ns0::BaseEvent_t>(),
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::Ns0Uri, UA_NS0ID_CONDITIONTYPE)))
REFL_FIELD(BranchId)
REFL_FIELD(ClientUserId)
REFL_FIELD(Comment)
REFL_FIELD(ConditionClassId)
REFL_FIELD(ConditionClassName)
REFL_FIELD(ConditionName)
REFL_FIELD(ConditionSubClassId, UmatiServerLib::attribute::PlaceholderOptional())
REFL_FIELD(ConditionSubClassName, UmatiServerLib::attribute::PlaceholderOptional())
REFL_FIELD(EnabledState)
REFL_FIELD(LastSeverity)
REFL_FIELD(Quality)
REFL_FIELD(Retain)
REFL_END
