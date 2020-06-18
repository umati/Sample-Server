#pragma once
#include "../TypeDefiniton.hpp"
#include "Constants.hpp"

namespace ns0
{

template <typename T>
struct StateVariable_t
{
  BindableMemberValue<open62541Cpp::LocalizedText_t> Value;
  BindableMemberValue<open62541Cpp::LocalizedText_t> EffectiveDisplayName;
  BindableMemberValue<T> Id;
  BindableMemberValue<UA_QualifiedName> Name;
  BindableMemberValue<UA_UInt32> Number;
};

} // namespace ns0

REFL_TEMPLATE((typename T), (ns0::StateVariable_t<T>),
              open62541Cpp::attribute::UaVariableType{
                  .NodeId = open62541Cpp::constexp::NodeId(constants::Ns0Uri, UA_NS0ID_STATEVARIABLETYPE)})
REFL_FIELD(Id)
REFL_FIELD(Value, open62541Cpp::attribute::UaVariableTypeValue())
REFL_FIELD(Number, open62541Cpp::attribute::PlaceholderOptional())
REFL_FIELD(Name, open62541Cpp::attribute::PlaceholderOptional())
REFL_FIELD(EffectiveDisplayName, open62541Cpp::attribute::PlaceholderOptional())
REFL_END
