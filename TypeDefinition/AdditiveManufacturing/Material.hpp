/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2022 (c) Patrick Moeller, 3Yourmind GmbH
 */

#pragma once
#include <string>

#include "../../OpcUaTypes/EUInformation.hpp"
#include "../../UmatiServerLib/BindableMemberValue.hpp"
#include "../TypeDefinition.hpp"
#include "../ns0/Constants.hpp"
#include "Constants.hpp"

namespace AdditiveManufacturing {

struct Material_t {
  BindableMemberValue<bool> ReadyForProduction;
  BindableMemberValue<std::string> ExternalIdentifier;
  BindableMemberValue<std::string> Identifier;
  BindableMemberValue<std::string> Function;  // FIXME: Add enum MaterialFunction
  BindableMemberValue<std::string> Manufacturer;
  BindableMemberValue<std::string> Name;
  BindableMemberValue<std::string> RemainingQuantity;  // FIXME: Add correct type
};

}  // namespace AdditiveManufacturing

REFL_TYPE(
  AdditiveManufacturing::Material_t,
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsAMUri, UA_ADDITIVEMANUFACTURINGID_MATERIALTYPE)))
REFL_FIELD(
  ExternalIdentifier,
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsAMUri, UA_ADDITIVEMANUFACTURINGID_MATERIALTYPE_EXTERNALIDENTIFIER)),
  UmatiServerLib::attribute::PlaceholderOptional())
REFL_FIELD(
  Identifier,
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsAMUri, UA_ADDITIVEMANUFACTURINGID_MATERIALTYPE_IDENTIFIER)),
  UmatiServerLib::attribute::PlaceholderOptional())
REFL_FIELD(
  Function,
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsAMUri, UA_ADDITIVEMANUFACTURINGID_MATERIALTYPE_FUNCTION)),
  UmatiServerLib::attribute::PlaceholderOptional())
REFL_FIELD(
  Manufacturer,
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsAMUri, UA_ADDITIVEMANUFACTURINGID_MATERIALTYPE_MANUFACTURER)),
  UmatiServerLib::attribute::PlaceholderOptional())
REFL_FIELD(
  Name,
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsAMUri, UA_ADDITIVEMANUFACTURINGID_MATERIALTYPE_NAME)),
  UmatiServerLib::attribute::PlaceholderOptional())
REFL_FIELD(
  RemainingQuantity,
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsAMUri, UA_ADDITIVEMANUFACTURINGID_MATERIALTYPE_REMAININGQUANTITY)),
  UmatiServerLib::attribute::PlaceholderOptional())
REFL_END
