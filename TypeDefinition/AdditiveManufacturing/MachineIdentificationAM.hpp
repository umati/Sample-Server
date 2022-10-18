/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2022 (c) Patrick Moeller, 3Yourmind GmbH
 * Copyright 2022 (c) Dennis Hezel, 3Yourmind GmbH
 */

#pragma once
#include <string>

#include "../../UmatiServerLib/BindableMemberValue.hpp"
#include "../MachineTool/MachineToolIdentification.hpp"
#include "Constants.hpp"

namespace AdditiveManufacturing {

struct MachineIdentificationAM_t : public machineTool::MachineToolIdentification_t {
  BindableMemberValue<std::string> AMTechnologyIdentifier;
};

}  // namespace AdditiveManufacturing

REFL_TYPE(
  AdditiveManufacturing::MachineIdentificationAM_t,
  Bases<machineTool::MachineToolIdentification_t>(),
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsAMUri, UA_ADDITIVEMANUFACTURINGID_MACHINEIDENTIFICATIONAMTYPE)))
REFL_FIELD(
  AMTechnologyIdentifier,
  UmatiServerLib::attribute::MemberInTypeNodeId(
    UmatiServerLib::constexp::NodeId(constants::NsAMUri, UA_ADDITIVEMANUFACTURINGID_MACHINEIDENTIFICATIONAMTYPE_AMTECHNOLOGYIDENTIFIER)),
  UmatiServerLib::attribute::PlaceholderOptional())
REFL_END
