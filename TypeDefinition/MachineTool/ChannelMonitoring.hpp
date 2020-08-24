#pragma once
#include "../TypeDefinition.hpp"
#include "Constants.hpp"
#include "../ns0/AnalogUnitRange.hpp"
#include "../../src_generated/types_machinetool_generated.h"
#include "ElementMonitoring.hpp"

namespace machineTool
{

struct ChannelMonitoring_t : public ElementMonitoring_t
{
  BindableMemberValue<UA_ChannelState> ChannelState;
  BindableMember<ns0::AnalogUnitRange_t<double>> FeedOverride;
  BindableMember<ns0::AnalogUnitRange_t<double>> RapidOverride;
  BindableMemberValue<UA_ChannelMode> ChannelMode;
};

} // namespace machineTool

REFL_TYPE(machineTool::ChannelMonitoring_t,
          Bases<machineTool::ElementMonitoring_t>(),
          UmatiServerLib::attribute::UaObjectType{
              .NodeId = UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_CHANNELMONITORINGTYPE)})
REFL_FIELD(ChannelState)
REFL_FIELD(FeedOverride)
REFL_FIELD(ChannelMode)
REFL_FIELD(RapidOverride, UmatiServerLib::attribute::MemberInTypeNodeId{
               .NodeId = UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_CHANNELMONITORINGTYPE_RAPIDOVERRIDE)},
           UmatiServerLib::attribute::PlaceholderOptional())
REFL_END
