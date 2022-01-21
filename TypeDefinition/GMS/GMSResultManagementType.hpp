/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2022 (c) Sebastian Friedl, ISW University of Stuttgart (for VDMA e.V.)
 */

#pragma once
#include "../TypeDefinition.hpp"
#include "Constants.hpp"
#include "../MachineryResult/ResultManagementType.hpp"

namespace GMS {
    struct GMSResultManagementType_t : public machinery_result::ResultManagementType_t {

    };
}  // namespace GMS

REFL_TYPE(
        GMS::GMSResultManagementType_t,
        Bases<machinery_result::ResultManagementType_t>(),
        UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsGMSUri, UA_GMSID_GMSRESULTMANAGEMENTTYPE)))
REFL_END
