#pragma once
#include "../TypeDefiniton.hpp"
#include "Constants.hpp"

namespace ns0
{

template<typename T>
struct TransitionVariable_t
{
  BindableMemberValue<open62541Cpp::LocalizedText_t> Value;
  BindableMemberValue<T> Id;
  BindableMemberValue<std::uint32_t> Number;
  BindableMemberValue<UA_QualifiedName> Name;
  BindableMemberValue<open62541Cpp::DateTime_t> TransitionTime;
};

}

REFL_TEMPLATE((typename T), (ns0::TransitionVariable_t<T>), open62541Cpp::attribute::UaVariableType{.NodeId = open62541Cpp::constexp::NodeId(constants::Ns0Uri, UA_NS0ID_TRANSITIONVARIABLETYPE)})
REFL_FIELD(Id)
REFL_FIELD(Value, open62541Cpp::attribute::UaVariableTypeValue())
REFL_FIELD(Number, open62541Cpp::attribute::PlaceholderOptional())
REFL_FIELD(Name, open62541Cpp::attribute::PlaceholderOptional())
REFL_FIELD(TransitionTime, open62541Cpp::attribute::PlaceholderOptional())
REFL_END
