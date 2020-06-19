#pragma once
#include "../TypeDefiniton.hpp"
#include "Constants.hpp"

namespace ns0
{

template <typename T>
struct IOrderedObject_t
{
  BindableMemberValue<T> NumberInList;
};

} // namespace ns0

REFL_TEMPLATE((typename T), (ns0::IOrderedObject_t<T>),
              open62541Cpp::attribute::UaObjectType{
                  .NodeId = open62541Cpp::constexp::NodeId(constants::Ns0Uri, UA_NS0ID_IORDEREDOBJECTTYPE)})
REFL_FIELD(NumberInList, open62541Cpp::attribute::UaVariableTypeValue())
REFL_END
