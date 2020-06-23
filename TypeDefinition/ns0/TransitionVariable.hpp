#pragma once
#include "../TypeDefiniton.hpp"
#include "Constants.hpp"

namespace ns0
{

template<typename T>
struct TransitionVariable_t
{
  BindableMemberValue<UmatiServerLib::LocalizedText_t> Value;
  BindableMemberValue<T> Id;
  BindableMemberValue<std::uint32_t> Number;
  BindableMemberValue<UA_QualifiedName> Name;
  BindableMemberValue<UmatiServerLib::DateTime_t> TransitionTime;
};

}

REFL_TEMPLATE((typename T), (ns0::TransitionVariable_t<T>), UmatiServerLib::attribute::UaVariableType{.NodeId = UmatiServerLib::constexp::NodeId(constants::Ns0Uri, UA_NS0ID_TRANSITIONVARIABLETYPE)})
REFL_FIELD(Id)
REFL_FIELD(Value, UmatiServerLib::attribute::UaVariableTypeValue())
REFL_FIELD(Number, UmatiServerLib::attribute::PlaceholderOptional())
REFL_FIELD(Name, UmatiServerLib::attribute::PlaceholderOptional())
REFL_FIELD(TransitionTime, UmatiServerLib::attribute::PlaceholderOptional())
REFL_END
