#pragma once
#include "../TypeDefinition.hpp"
#include "Constants.hpp"

namespace ns0 {

template <typename T>
struct ConditionVariable_t {
  T Value;
  UmatiServerLib::DateTime_t SourceTimestamp;
};

}  // namespace ns0

REFL_TEMPLATE(
  (typename T),
  (ns0::ConditionVariable_t<T>),
  UmatiServerLib::attribute::UaVariableType(UmatiServerLib::constexp::NodeId(constants::Ns0Uri, UA_NS0ID_CONDITIONVARIABLETYPE)))
REFL_FIELD(Value, UmatiServerLib::attribute::UaVariableTypeValue())
REFL_FIELD(SourceTimestamp)
REFL_END
