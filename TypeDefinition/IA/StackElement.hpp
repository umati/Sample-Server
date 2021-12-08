/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2020-2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 * Copyright 2020 (c) Tonja Heinemann, ISW University of Stuttgart (for umati and VDW e.V.)
 */

#pragma once
#include <cstdint>

#include "../TypeDefinition.hpp"
#include "../ns0/IOrderedObject.hpp"
#include "Constants.hpp"

namespace ia {

struct StackElement_t : public ns0::IOrderedObject_t<std::uint16_t> {
  BindableMemberValue<bool> IsPartOfBase;
  BindableMemberValue<bool> SignalOn;
};

}  // namespace ia

REFL_TYPE(
  ia::StackElement_t,
  Bases<ns0::IOrderedObject_t<std::uint16_t>>(),
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsIAUri, UA_IAID_STACKELEMENTTYPE)))

REFL_FIELD(
  IsPartOfBase,
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsIAUri, UA_IAID_STACKELEMENTTYPE_ISPARTOFBASE)),
  UmatiServerLib::attribute::PlaceholderOptional())
REFL_FIELD(
  SignalOn,
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsIAUri, UA_IAID_STACKELEMENTTYPE_SIGNALON)),
  UmatiServerLib::attribute::PlaceholderOptional())
REFL_END
