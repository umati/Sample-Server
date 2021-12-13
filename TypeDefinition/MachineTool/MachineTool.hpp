/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2020-2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 */

#pragma once
#include "../TypeDefinition.hpp"
#include "Constants.hpp"
#include "Equipment.hpp"
#include "MachineToolIdentification.hpp"
#include "Monitoring.hpp"
#include "Notification.hpp"
#include "Production.hpp"

namespace machineTool {
struct MachineTool_t {
  BindableMember<Equipment_t> Equipment;
  BindableMember<MachineToolIdentification_t> Identification;
  BindableMember<Monitoring_t> Monitoring;
  BindableMember<Production_t> Production;
  BindableMember<Notification_t> Notification;
};
}  // namespace machineTool

REFL_TYPE(
  machineTool::MachineTool_t,
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_MACHINETOOLTYPE)))
REFL_FIELD(Identification, UmatiServerLib::attribute::UaBrowseName(constants::NsDIUri))
REFL_FIELD(Equipment)
REFL_FIELD(Monitoring)
REFL_FIELD(Production)
REFL_FIELD(Notification)
REFL_END
