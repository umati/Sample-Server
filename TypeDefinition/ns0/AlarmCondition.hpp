/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2020-2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 */

#pragma once
#include "../TypeDefinition.hpp"
#include "AcknowledgeableCondition.hpp"
#include "Constants.hpp"

namespace ns0 {

struct AlarmCondition_t : public AcknowledgeableCondition_t {
  ///\todo declare fields
};

}  // namespace ns0

REFL_TYPE(
  ns0::AlarmCondition_t,
  Bases<ns0::AcknowledgeableCondition_t>(),
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::Ns0Uri, UA_NS0ID_ALARMCONDITIONTYPE)))
REFL_END
