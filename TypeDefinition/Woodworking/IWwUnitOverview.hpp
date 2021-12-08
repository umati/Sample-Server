/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2021 (c) Christoph Ruckstetter, Michael Weinig AG
 */

#pragma once
#include <stdint.h>

#include "../../UmatiServerLib/BindableMemberPlaceholder.hpp"
#include "../../src_generated/types_woodworking_generated.h"
#include "../TypeDefinition.hpp"
#include "../ns0/BaseObject.hpp"
#include "../ns0/Constants.hpp"
#include "Constants.hpp"

namespace woodworking {

struct IWwUnitOverview_t {
  BindableMemberValue<UA_WwUnitStateEnumeration> CurrentState;
  BindableMemberValue<UA_WwUnitModeEnumeration> CurrentMode;
};

}  // namespace woodworking

REFL_TYPE(
  woodworking::IWwUnitOverview_t,
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_IWWUNITOVERVIEWTYPE)))
REFL_FIELD(CurrentState)
REFL_FIELD(CurrentMode)
REFL_END
