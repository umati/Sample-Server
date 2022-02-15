/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2022 (c) Sebastian Friedl, ISW University of Stuttgart (for VDMA e.V.)
 */

#pragma once
#include "../MachineryResult/ResultManagementType.hpp"
#include "../TypeDefinition.hpp"
#include "Constants.hpp"
#include "CorrectionType.hpp"

namespace GMS {

struct GMSResultManagementType_CorrectionsFolder_t {
  BindableMemberPlaceholder<BindableMember, CorrectionType_t> Corrections;
};

struct GMSResultManagementType_t : public machinery_result::ResultManagementType_t {
  BindableMember<GMSResultManagementType_CorrectionsFolder_t> CorrectionsFolder;
};
}  // namespace GMS

REFL_TYPE(
  GMS::GMSResultManagementType_CorrectionsFolder_t,
  UmatiServerLib::attribute::UaObjectType(  // TODO handle correctly when this is pointing to an object instead of an object type
    UmatiServerLib::constexp::NodeId(constants::Ns0Uri, UA_NS0ID_FOLDERTYPE)))
REFL_FIELD(
  Corrections,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(
    UmatiServerLib::constexp::NodeId(constants::NsGMSUri, UA_GMSID_GMSRESULTMANAGEMENTTYPE_CORRECTIONSFOLDER_CORRECTIONS)))
REFL_END

REFL_TYPE(
  GMS::GMSResultManagementType_t,
  Bases<machinery_result::ResultManagementType_t>(),
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsGMSUri, UA_GMSID_GMSRESULTMANAGEMENTTYPE)))
REFL_FIELD(
  CorrectionsFolder,
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsGMSUri, UA_GMSID_GMSRESULTMANAGEMENTTYPE_CORRECTIONSFOLDER)),
  UmatiServerLib::attribute::PlaceholderOptional())
REFL_END
