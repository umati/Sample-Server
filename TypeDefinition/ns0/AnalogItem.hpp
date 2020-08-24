#pragma once
#include "../TypeDefinition.hpp"
#include "Constants.hpp"

namespace ns0
{

template <typename T>
struct AnalogItem_t
{
  BindableMemberValue<T> Value;
  BindableMemberValue<UA_Range> EURange;
  BindableMemberValue<UmatiServerLib::EUInformation_t> EngineeringUnits;
};

} // namespace ns0

REFL_TEMPLATE((typename T), (ns0::AnalogItem_t<T>),
              UmatiServerLib::attribute::UaVariableType{
                  .NodeId = UmatiServerLib::constexp::NodeId(constants::Ns0Uri, UA_NS0ID_ANALOGITEMTYPE)})
REFL_FIELD(Value, UmatiServerLib::attribute::UaVariableTypeValue())
REFL_FIELD(EURange)
REFL_FIELD(EngineeringUnits, UmatiServerLib::attribute::PlaceholderOptional())
REFL_END
