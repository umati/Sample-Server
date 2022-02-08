/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2020-2021 (c) Sebastian Friedl, ISW University of Stuttgart (for umati and VDMA e.V.)
 */

#pragma once
#include "../TypeDefinition.hpp"
#include "Constants.hpp"

namespace ns0 {

    template <typename T>
    struct AnalogUnitType_t {
        BindableMemberValue<T> Value;
        BindableMemberValue<UmatiServerLib::EUInformation_t> EngineeringUnits;
    };

}  // namespace ns0

REFL_TEMPLATE(
        (typename T), (ns0::AnalogUnitType_t<T>), UmatiServerLib::attribute::UaVariableType(UmatiServerLib::constexp::NodeId(constants::Ns0Uri, UA_NS0ID_ANALOGUNITTYPE))
        )
        REFL_FIELD(Value, UmatiServerLib::attribute::UaVariableTypeValue())
        REFL_FIELD(EngineeringUnits)
REFL_END
