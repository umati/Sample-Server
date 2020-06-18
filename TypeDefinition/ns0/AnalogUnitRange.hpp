#pragma once
#include "../TypeDefiniton.hpp"
#include "Constants.hpp"

namespace ns0
{

template <typename T>
struct AnalogUnitRange_t
{
  BindableMemberValue<T> Value;
  BindableMemberValue<UA_Range> EURange;
  BindableMemberValue<open62541Cpp::EUInformation_t> EngineeringUnits;
};

} // namespace ns0

REFL_TEMPLATE((typename T), (ns0::AnalogUnitRange_t<T>),
              open62541Cpp::attribute::UaVariableType{
                  .NodeId = open62541Cpp::constexp::NodeId(constants::Ns0Uri, UA_NS0ID_ANALOGUNITRANGETYPE)})
REFL_FIELD(Value, open62541Cpp::attribute::UaVariableTypeValue())
REFL_FIELD(EURange)
REFL_FIELD(EngineeringUnits)
REFL_END
