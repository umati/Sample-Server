/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2020-2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 */

#pragma once
#include "../../UmatiServerLib/BindableMemberPlaceholder.hpp"
#include "../TypeDefinition.hpp"
#include "Constants.hpp"

namespace ns0 {

///\todo limit T to IOrderedObject
template <typename T>
struct OrderedList_t {
  BindableMemberValue<std::string> NodeVersion;
  BindableMemberPlaceholder<BindableMember, T> OrderedObjects;
};

}  // namespace ns0

REFL_TEMPLATE(
  (typename T), (ns0::OrderedList_t<T>), UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::Ns0Uri, UA_NS0ID_ORDEREDLISTTYPE)))
REFL_FIELD(
  NodeVersion,
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::Ns0Uri, UA_NS0ID_ORDEREDLISTTYPE_NODEVERSION)),
  UmatiServerLib::attribute::PlaceholderOptional())
REFL_FIELD(
  OrderedObjects,
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::Ns0Uri, UA_NS0ID_ORDEREDLISTTYPE_ORDEREDOBJECT_PLACEHOLDER)),
  UmatiServerLib::attribute::PlaceholderOptional())
REFL_END
