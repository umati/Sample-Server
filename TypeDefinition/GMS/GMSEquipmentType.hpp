/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2020-2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 * Copyright 2020 (c) Tonja Heinemann, ISW University of Stuttgart (for umati and VDW e.V.)
 */

#pragma once
#include "../MachineTool/Equipment.hpp"
#include "../TypeDefinition.hpp"
#include "Constants.hpp"
#include "ToolList.hpp"

namespace GMS {

struct GMSEquipment_t {};

}  // namespace GMS

REFL_TYPE(
  GMS::GMSEquipment_t,
  Bases<machineTool::Equipment_t>(),
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsGMSUri, UA_GMSID_EQUIPMENTTYPE)))
REFL_END
