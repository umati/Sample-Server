/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2022 (c) Sebastian Friedl, ISW University of Stuttgart (for VDMA e.V.)
 */

#pragma once
#include "../MachineTool/Equipment.hpp"
#include "../MachineTool/MachineTool.hpp"
#include "../MachineTool/MachineToolIdentification.hpp"
#include "../MachineTool/Monitoring.hpp"
#include "../MachineTool/Notification.hpp"
#include "../MachineTool/Production.hpp"
#include "../TypeDefinition.hpp"
#include "Constants.hpp"
#include "GMSNotification.hpp"
#include "GMSResultManagementType.hpp"

namespace GMS {
struct GMS_t : public machineTool::MachineTool_t {
  BindableMember<GMSResultManagementType_t> ResultManagement;
  BindableMember<GMSNotification_t> Notification;
};
}  // namespace GMS

REFL_TYPE(
  GMS::GMS_t,
  Bases<machineTool::MachineTool_t>(),
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsGMSUri, UA_GMSID_GMSTYPE)))
REFL_FIELD(ResultManagement)
REFL_FIELD(Notification, UmatiServerLib::attribute::UaBrowseName(constants::NsMachineToolUri))
REFL_END
