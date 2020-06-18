#pragma once
#include "../TypeDefiniton.hpp"
#include "Constants.hpp"

namespace ns0
{

template <typename T>
struct ConditionVariable_t
{
  T Value;
  open62541Cpp::DateTime_t SourceTimestamp;
};

} // namespace ns0

REFL_TEMPLATE((typename T), (ns0::ConditionVariable_t<T>),
              open62541Cpp::attribute::UaVariableType{
                  .NodeId = open62541Cpp::constexp::NodeId(constants::Ns0Uri, UA_NS0ID_CONDITIONVARIABLETYPE)})
REFL_FIELD(Value, open62541Cpp::attribute::UaVariableTypeValue())
REFL_FIELD(SourceTimestamp)
REFL_END
