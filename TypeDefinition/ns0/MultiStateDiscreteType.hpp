/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2022 (c) mindonwarp
 */

#pragma once
#include "../TypeDefinition.hpp"
#include "DiscreteItem.hpp"
#include "Constants.hpp"

namespace ns0 {

struct MultiStateDiscreteType_t : public DiscreteItem_t {
  BindableMemberValue<std::uint32_t> Value;
  BindableMemberValue<std::vector<UmatiServerLib::LocalizedText_t>> EnumStrings;
};

}  // namespace ns0

REFL_TYPE(
  ns0::MultiStateDiscreteType_t,
  Bases<ns0::DiscreteItem_t>(),
  UmatiServerLib::attribute::UaVariableType(UmatiServerLib::constexp::NodeId(constants::Ns0Uri, UA_NS0ID_MULTISTATEDISCRETETYPE)))
REFL_FIELD(Value, UmatiServerLib::attribute::UaVariableTypeValue())
REFL_FIELD(EnumStrings)
REFL_END
