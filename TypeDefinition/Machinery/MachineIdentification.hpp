/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2020-2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 */

#pragma once
#include "../TypeDefinition.hpp"
#include "Constants.hpp"
#include "IMachineTagNameplate.hpp"

namespace machinery {

struct MachineIdentification_t : public IMachineryItemVendorNameplateType_t {};

}  // namespace machinery

REFL_TYPE(
  machinery::MachineIdentification_t,
  Bases<di::IVendorNameplate_t>(),
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsMachineryUri, UA_MACHINERYID_IMACHINEVENDORNAMEPLATETYPE)))

REFL_END
