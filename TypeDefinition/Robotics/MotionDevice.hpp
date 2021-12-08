/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2020-2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 */

#pragma once
#include "../../src_generated/types_robotics_generated.h"
#include "../DI/Component.hpp"
#include "../TypeDefinition.hpp"
#include "Constants.hpp"

namespace robotics {

struct MotionDevice_t : public di::Component_t {
  BindableMemberValue<UA_MotionDeviceCategoryEnumeration> MotionDeviceCategory;
};

}  // namespace robotics

REFL_TYPE(
  robotics::MotionDevice_t,
  Bases<di::Component_t>(),
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsRoboticsUri, UA_ROBOTICSID_MOTIONDEVICETYPE)))
REFL_FIELD(Manufacturer)
REFL_FIELD(Model)
REFL_FIELD(MotionDeviceCategory)
REFL_FIELD(ProductCode)
REFL_FIELD(SerialNumber)
REFL_END
