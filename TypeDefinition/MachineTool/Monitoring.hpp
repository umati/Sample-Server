#pragma once
#include "../TypeDefinition.hpp"
#include "Constants.hpp"
#include "../../UmatiServerLib/BindableMemberPlaceholder.hpp"
#include "ChannelMonitoring.hpp"
#include "../IA/BasicStacklight.hpp"
#include "EDMGeneratorMonitoring.hpp"
#include "SpindleMonitoring.hpp"
#include "LaserMonitoring.hpp"
#include "MachineOperationMonitoring.hpp"

namespace machineTool
{

  struct Monitoring_t
  {
    BindableMemberPlaceholder<BindableMember, ChannelMonitoring_t> Channels; // TODO embed as MonitoredElement?!
    BindableMemberPlaceholder<BindableMember, std::variant<SpindleMonitoring_t, EDMGeneratorMonitoring_t, LaserMonitoring_t>> MonitoredElement;
    BindableMember<ia::BasicStacklight_t> Stacklight;
    BindableMember<MachineOperationMonitoring_t> MachineTool;
  };

} // namespace machineTool

REFL_TYPE(machineTool::Monitoring_t,
          UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_MONITORINGTYPE)))
REFL_FIELD(MachineTool)
REFL_FIELD(Channels,
           UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_MONITORINGTYPE_MONITOREDELEMENT)),
           UmatiServerLib::attribute::PlaceholderOptional())
REFL_FIELD(MonitoredElement,
           UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_MONITORINGTYPE_MONITOREDELEMENT)),
           UmatiServerLib::attribute::PlaceholderOptional())
REFL_FIELD(Stacklight,
            UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_MONITORINGTYPE_STACKLIGHT)),
           UmatiServerLib::attribute::PlaceholderOptional())
REFL_END
