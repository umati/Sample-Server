/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2022 (c) Sebastian Friedl, ISW University of Stuttgart (for VDMA e.V.)
 */

#pragma once
#include "Constants.hpp"
#include "ResultDataType.hpp"

namespace machinery_result {
    struct ResultType_t {
        BindableMemberValue<UA_ResultDataType> Value;
    };
}  // namespace machineTool


REFL_TYPE(
machinery_result::ResultType_t,
UmatiServerLib::attribute::UaVariableType(UmatiServerLib::constexp::NodeId(constants::NsMachineryResultUri, UA_MACHINERY_RESULTID_RESULTTYPE))
)
REFL_FIELD(Value, UmatiServerLib::attribute::UaVariableTypeValue())
REFL_END