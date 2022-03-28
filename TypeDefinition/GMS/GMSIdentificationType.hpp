/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2022 (c) Sebastian Friedl, ISW University of Stuttgart (for VDMA e.V.)
 */

#pragma once
#include "../../UmatiServerLib/BindableMemberPlaceholder.hpp"
#include "../MachineTool/MachineToolIdentification.hpp"
#include "../Machinery/IMachineTagNameplate.hpp"
#include "../Machinery/IMachineryItemVendorNameplateType.hpp"
#include "../TypeDefinition.hpp"
#include "../ns0/Constants.hpp"
#include "Constants.hpp"

namespace GMS {

struct GMSIdentification_t : public machineTool::MachineToolIdentification_t {};

}  // namespace GMS

REFL_TYPE(
  GMS::GMSIdentification_t,
  Bases<machineTool::MachineToolIdentification_t>(),
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsGMSUri, UA_GMSID_GMSIDENTIFICATIONTYPE)))
/* REFL_FIELD(
         SoftwareIdentification,
         UmatiServerLib::attribute::PlaceholderOptional(),
         UmatiServerLib::attribute::MemberInTypeNodeId(
                 UmatiServerLib::constexp::NodeId(constants::NsGMSUri, UA_GMSID_GMSIDENTIFICATIONTYPE_SOFTWAREIDENTIFICATION)))*/
REFL_END
