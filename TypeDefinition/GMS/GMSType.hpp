/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2022 (c) Sebastian Friedl, ISW University of Stuttgart (for VDMA e.V.)
 */

#pragma once
#include "../MachineTool/MachineTool.hpp"
#include "../MachineTool/Monitoring.hpp"
#include "../MachineTool/Production.hpp"
#include "../TypeDefinition.hpp"

#include "Constants.hpp"
#include "GMSNotification.hpp"
#include "GMSEquipmentType.hpp"
#include "GMSIdentificationType.hpp"
#include "GMSResultManagementType.hpp"

namespace GMS {
struct GMS_t : public machineTool::MachineTool_t {
  BindableMember<GMSEquipment_t> Equipment;
  BindableMember<GMSNotification_t> Notification;
  BindableMember<GMSIdentification_t> Identification;
  BindableMember<GMSResultManagementType_t> ResultManagement;
};
}  // namespace GMS

REFL_TYPE(
  GMS::GMS_t,
  Bases<machineTool::MachineTool_t>(),
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsGMSUri, UA_GMSID_GMSTYPE)))
REFL_FIELD(ResultManagement)
REFL_FIELD(Equipment, UmatiServerLib::attribute::UaBrowseName(constants::NsMachineToolUri))
REFL_FIELD(Notification, UmatiServerLib::attribute::UaBrowseName(constants::NsMachineToolUri))
REFL_FIELD(Identification, UmatiServerLib::attribute::UaBrowseName(constants::NsDIUri))
REFL_END
