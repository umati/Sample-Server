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
              UmatiServerLib::attribute::UaObjectType{
                  .NodeId = UmatiServerLib::constexp::NodeId(constants::Ns0Uri, UA_NS0ID_ORDEREDLISTTYPE)})
REFL_FIELD(NodeVersion,
           UmatiServerLib::attribute::MemberInTypeNodeId{
               .NodeId = UmatiServerLib::constexp::NodeId(constants::Ns0Uri, UA_NS0ID_ORDEREDLISTTYPE_NODEVERSION)},
           UmatiServerLib::attribute::PlaceholderOptional())
REFL_FIELD(OrderedObjects,
           UmatiServerLib::attribute::MemberInTypeNodeId{
               .NodeId = UmatiServerLib::constexp::NodeId(constants::Ns0Uri, UA_NS0ID_ORDEREDLISTTYPE_ORDEREDOBJECT_PLACEHOLDER)},
           UmatiServerLib::attribute::PlaceholderOptional())
REFL_END
