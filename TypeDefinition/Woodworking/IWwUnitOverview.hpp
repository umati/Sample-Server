#pragma once
#include "../../UmatiServerLib/BindableMemberPlaceholder.hpp"
#include "../TypeDefinition.hpp"
#include "../ns0/Constants.hpp"
#include "../ns0/BaseObject.hpp"
#include "../../src_generated/types_woodworking_generated.h"
#include "Constants.hpp"
#include <stdint.h>

namespace woodworking{

struct IWwUnitOverview_t {
    BindableMemberValue<UA_WwUnitStateEnumeration> CurrentState;
    BindableMemberValue<UA_WwUnitModeEnumeration> CurrentMode;
};

} // namespace woodworking

REFL_TYPE(
  woodworking::IWwUnitOverview_t,
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_IWWUNITOVERVIEWTYPE))
)
REFL_FIELD(CurrentState)
REFL_FIELD(CurrentMode)
REFL_END
