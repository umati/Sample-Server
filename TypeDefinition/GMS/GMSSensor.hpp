/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2022 (c) mindonwarp
 */

#pragma once
#include <cstdint>
#include <string>
#include <vector>

#include "../TypeDefinition.hpp"
#include "../MachineTool/Tool.hpp"

#include "Constants.hpp"

namespace GMS {

struct GMSToolLife_t : public machineTool::ToolLife_t<double> {
  BindableMemberValue<double> Value;
  BindableMemberValue<UA_ToolLifeIndication> Indication;
  BindableMemberValue<bool> IsCountingUp;
};

struct GMSSensor_ToolLife_t {
  BindableMember<GMSToolLife_t> Qualified;
};

struct GMSSensor_t : public machineTool::Tool_t {
  BindableMemberValue<UA_ToolAlignmentState> Alignment;
  BindableMemberValue<std::vector<std::string>> Axes;
  BindableMemberValue<std::vector<std::uint32_t>> Capabilities;
  BindableMemberValue<std::uint32_t> Class;
  BindableMemberValue<UA_ToolIsQualifiedStatus> IsQualifiedStatus;
  BindableMember<GMSSensor_ToolLife_t> ToolLife;
};

}  // namespace GMS

REFL_TYPE(
  GMS::GMSToolLife_t,
  Bases<machineTool::ToolLife_t<double>>(),
  UmatiServerLib::attribute::UaVariableType(UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_TOOLLIFETYPE)))
REFL_FIELD(Value, UmatiServerLib::attribute::UaVariableTypeValue())
REFL_FIELD(
  Indication,
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsGMSUri, UA_GMSID_SENSORTYPE_TOOLLIFE_QUALIFIED_INDICATION)))
REFL_FIELD(
  IsCountingUp,
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsGMSUri, UA_GMSID_SENSORTYPE_TOOLLIFE_QUALIFIED_ISCOUNTINGUP)))
REFL_END

REFL_TYPE(GMS::GMSSensor_ToolLife_t, UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::Ns0Uri, UA_NS0ID_BASEOBJECTTYPE)))
REFL_FIELD(
  Qualified,
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsGMSUri, UA_GMSID_SENSORTYPE_TOOLLIFE_QUALIFIED)),
  UmatiServerLib::attribute::PlaceholderOptional())
REFL_END

REFL_TYPE(
  GMS::GMSSensor_t,
  Bases<machineTool::Tool_t>(),
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsGMSUri, UA_GMSID_SENSORTYPE)))
REFL_FIELD(Class, UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsGMSUri, UA_GMSID_SENSORTYPE_CLASS)))
REFL_FIELD(
  ToolLife,
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsGMSUri, UA_GMSID_SENSORTYPE_TOOLLIFE)),
  UmatiServerLib::attribute::PlaceholderOptional())
REFL_FIELD(
  Alignment,
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsGMSUri, UA_GMSID_SENSORTYPE_ALIGNMENT)),
  UmatiServerLib::attribute::PlaceholderOptional())
REFL_FIELD(
  Axes,
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsGMSUri, UA_GMSID_SENSORTYPE_AXES)),
  UmatiServerLib::attribute::PlaceholderOptional())
REFL_FIELD(
  Capabilities,
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsGMSUri, UA_GMSID_SENSORTYPE_CAPABILITIES)),
  UmatiServerLib::attribute::PlaceholderOptional())
REFL_FIELD(
  IsQualifiedStatus,
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsGMSUri, UA_GMSID_SENSORTYPE_ISQUALIFIEDSTATUS)),
  UmatiServerLib::attribute::PlaceholderOptional())
REFL_END