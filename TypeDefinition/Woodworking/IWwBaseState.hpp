/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2021 (c) Christoph Ruckstetter, Michael Weinig AG
 */

#pragma once
#include "../../UmatiServerLib/BindableMemberPlaceholder.hpp"
#include "../TypeDefinition.hpp"
#include "../ns0/BaseObject.hpp"
#include "../ns0/Constants.hpp"
#include "Constants.hpp"
#include "IWwUnitFlags.hpp"
#include "IWwUnitOverview.hpp"
#include "IWwUnitValues.hpp"

namespace woodworking {

struct IWwBaseState_t {
  BindableMember<IWwUnitOverview_t> Overview;
  BindableMember<IWwUnitFlags_t> Flags;
  BindableMember<IWwUnitValues_t> Values;
};

}  // namespace woodworking

REFL_TYPE(
  woodworking::IWwBaseState_t,
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_IWWBASESTATETYPE)))
REFL_FIELD(Overview)
REFL_FIELD(
  Flags,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_IWWBASESTATETYPE_FLAGS)))
REFL_FIELD(
  Values,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_IWWBASESTATETYPE_VALUES)))

REFL_END
