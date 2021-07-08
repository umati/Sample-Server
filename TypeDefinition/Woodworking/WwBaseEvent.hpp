#pragma once
#include "../TypeDefinition.hpp"
#include "../ns0/BaseEvent.hpp"
#include "../ns0/Constants.hpp"
#include "Constants.hpp"
#include "IWwEventMessage.hpp"

namespace woodworking {

struct WwBaseEvent_t : public IWwEventMessage_t, public ns0::BaseEvent_t {};

}  // namespace woodworking

REFL_TYPE(
  woodworking::WwBaseEvent_t,
  Bases<ns0::BaseEvent_t, woodworking::IWwEventMessage_t>(),
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_WWBASEEVENTTYPE)))
REFL_END
