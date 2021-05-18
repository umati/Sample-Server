#pragma once
#include "../TypeDefinition.hpp"
#include "AnalogItem.hpp"
#include "Constants.hpp"

namespace ns0 {

template <typename T>
struct AnalogUnitRange_t : public AnalogItem_t<T> {};

}  // namespace ns0

REFL_TEMPLATE(
  (typename T),
  (ns0::AnalogUnitRange_t<T>),
  Bases<ns0::AnalogItem_t<T>>(),
  UmatiServerLib::attribute::UaVariableType(UmatiServerLib::constexp::NodeId(constants::Ns0Uri, UA_NS0ID_ANALOGUNITRANGETYPE)))
REFL_FIELD(EngineeringUnits)
REFL_END
