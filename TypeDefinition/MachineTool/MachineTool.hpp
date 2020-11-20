#pragma once
#include "../TypeDefinition.hpp"
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
          UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_MACHINETOOLTYPE)))
REFL_FIELD(Identification, UmatiServerLib::attribute::UaBrowseName(constants::NsDIUri))
REFL_FIELD(Equipment)
REFL_FIELD(Monitoring)
REFL_FIELD(Production)
REFL_FIELD(Notification)
REFL_END
