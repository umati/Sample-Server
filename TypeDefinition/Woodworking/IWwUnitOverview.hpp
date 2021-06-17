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


struct WwStateMachineOverview_t : public IWwUnitOverview_t {};

} // namespace woodworking

REFL_TYPE(
  woodworking::WwStateMachineOverview_t,
  Bases<woodworking::IWwUnitOverview_t>(),
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_WWMACHINETYPE_STATE_MACHINE_OVERVIEW))
)
REFL_END

REFL_TYPE(
  woodworking::IWwUnitOverview_t,
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_IWWUNITOVERVIEWTYPE))
)
REFL_FIELD(CurrentState)
REFL_FIELD(CurrentMode)
REFL_END
