/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2022 (c) Alen Galinec
 */
#pragma once

#include "../TypeDefinition.hpp"
#include "Constants.hpp"

namespace GMS {

struct WorkspaceType_t {
  BindableMemberValue<UA_Double> EngineeringUnits;
};

struct CartesianWorkspaceType_t : public WorkspaceType_t {
  BindableMemberValue<UA_Double> Length;
  BindableMemberValue<UA_Double> Width;
  BindableMemberValue<UA_Double> Height;
};

struct CylindricalWorkspaceType_t : public WorkspaceType_t {
  BindableMemberValue<UA_Double> Length;
  BindableMemberValue<UA_Double> Radius;
};

namespace internalTypes {

static const UA_DataType UA_WorkspaceType = {
  UA_TYPENAME("WorkspaceType")        /* .typeName */
  {0, UA_NODEIDTYPE_NUMERIC, {3006}}, /* .typeId */
  {0, UA_NODEIDTYPE_NUMERIC, {0}},    /* .binaryEncodingId, the numericidentifier used on the wire (thenamespaceindex is from .typeId) */
  0,                                  /* .memSize */
  UA_DATATYPEKIND_STRUCTURE,          /* .typeKind */
  true,                               /* .pointerFree */
  false,                              /* .overlayable (depends on endianness and the absence of padding) */
  0,                                  /* .membersSize */
  NULL};
}  // namespace internalTypes

}  // namespace GMS

REFL_TYPE(GMS::WorkspaceType_t, UmatiServerLib::attribute::UaDataType(&GMS::internalTypes::UA_WorkspaceType))
REFL_END

REFL_TYPE(
  GMS::CartesianWorkspaceType_t, Bases<GMS::WorkspaceType_t>(), UmatiServerLib::attribute::UaDataType(&UA_TYPES_GMS[UA_TYPES_GMS_CARTESIANWORKSPACETYPE]));
REFL_END

REFL_TYPE(
  GMS::CylindricalWorkspaceType_t, Bases<GMS::WorkspaceType_t>(), UmatiServerLib::attribute::UaDataType(&UA_TYPES_GMS[UA_TYPES_GMS_CYLINDRICALWORKSPACETYPE]));
REFL_END
