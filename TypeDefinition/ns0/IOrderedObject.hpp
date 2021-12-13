/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2020-2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 */

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
