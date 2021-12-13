/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2020-2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 */

#pragma once
#include "../TypeDefinition.hpp"
#include "AnalogItem.hpp"
#include "Constants.hpp"

namespace ns0 {

template <typename T>
struct AnalogUnitRange_t : public AnalogItem_t<T> {};

}  // namespace ns0

REFL_TEMPLATE(
  (typename T),
  (ns0::AnalogUnitRange_t<T>),
  Bases<ns0::AnalogItem_t<T>>(),
  UmatiServerLib::attribute::UaVariableType(UmatiServerLib::constexp::NodeId(constants::Ns0Uri, UA_NS0ID_ANALOGUNITRANGETYPE)))
REFL_FIELD(EngineeringUnits)
REFL_END
