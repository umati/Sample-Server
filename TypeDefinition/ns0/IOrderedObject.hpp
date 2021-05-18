#pragma once
#include "../TypeDefinition.hpp"
#include "Constants.hpp"

namespace ns0 {

template <typename T>
struct IOrderedObject_t {
  BindableMemberValue<T> NumberInList;
};

}  // namespace ns0

REFL_TEMPLATE(
  (typename T),
  (ns0::IOrderedObject_t<T>),
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::Ns0Uri, UA_NS0ID_IORDEREDOBJECTTYPE)))
REFL_FIELD(NumberInList)
REFL_END
