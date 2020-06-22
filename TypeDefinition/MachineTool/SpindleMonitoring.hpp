#pragma once
#include "../TypeDefiniton.hpp"
#include "Constants.hpp"
#include "WorkingUnitMonitoring.hpp"
#include "../../src_generated/types_machinetool_generated.h"
#include "../ns0/AnalogUnitRange.hpp"

namespace machineTool
{
  struct SpindleMonitoring_t : public WorkingUnitMonitoring_t
  {
    BindableMemberValue<bool> IsUsedAsAxis;
    BindableMemberValue<bool> IsRotating;
    BindableMember<ns0::AnalogUnitRange_t<double>> Override;
  };
} // namespace machineTool

REFL_TYPE(machineTool::SpindleMonitoring_t,
          Bases<machineTool::WorkingUnitMonitoring_t>(),
          open62541Cpp::attribute::UaObjectType{
              .NodeId = open62541Cpp::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_SPINDLEMONITORINGTYPE)})
REFL_FIELD(IsRotating)
REFL_FIELD(IsUsedAsAxis, open62541Cpp::attribute::PlaceholderOptional(),
          open62541Cpp::attribute::MemberInTypeNodeId{
              .NodeId = open62541Cpp::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_SPINDLEMONITORINGTYPE_ISUSEDASAXIS)})
REFL_FIELD(Override, open62541Cpp::attribute::PlaceholderOptional(),
          open62541Cpp::attribute::MemberInTypeNodeId{
              .NodeId = open62541Cpp::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_SPINDLEMONITORINGTYPE_OVERRIDE)})
REFL_END
