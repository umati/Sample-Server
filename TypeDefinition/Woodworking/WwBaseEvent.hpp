/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2021 (c) Christoph Ruckstetter, Michael Weinig AG
 */

#pragma once
#include "../TypeDefinition.hpp"
#include "../ns0/BaseEvent.hpp"
#include "../ns0/Constants.hpp"
#include "Constants.hpp"
#include "IWwEventMessage.hpp"

namespace woodworking {

struct WwBaseEvent_t : public IWwEventMessage_t, public ns0::BaseEvent_t {};

}  // namespace woodworking

REFL_TYPE(
  woodworking::WwBaseEvent_t,
  Bases<ns0::BaseEvent_t, woodworking::IWwEventMessage_t>(),
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_WWBASEEVENTTYPE)))
REFL_END
