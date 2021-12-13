/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2020-2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 * Copyright 2020 (c) Tonja Heinemann, ISW University of Stuttgart (for umati and VDW e.V.)
 */

#pragma once
#include "../../src_generated/types_ia_generated.h"
#include "../TypeDefinition.hpp"
#include "../ns0/AnalogItem.hpp"
#include "Constants.hpp"
#include "StackElement.hpp"

namespace ia {

struct StackElementLight_t : public StackElement_t {
  BindableMember<ns0::AnalogItem_t<float>> Intensity;
  BindableMemberValue<UA_SignalColor> SignalColor;
  BindableMemberValue<UA_SignalModeLight> StacklightMode;
};

}  // namespace ia

REFL_TYPE(
  ia::StackElementLight_t,
  Bases<ia::StackElement_t>(),
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsIAUri, UA_IAID_STACKELEMENTLIGHTTYPE)))
REFL_FIELD(
  Intensity,
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsIAUri, UA_IAID_STACKELEMENTLIGHTTYPE_INTENSITY)),
  UmatiServerLib::attribute::PlaceholderOptional())
REFL_FIELD(
  SignalColor,
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsIAUri, UA_IAID_STACKELEMENTLIGHTTYPE_SIGNALCOLOR)),
  UmatiServerLib::attribute::PlaceholderOptional())
REFL_FIELD(
  StacklightMode,
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsIAUri, UA_IAID_STACKELEMENTLIGHTTYPE_STACKLIGHTMODE)),
  UmatiServerLib::attribute::PlaceholderOptional())
REFL_END
