#pragma once
#include "../TypeDefinition.hpp"
#include "Constants.hpp"
#include "BaseTool.hpp"
#include "../../src_generated/types_machinetool_generated.h"
#include "../../OpcUaTypes/EUInformation.hpp"

namespace machineTool
{
  template<typename T>
  struct ToolLife_t
  {
    BindableMemberValue<T> Value;
    BindableMemberValue<UmatiServerLib::EUInformation_t> EngineeringUnits;
    BindableMemberValue<UA_ToolLifeIndication> Indication;
    BindableMemberValue<bool> IsCountingUp;
    BindableMemberValue<T> LimitValue;
    BindableMemberValue<T> StartValue;
    BindableMemberValue<T> WarningValue;
  };
}

REFL_TEMPLATE((typename T), (machineTool::ToolLife_t<T>),
              UmatiServerLib::attribute::UaVariableType{
                  .NodeId = UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_TOOLLIFETYPE)})
REFL_FIELD(Value, UmatiServerLib::attribute::UaVariableTypeValue())
REFL_FIELD(EngineeringUnits)
REFL_FIELD(Indication)
REFL_FIELD(IsCountingUp)
REFL_FIELD(LimitValue, UmatiServerLib::attribute::PlaceholderOptional(),
           UmatiServerLib::attribute::MemberInTypeNodeId{
               .NodeId = UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_TOOLLIFETYPE_LIMITVALUE)})
REFL_FIELD(StartValue, UmatiServerLib::attribute::PlaceholderOptional(),
           UmatiServerLib::attribute::MemberInTypeNodeId{
               .NodeId = UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_TOOLLIFETYPE_STARTVALUE)})
REFL_FIELD(WarningValue, UmatiServerLib::attribute::PlaceholderOptional(),
           UmatiServerLib::attribute::MemberInTypeNodeId{
               .NodeId = UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_TOOLLIFETYPE_WARNINGVALUE)})
REFL_END
