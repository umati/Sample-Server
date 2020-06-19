#pragma once
#include "../TypeDefiniton.hpp"
#include "Constants.hpp"
#include "MachineToolIdentification.hpp"
#include "Monitoring.hpp"
#include "Production.hpp"
#include "Notification.hpp"
#include "Equipment.hpp"

namespace machineTool
{
struct MachineTool_t
{
  BindableMember<Equipment_t> Equipment;
  BindableMember<MachineToolIdentification_t> Identification;
  BindableMember<Monitoring_t> Monitoring;
  BindableMember<Production_t> Production;
  BindableMember<Notification_t> Notification;
};
} // namespace machineTool

REFL_TYPE(machineTool::MachineTool_t,
          open62541Cpp::attribute::UaObjectType{
              .NodeId = open62541Cpp::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_MACHINETOOLTYPE)})
REFL_FIELD(Identification, open62541Cpp::attribute::UaBrowseName{.NsURI = constants::NsDIUri})
REFL_FIELD(Equipment)
REFL_FIELD(Monitoring)
REFL_FIELD(Production)
REFL_FIELD(Notification)
REFL_END
