/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2020-2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 * Copyright 2020 (c) Tonja Heinemann, ISW University of Stuttgart (for umati and VDW e.V.)
 */

#pragma once
#include "../TypeDefinition.hpp"
#include "../ns0/BaseObject.hpp"
#include "Constants.hpp"
#include "PrognosisList.hpp"

namespace machineTool {

struct Notification_t {
  BindableMember<PrognosisList_t> Prognoses;
  BindableMember<ns0::BaseObject_t> Messages;
};

}  // namespace machineTool

REFL_TYPE(
  machineTool::Notification_t,
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PROGNOSISLISTTYPE)))
REFL_FIELD(
  Prognoses,
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_NOTIFICATIONTYPE_PROGNOSES)),
  UmatiServerLib::attribute::PlaceholderOptional())
REFL_FIELD(
  Messages,
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_NOTIFICATIONTYPE_MESSAGES)),
  UmatiServerLib::attribute::PlaceholderOptional())
REFL_END
