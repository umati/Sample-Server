/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2020-2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 * Copyright 2020 (c) Tonja Heinemann, ISW University of Stuttgart (for umati and VDW e.V.)
 */

#pragma once
#include "../TypeDefinition.hpp"
#include "../ns0/BaseEvent.hpp"
#include "Constants.hpp"

namespace machineTool {

struct NotificationEvent_t : public ns0::BaseEvent_t {
  std::string Identifier;
};

}  // namespace machineTool

REFL_TYPE(
  machineTool::NotificationEvent_t,
  Bases<ns0::BaseEvent_t>(),
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_NOTIFICATIONEVENTTYPE)))
REFL_FIELD(Identifier)
REFL_END
