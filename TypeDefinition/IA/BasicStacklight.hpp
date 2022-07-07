/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2020-2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 * Copyright 2020 (c) Tonja Heinemann, ISW University of Stuttgart (for umati and VDW e.V.)
 */

#pragma once
#include "../TypeDefinition.hpp"
#include "../ns0/OrderedList.hpp"
#include "Constants.hpp"
#include "StackElementLight.hpp"

namespace ia {

struct BasicStacklight_t : public ns0::OrderedList_t<StackElementLight_t> {
  BindableMemberValue<UA_StacklightOperationMode> SignalMode;
};

}  // namespace ia

REFL_TYPE(
  ia::BasicStacklight_t,
  Bases<ns0::OrderedList_t<ia::StackElementLight_t>>(),
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsIAUri, UA_IAID_BASICSTACKLIGHTTYPE)))
REFL_FIELD(SignalMode)
REFL_END
