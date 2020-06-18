#pragma once
#include "../TypeDefiniton.hpp"
#include "Constants.hpp"
#include "../ns0/BaseEvent.hpp"

namespace machineTool
{

struct NotificationEvent_t : public ns0::BaseEvent_t
{
  std::string Identifier;
};

} // namespace machineTool

REFL_TYPE(machineTool::NotificationEvent_t,
          Bases<ns0::BaseEvent_t>(),
          open62541Cpp::attribute::UaObjectType{
              .NodeId = open62541Cpp::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_NOTIFICATIONEVENTTYPE)})
REFL_FIELD(Identifier)
REFL_END
