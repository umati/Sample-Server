/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2020-2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 * Copyright 2020 (c) Tonja Heinemann, ISW University of Stuttgart (for umati and VDW e.V.)
 */

#pragma once
#include "../../src_generated/types_machinetool_generated.h"
#include "../TypeDefinition.hpp"
#include "../ns0/AnalogUnitRange.hpp"
#include "Constants.hpp"
#include "ElementMonitoring.hpp"

namespace machineTool {

struct MachineOperationMonitoring_t {
  BindableMemberValue<bool> IsWarmUp;
  BindableMemberValue<UA_MachineOperationMode> OperationMode;
  BindableMember<ns0::AnalogUnitRange_t<double>> FeedOverride;
  BindableMemberValue<std::uint32_t> PowerOnDuration;
};

}  // namespace machineTool

REFL_TYPE(
  machineTool::MachineOperationMonitoring_t,
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_MACHINEOPERATIONMONITORINGTYPE)))
REFL_FIELD(OperationMode)
REFL_FIELD(
  FeedOverride,
  UmatiServerLib::attribute::MemberInTypeNodeId(
    UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_MACHINEOPERATIONMONITORINGTYPE_FEEDOVERRIDE)),
  UmatiServerLib::attribute::PlaceholderOptional())
REFL_FIELD(
  PowerOnDuration,
  UmatiServerLib::attribute::MemberInTypeNodeId(
    UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_MACHINEOPERATIONMONITORINGTYPE_POWERONDURATION)),
  UmatiServerLib::attribute::PlaceholderOptional())
REFL_FIELD(
  IsWarmUp,
  UmatiServerLib::attribute::MemberInTypeNodeId(
    UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_MACHINEOPERATIONMONITORINGTYPE_ISWARMUP)),
  UmatiServerLib::attribute::PlaceholderOptional())
REFL_END
