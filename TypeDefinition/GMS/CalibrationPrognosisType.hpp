/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2020-202 (c) Sebastian Friedl, ISW University of Stuttgart (for umati and VDMA e.V.)
 */

#pragma once
#include "../TypeDefinition.hpp"
#include "Constants.hpp"
#include "../MachineTool/Prognosis.hpp"
#include "../../OpcUaTypes/DateTime.hpp"

namespace GMS {

struct CalibrationPrognosisType_t : public machineTool::Prognosis_t {
  //BindableMemberValue<bool> Calibrated;
  //BindableMemberValue<std::string> CalibrationCertificate;
  //BindableMemberValue<double> CalibrationInterval;
  //BindableMemberValue<double> CalibrationPreptime;
  //BindableMemberValue<UmatiServerLib::DateTime_t> DateOfCalibration;
};

}  // namespace gms

REFL_TYPE(
  GMS::CalibrationPrognosisType_t,
  Bases<machineTool::Prognosis_t>(),
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsGMSUri, UA_GMSID_CALIBRATIONPROGNOSISTYPE)))
/*REFL_FIELD(
    Calibrated,
    UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsGMSUri, UA_GMSID_CALIBRATIONPROGNOSISTYPE_CALIBRATED)))*/
/*REFL_FIELD(
  CalibrationCertificate,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(
    UmatiServerLib::constexp::NodeId(constants::NsGMSUri, UA_GMSID_CALIBRATIONPROGNOSISTYPE_CALIBRATIONCERTIFICATE)))
REFL_FIELD(
    CalibrationInterval,
    UmatiServerLib::attribute::PlaceholderOptional(),
    UmatiServerLib::attribute::MemberInTypeNodeId(
    UmatiServerLib::constexp::NodeId(constants::NsGMSUri, UA_GMSID_CALIBRATIONPROGNOSISTYPE_CALIBRATIONINTERVAL)))
REFL_FIELD(
    CalibrationPreptime,
    UmatiServerLib::attribute::PlaceholderOptional(),
    UmatiServerLib::attribute::MemberInTypeNodeId(
    UmatiServerLib::constexp::NodeId(constants::NsGMSUri, UA_GMSID_CALIBRATIONPROGNOSISTYPE_CALIBRATIONPREPTIME)))
REFL_FIELD(
    DateOfCalibration,
    UmatiServerLib::attribute::PlaceholderOptional(),
    UmatiServerLib::attribute::MemberInTypeNodeId(
    UmatiServerLib::constexp::NodeId(constants::NsGMSUri, UA_GMSID_CALIBRATIONPROGNOSISTYPE_DATEOFCALIBRATION)))*/
REFL_END
