#pragma once
#include "../TypeDefinition.hpp"
#include "Constants.hpp"

namespace ns0 {

template <typename T>
struct StateVariable_t {
  BindableMemberValue<UmatiServerLib::LocalizedText_t> Value;
  BindableMemberValue<UmatiServerLib::LocalizedText_t> EffectiveDisplayName;
  BindableMemberValue<T> Id;
  BindableMemberValue<UA_QualifiedName> Name;
  BindableMemberValue<UA_UInt32> Number;
};

}  // namespace ns0

REFL_TEMPLATE(
  (typename T),
  (ns0::StateVariable_t<T>),
  UmatiServerLib::attribute::UaVariableType(UmatiServerLib::constexp::NodeId(constants::Ns0Uri, UA_NS0ID_STATEVARIABLETYPE)))
REFL_FIELD(Id)
REFL_FIELD(Value, UmatiServerLib::attribute::UaVariableTypeValue())
REFL_FIELD(Number, UmatiServerLib::attribute::PlaceholderOptional())
REFL_FIELD(Name, UmatiServerLib::attribute::PlaceholderOptional())
REFL_FIELD(EffectiveDisplayName, UmatiServerLib::attribute::PlaceholderOptional())
REFL_END
