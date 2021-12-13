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

struct ChannelMonitoring_t : public ElementMonitoring_t {
  BindableMemberValue<UA_ChannelState> ChannelState;
  BindableMember<ns0::AnalogUnitRange_t<double>> FeedOverride;
  BindableMember<ns0::AnalogUnitRange_t<double>> RapidOverride;
  BindableMemberValue<UA_ChannelMode> ChannelMode;
};

}  // namespace machineTool

REFL_TYPE(
  machineTool::ChannelMonitoring_t,
  Bases<machineTool::ElementMonitoring_t>(),
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_CHANNELMONITORINGTYPE)))
REFL_FIELD(ChannelState)
REFL_FIELD(FeedOverride)
REFL_FIELD(ChannelMode)
REFL_FIELD(
  RapidOverride,
  UmatiServerLib::attribute::MemberInTypeNodeId(
    UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_CHANNELMONITORINGTYPE_RAPIDOVERRIDE)),
  UmatiServerLib::attribute::PlaceholderOptional())
REFL_END
