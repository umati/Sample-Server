/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2022 (c) Sebastian Friedl, ISW University of Stuttgart (for VDMA e.V.)
 */

#pragma once
#include "../TypeDefinition.hpp"
#include "../ns0/AnalogUnitType.hpp"
#include "Constants.hpp"

namespace GMS {
struct CorrectionType_t {
  BindableMemberValue<std::string> CharacteristicIdentfier;
  BindableMember<ns0::AnalogUnitType_t<double>> CorrectionValueAbsolute;
  BindableMember<ns0::AnalogUnitType_t<double>> CorrectionValueRelative;
  BindableMemberValue<std::string> Description;
  BindableMemberValue<std::string> Identifier;
  BindableMember<ns0::AnalogUnitType_t<double>> LowerControlLimit;
  BindableMember<ns0::AnalogUnitType_t<double>> UpperControlLimit;
};
}  // namespace GMS

REFL_TYPE(GMS::CorrectionType_t, UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsGMSUri, UA_GMSID_CORRECTIONTYPE)))
REFL_FIELD(
  CharacteristicIdentfier,
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsGMSUri, UA_GMSID_CORRECTIONTYPE_CHARACTERISTICIDENTIFIER)))
REFL_FIELD(
  CorrectionValueAbsolute,
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsGMSUri, UA_GMSID_CORRECTIONTYPE_CORRECTIONVALUEABSOLUTE)),
  UmatiServerLib::attribute::PlaceholderOptional())
REFL_FIELD(
  CorrectionValueRelative,
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsGMSUri, UA_GMSID_CORRECTIONTYPE_CORRECTIONVALUERELATIVE)),
  UmatiServerLib::attribute::PlaceholderOptional())
REFL_FIELD(
  Description,
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsGMSUri, UA_GMSID_CORRECTIONTYPE_DESCRIPTION)),
  UmatiServerLib::attribute::PlaceholderOptional())
REFL_FIELD(Identifier, UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsGMSUri, UA_GMSID_CORRECTIONTYPE_IDENTIFIER)))
REFL_FIELD(
  LowerControlLimit,
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsGMSUri, UA_GMSID_CORRECTIONTYPE_LOWERCONTROLLIMIT)),
  UmatiServerLib::attribute::PlaceholderOptional())
REFL_FIELD(
  UpperControlLimit,
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsGMSUri, UA_GMSID_CORRECTIONTYPE_UPPERCONTROLLIMIT)),
  UmatiServerLib::attribute::PlaceholderOptional())
REFL_END
