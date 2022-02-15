/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2022 (c) Sebastian Friedl, ISW University of Stuttgart (for umati and VDMA e.V.)
 */

#pragma once
#include "../MachineTool/MaintenancePrognosis.hpp"
#include "../MachineTool/ManualActivityPrognosis.hpp"
#include "../MachineTool/PartLoadPrognosis.hpp"
#include "../MachineTool/PartUnLoadPrognosis.hpp"
#include "../MachineTool/ProcessChangeoverPrognosis.hpp"
#include "../MachineTool/ProductionJobEndPrognosis.hpp"
#include "../MachineTool/PrognosisList.hpp"
#include "../MachineTool/ToolChangePrognosis.hpp"
#include "../MachineTool/ToolLoadPrognosis.hpp"
#include "../MachineTool/ToolUnloadPrognosis.hpp"
#include "../MachineTool/UtilityChangePrognosis.hpp"
#include "../TypeDefinition.hpp"
#include "CalibrationPrognosisType.hpp"
#include "Constants.hpp"

namespace GMS {

struct GMSPrognosisList_t : public machineTool::PrognosisList_t {
  BindableMember<CalibrationPrognosisType_t> Calibration;
};

}  // namespace GMS

REFL_TYPE(
  GMS::GMSPrognosisList_t,
  Bases<machineTool::PrognosisList_t>(),
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PROGNOSISLISTTYPE)))
REFL_FIELD(
  Calibration,
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsGMSUri, UA_GMSID_GMSTYPE_NOTIFICATION_PROGNOSES_CALIBRATION)),
  UmatiServerLib::attribute::PlaceholderOptional())
REFL_END
