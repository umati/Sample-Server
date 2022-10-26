/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2022 (c) Patrick Moeller, 3Yourmind GmbH
 */

#pragma once
#include "../MachineTool/Equipment.hpp"
#include "../TypeDefinition.hpp"
#include "Constants.hpp"
#include "ConsumableList.hpp"
#include "MaterialList.hpp"

namespace AdditiveManufacturing {

struct EquipmentAM_t : public machineTool::Equipment_t {
  BindableMember<ConsumableList_t> Consumables;
  BindableMember<MaterialList_t> Materials;
};

}  // namespace AdditiveManufacturing

REFL_TYPE(
  AdditiveManufacturing::EquipmentAM_t,
  Bases<machineTool::Equipment_t>(),
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsAMUri, UA_ADDITIVEMANUFACTURINGID_EQUIPMENTAM)))
REFL_FIELD(
  Consumables,
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsAMUri, UA_ADDITIVEMANUFACTURINGID_CONSUMABLES)),
  UmatiServerLib::attribute::PlaceholderOptional())
REFL_FIELD(
  Materials,
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsAMUri, UA_ADDITIVEMANUFACTURINGID_EQUIPMENTAM_MATERIALS)),
  UmatiServerLib::attribute::PlaceholderOptional())
REFL_END