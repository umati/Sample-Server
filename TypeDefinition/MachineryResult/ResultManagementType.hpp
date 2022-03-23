/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2022 (c) Sebastian Friedl, ISW University of Stuttgart (for VDMA e.V.)
 */

#pragma once
#include "../TypeDefinition.hpp"
#include "../ns0/Folder.hpp"
#include "Constants.hpp"
#include "ResultDataType.hpp"
#include "ResultVariableType.hpp"

namespace machinery_result {

struct ResultManagementType_Results_t {
  BindableMemberPlaceholder<BindableMember, machinery_result::ResultType_t> ResultVariable;
};

struct ResultManagementType_t {
  BindableMember<ResultManagementType_Results_t> Results;
};

}  // namespace machinery_result

REFL_TYPE(
  machinery_result::ResultManagementType_Results_t,
  UmatiServerLib::attribute::UaObjectType(  // TODO handle correctly when this is pointing to an object instead of an object type
    UmatiServerLib::constexp::NodeId(constants::Ns0Uri, UA_NS0ID_FOLDERTYPE)))
REFL_FIELD(
  ResultVariable,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(
    UmatiServerLib::constexp::NodeId(constants::NsMachineryResultUri, UA_MACHINERY_RESULTID_RESULTMANAGEMENTTYPE_RESULTS_RESULTVARIABLE)))
REFL_END

REFL_TYPE(
  machinery_result::ResultManagementType_t,
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsMachineryResultUri, UA_MACHINERY_RESULTID_RESULTMANAGEMENTTYPE)))
REFL_FIELD(
  Results,
  UmatiServerLib::attribute::MemberInTypeNodeId(
    UmatiServerLib::constexp::NodeId(constants::NsMachineryResultUri, UA_MACHINERY_RESULTID_RESULTMANAGEMENTTYPE_RESULTS)),
  UmatiServerLib::attribute::PlaceholderOptional())
REFL_END
