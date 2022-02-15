/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2022 (c) Sebastian Friedl, ISW University of Stuttgart (for umati and VDMA e.V.)
 */

#pragma once
#include "../MachineTool/Notification.hpp"
#include "../MachineTool/PrognosisList.hpp"
#include "../TypeDefinition.hpp"
#include "../ns0/BaseObject.hpp"
#include "Constants.hpp"
#include "GMSPrognosisList.hpp"

namespace GMS {

struct GMSNotification_t : public machineTool::Notification_t {
  BindableMember<GMSPrognosisList_t> Prognoses;
  BindableMember<ns0::BaseObject_t> Messages;
};

}  // namespace GMS

REFL_TYPE(
  GMS::GMSNotification_t,
  Bases<machineTool::Notification_t>(),
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_NOTIFICATIONTYPE)))
REFL_FIELD(
  Prognoses,
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_NOTIFICATIONTYPE_PROGNOSES)),
  UmatiServerLib::attribute::PlaceholderOptional())
REFL_FIELD(
  Messages,
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_NOTIFICATIONTYPE_MESSAGES)),
  UmatiServerLib::attribute::PlaceholderOptional())
REFL_END