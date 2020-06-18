#pragma once
#include "../TypeDefiniton.hpp"
#include "Constants.hpp"

namespace ns0
{

///\todo limit T to IOrderedObject
template <typename T>
struct OrderedList_t
{
  BindableMemberValue<std::string> NodeVersion;
  BindableMemberPlaceholder<BindableMember, T> OrderedObjects;
};

} // namespace ns0

REFL_TEMPLATE((typename T), (ns0::OrderedList_t<T>),
              open62541Cpp::attribute::UaObjectType{
                  .NodeId = open62541Cpp::constexp::NodeId(constants::Ns0Uri, UA_NS0ID_ORDEREDLISTTYPE)})
REFL_FIELD(NodeVersion,
           open62541Cpp::attribute::MemberInTypeNodeId{
               .NodeId = open62541Cpp::constexp::NodeId(constants::Ns0Uri, UA_NS0ID_ORDEREDLISTTYPE_NODEVERSION)},
           open62541Cpp::attribute::PlaceholderOptional())
REFL_FIELD(OrderedObjects,
           open62541Cpp::attribute::MemberInTypeNodeId{
               .NodeId = open62541Cpp::constexp::NodeId(constants::Ns0Uri, UA_NS0ID_ORDEREDLISTTYPE_ORDEREDOBJECT_PLACEHOLDER)},
           open62541Cpp::attribute::PlaceholderOptional())
REFL_END
