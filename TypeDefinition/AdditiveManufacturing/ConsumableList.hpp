/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2022 (c) Patrick Moeller, 3Yourmind GmbH
 */

#pragma once
#include "../../UmatiServerLib/BindableMember.hpp"
#include "../../UmatiServerLib/BindableMemberPlaceholder.hpp"
#include "../TypeDefinition.hpp"
#include "../ns0/Constants.hpp"
#include "Constants.hpp"
#include "Consumable.hpp"

namespace AdditiveManufacturing {

struct ConsumableList_t {
  BindableMemberPlaceholder<BindableMember, Consumable_t> Consumable;
};

}  // namespace AdditiveManufacturing

REFL_TYPE(
  AdditiveManufacturing::ConsumableList_t,
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::Ns0Uri, UA_NS0ID_BASEOBJECTTYPE)))
REFL_FIELD(
  Consumable,
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsAMUri, UA_ADDITIVEMANUFACTURINGID_CONSUMABLELISTTYPE_CONSUMABLE)),
  UmatiServerLib::attribute::PlaceholderOptional())
REFL_END
