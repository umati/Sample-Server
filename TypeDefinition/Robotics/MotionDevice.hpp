#pragma once
#include "../TypeDefinition.hpp"
#include "Constants.hpp"
#include "../DI/Component.hpp"
#include "../../src_generated/types_robotics_generated.h"

namespace robotics
{

struct MotionDevice_t : public di::Component_t
{
  BindableMemberValue<UA_MotionDeviceCategoryEnumeration> MotionDeviceCategory;
};

} // namespace robotics

REFL_TYPE(robotics::MotionDevice_t,
          Bases<di::Component_t>(),
          UmatiServerLib::attribute::UaObjectType{
              .NodeId = UmatiServerLib::constexp::NodeId(constants::NsRoboticsUri, UA_ROBOTICSID_MOTIONDEVICETYPE)})
REFL_FIELD(Manufacturer)
REFL_FIELD(Model)
REFL_FIELD(MotionDeviceCategory)
REFL_FIELD(ProductCode)
REFL_FIELD(SerialNumber)
REFL_END
