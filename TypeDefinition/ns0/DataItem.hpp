/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2022 (c) mindonwarp
 */

#pragma once
#include "../TypeDefinition.hpp"
#include "Constants.hpp"

#include <string>

namespace ns0 {

struct DataItem_t {
  BindableMemberValue<std::string> Definition;
  BindableMemberValue<double> ValuePrecision;
};

}  // namespace ns0

REFL_TYPE(ns0::DataItem_t, UmatiServerLib::attribute::UaVariableType(UmatiServerLib::constexp::NodeId(constants::Ns0Uri, UA_NS0ID_DATAITEMTYPE)))
REFL_FIELD(Definition, UmatiServerLib::attribute::PlaceholderOptional())
REFL_FIELD(ValuePrecision, UmatiServerLib::attribute::PlaceholderOptional())
REFL_END
