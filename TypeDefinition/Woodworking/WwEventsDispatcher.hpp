#pragma once
#include "../../UmatiServerLib/BindableMemberPlaceholder.hpp"
#include "../Machinery/IMachineTagNameplate.hpp"
#include "../Machinery/IMachineryItemVendorNameplateType.hpp"
#include "../TypeDefinition.hpp"
#include "../ns0/Constants.hpp"
#include "Constants.hpp"

namespace woodworking {

struct WwEventsDispatcher_t {};

}  // namespace woodworking

REFL_TYPE(
  woodworking::WwEventsDispatcher_t,
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_WWEVENTSDISPATCHERTYPE)))
REFL_END
