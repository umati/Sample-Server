#pragma once
#include "../TypeDefiniton.hpp"
#include "Constants.hpp"
#include "../ns0/AnalogUnitRange.hpp"
#include "../../src_generated/types_machinetool_generated.h"

namespace machineTool
{

struct ChannelMonitoring_t
{
  BindableMemberValue<UA_ChannelState> ChannelState;
  BindableMember<ns0::AnalogUnitRange_t<double>> FeedOverride;
};

} // namespace machineTool

REFL_TYPE(machineTool::ChannelMonitoring_t,
          open62541Cpp::attribute::UaObjectType{
              .NodeId = open62541Cpp::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_CHANNELMONITORINGTYPE)})
REFL_FIELD(ChannelState)
REFL_FIELD(FeedOverride)
REFL_END
