#pragma once
#include "../TypeDefiniton.hpp"
#include "Constants.hpp"
#include "AnalogItem.hpp"

namespace ns0
{

template <typename T>
struct AnalogUnitRange_t : public AnalogItem_t<T>
{
};

} // namespace ns0

REFL_TEMPLATE((typename T), (ns0::AnalogUnitRange_t<T>),
              Bases<ns0::AnalogItem_t<T>>(),
              open62541Cpp::attribute::UaVariableType{
                  .NodeId = open62541Cpp::constexp::NodeId(constants::Ns0Uri, UA_NS0ID_ANALOGUNITRANGETYPE)})
REFL_FIELD(EngineeringUnits)
REFL_END
