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
#include "WorkingUnitMonitoring.hpp"

namespace machineTool {
struct SpindleMonitoring_t : public WorkingUnitMonitoring_t {
  BindableMemberValue<bool> IsUsedAsAxis;
  BindableMemberValue<bool> IsRotating;
  BindableMember<ns0::AnalogUnitRange_t<double>> Override;
};
}  // namespace machineTool

REFL_TYPE(
  machineTool::SpindleMonitoring_t,
  Bases<machineTool::WorkingUnitMonitoring_t>(),
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_SPINDLEMONITORINGTYPE)))
REFL_FIELD(IsRotating)
REFL_FIELD(
  IsUsedAsAxis,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(
    UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_SPINDLEMONITORINGTYPE_ISUSEDASAXIS)))
REFL_FIELD(
  Override,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_SPINDLEMONITORINGTYPE_OVERRIDE)))
REFL_END
