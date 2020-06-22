#pragma once
#include "../TypeDefiniton.hpp"
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
    BindableMemberValue<open62541Cpp::EUInformation_t> EngineeringUnits;
    BindableMemberValue<UA_ToolLifeIndication> Indication;
    BindableMemberValue<bool> IsCountingUp;
    BindableMemberValue<T> LimitValue;
    BindableMemberValue<T> StartValue;
    BindableMemberValue<T> WarningValue;
  };
}

REFL_TEMPLATE((typename T), (machineTool::ToolLife_t<T>),
              open62541Cpp::attribute::UaVariableType{
                  .NodeId = open62541Cpp::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_TOOLLIFETYPE)})
REFL_FIELD(Value, open62541Cpp::attribute::UaVariableTypeValue())
REFL_FIELD(EngineeringUnits)
REFL_FIELD(Indication)
REFL_FIELD(IsCountingUp)
REFL_FIELD(LimitValue, open62541Cpp::attribute::PlaceholderOptional(),
           open62541Cpp::attribute::MemberInTypeNodeId{
               .NodeId = open62541Cpp::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_TOOLLIFETYPE_LIMITVALUE)})
REFL_FIELD(StartValue, open62541Cpp::attribute::PlaceholderOptional(),
           open62541Cpp::attribute::MemberInTypeNodeId{
               .NodeId = open62541Cpp::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_TOOLLIFETYPE_STARTVALUE)})
REFL_FIELD(WarningValue, open62541Cpp::attribute::PlaceholderOptional(),
           open62541Cpp::attribute::MemberInTypeNodeId{
               .NodeId = open62541Cpp::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_TOOLLIFETYPE_WARNINGVALUE)})
REFL_END
