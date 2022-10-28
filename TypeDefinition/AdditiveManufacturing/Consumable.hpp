/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2022 (c) Patrick Moeller, 3Yourmind GmbH
 */

#pragma once
#include <string>

#include "../TypeDefinition.hpp"
#include "../ns0/AnalogUnitType.hpp"
#include "../ns0/Constants.hpp"
#include "Constants.hpp"

namespace AdditiveManufacturing {

struct Consumable_t {
  BindableMemberValue<std::string> ExternalIdentifier;
  BindableMemberValue<std::string> Identifier;
  BindableMemberValue<std::string> Manufacturer;
  BindableMemberValue<std::string> Name;
  BindableMember<ns0::AnalogUnitType_t<double>> RemainingQuantity;
};

}  // namespace AdditiveManufacturing

REFL_TYPE(
  AdditiveManufacturing::Consumable_t,
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsAMUri, UA_ADDITIVEMANUFACTURINGID_CONSUMABLETYPE)))
REFL_FIELD(
  ExternalIdentifier,
  UmatiServerLib::attribute::MemberInTypeNodeId(
    UmatiServerLib::constexp::NodeId(constants::NsAMUri, UA_ADDITIVEMANUFACTURINGID_CONSUMABLETYPE_EXTERNALIDENTIFIER)),  // FIXME: Wrong LIST type
  UmatiServerLib::attribute::PlaceholderOptional())
REFL_FIELD(
  Identifier,
  UmatiServerLib::attribute::MemberInTypeNodeId(
    UmatiServerLib::constexp::NodeId(constants::NsAMUri, UA_ADDITIVEMANUFACTURINGID_CONSUMABLETYPE_IDENTIFIER)),  // FIXME: Wrong LIST type
  UmatiServerLib::attribute::PlaceholderOptional())
REFL_FIELD(
  Manufacturer,
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsAMUri, UA_ADDITIVEMANUFACTURINGID_CONSUMABLETYPE_MANUFACTURER)),
  UmatiServerLib::attribute::PlaceholderOptional())
REFL_FIELD(
  Name,
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsAMUri, UA_ADDITIVEMANUFACTURINGID_CONSUMABLETYPE_NAME)),
  UmatiServerLib::attribute::PlaceholderOptional())
REFL_FIELD(
  RemainingQuantity,
  UmatiServerLib::attribute::MemberInTypeNodeId(
    UmatiServerLib::constexp::NodeId(constants::NsAMUri, UA_ADDITIVEMANUFACTURINGID_CONSUMABLETYPE_REMAININGQUANTITY)),
  UmatiServerLib::attribute::PlaceholderOptional())
REFL_END
