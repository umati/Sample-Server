/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2020-2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 */

#pragma once
#include "../../UmatiServerLib/BindableMemberPlaceholder.hpp"
#include "../IA/BasicStacklight.hpp"
#include "../TypeDefinition.hpp"
#include "ChannelMonitoring.hpp"
#include "Constants.hpp"
#include "EDMGeneratorMonitoring.hpp"
#include "LaserMonitoring.hpp"
#include "MachineOperationMonitoring.hpp"
#include "SpindleMonitoring.hpp"

namespace machineTool {

struct Monitoring_t {
  BindableMemberPlaceholder<BindableMember, ChannelMonitoring_t> Channels;  // TODO embed as MonitoredElement?!
  BindableMemberPlaceholder<BindableMember, std::variant<SpindleMonitoring_t, EDMGeneratorMonitoring_t, LaserMonitoring_t>> MonitoredElement;
  BindableMember<ia::BasicStacklight_t> Stacklight;
  BindableMember<MachineOperationMonitoring_t> MachineTool;
};

}  // namespace machineTool

REFL_TYPE(
  machineTool::Monitoring_t,
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_MONITORINGTYPE)))
REFL_FIELD(MachineTool)
REFL_FIELD(
  Channels,
  UmatiServerLib::attribute::MemberInTypeNodeId(
    UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_MONITORINGTYPE_MONITOREDELEMENT)),
  UmatiServerLib::attribute::PlaceholderOptional())
REFL_FIELD(
  MonitoredElement,
  UmatiServerLib::attribute::MemberInTypeNodeId(
    UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_MONITORINGTYPE_MONITOREDELEMENT)),
  UmatiServerLib::attribute::PlaceholderOptional())
REFL_FIELD(
  Stacklight,
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_MONITORINGTYPE_STACKLIGHT)),
  UmatiServerLib::attribute::PlaceholderOptional())
REFL_END
