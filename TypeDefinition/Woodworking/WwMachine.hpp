/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2021 (c) Christoph Ruckstetter, Michael Weinig AG
 */

#pragma once
#include "../TypeDefinition.hpp"
#include "../ns0/BaseObject.hpp"
#include "../ns0/StateVariable.hpp"
#include "Constants.hpp"
#include "IWwState.hpp"
#include "MachineIdentification.hpp"
#include "ManufacturerSpecific.hpp"
#include "WwEventsDispatcher.hpp"

namespace woodworking {

struct WwMachine_t {
  BindableMember<MachineIdentification_t> Identification;
  BindableMember<IWwState_t> State;
  BindableMember<WwEventsDispatcher_t> Events;
  BindableMember<ManufacturerSpecific_t> ManufacturerSpecific;
};
}  // namespace woodworking

REFL_TYPE(
  woodworking::WwMachine_t,
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_WWMACHINETYPE)))
REFL_FIELD(Identification, UmatiServerLib::attribute::UaBrowseName(constants::NsDIUri))
REFL_FIELD(State)
REFL_FIELD(
  Events,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_WWMACHINETYPE_EVENTS)))
REFL_FIELD(
  ManufacturerSpecific,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(
    UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_WWMACHINETYPE_MANUFACTURERSPECIFIC)))
REFL_END
