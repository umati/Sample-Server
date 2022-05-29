/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2022 (c) mindonwarp
 */
#pragma once

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

}  // namespace GMS

REFL_TYPE(GMS::WorkspaceType_t, UmatiServerLib::attribute::UaDataType(&UA_TYPES_GMS[UA_TYPES_GMS_WORKSPACETYPE]))
REFL_END

REFL_TYPE(GMS::CartesianWorkspaceType_t, UmatiServerLib::attribute::UaDataType(&UA_TYPES_GMS[UA_TYPES_GMS_CARTESIANWORKSPACETYPE]));
REFL_END

REFL_TYPE(GMS::CylindricalWorkspaceType_t, UmatiServerLib::attribute::UaDataType(&UA_TYPES_GMS[UA_TYPES_GMS_CYLINDRICALWORKSPACETYPE]));
REFL_END