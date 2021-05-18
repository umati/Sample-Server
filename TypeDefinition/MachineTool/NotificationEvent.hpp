#pragma once
#include "../TypeDefinition.hpp"
#include "../ns0/BaseEvent.hpp"
#include "Constants.hpp"

namespace machineTool {

struct NotificationEvent_t : public ns0::BaseEvent_t {
  std::string Identifier;
};

}  // namespace machineTool

REFL_TYPE(
  machineTool::NotificationEvent_t,
  Bases<ns0::BaseEvent_t>(),
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_NOTIFICATIONEVENTTYPE)))
REFL_FIELD(Identifier)
REFL_END
