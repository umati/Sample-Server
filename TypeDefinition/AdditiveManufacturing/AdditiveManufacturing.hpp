/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2022 (c) Patrick Moeller, 3Yourmind GmbH
 */

#pragma once
#include "../MachineTool/MachineTool.hpp"
#include "../TypeDefinition.hpp"
#include "Constants.hpp"
#include "EquipmentAM.hpp"
#include "MachineIdentificationAM.hpp"

namespace AdditiveManufacturing {
struct AM_t : public machineTool::MachineTool_t {
  BindableMember<EquipmentAMType_t> Equipment;
  BindableMember<MachineIdentificationAMType_t> Identification;
};
}  // namespace AdditiveManufacturing

REFL_TYPE(
  AdditiveManufacturing::AM_t,
  Bases<machineTool::MachineTool_t>(),
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(
    constants::NsGMSUri, UA_ADDITIVEMANUFACTURINGID_ADDITIVEMANUFACTURING)))  // FIXME: Check NodeId() parameters for correctness
REFL_FIELD(Equipment, UmatiServerLib::attribute::UaBrowseName(constants::NsMachineToolUri))
REFL_FIELD(Identification, UmatiServerLib::attribute::UaBrowseName(constants::NsDIUri))
REFL_END
