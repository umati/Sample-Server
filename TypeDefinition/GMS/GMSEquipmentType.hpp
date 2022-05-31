/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2020-2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 * Copyright 2022 (c) Sebastian Friedl, ISW University of Stuttgart (for umati and VDMA e.V.)
 */

#pragma once
#include "../MachineTool/Equipment.hpp"
#include "../MachineTool/ToolList.hpp"
#include "../TypeDefinition.hpp"
#include "Constants.hpp"
#include "GMSToolList.hpp"

namespace GMS {

struct GMSEquipment_t : public machineTool::Equipment_t {
  BindableMember<GMSToolList_t> Tools;
};

}  // namespace GMS

REFL_TYPE(
  GMS::GMSEquipment_t,
  Bases<machineTool::Equipment_t>(),
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsGMSUri, UA_GMSID_GMSEQUIPMENTTYPE)))
REFL_FIELD(
  Tools,
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsGMSUri, UA_GMSID_GMSEQUIPMENTTYPE_TOOLS)),
  UmatiServerLib::attribute::PlaceholderOptional())
REFL_END
