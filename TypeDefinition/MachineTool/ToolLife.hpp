/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2020-2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 * Copyright 2020 (c) Tonja Heinemann, ISW University of Stuttgart (for umati and VDW e.V.)
 */

#pragma once
#include "../../OpcUaTypes/EUInformation.hpp"
#include "../../src_generated/types_machinetool_generated.h"
#include "../TypeDefinition.hpp"
#include "BaseTool.hpp"
#include "Constants.hpp"

namespace machineTool {
template <typename T>
struct ToolLife_t {
  BindableMemberValue<T> Value;
  BindableMemberValue<UmatiServerLib::EUInformation_t> EngineeringUnits;
  BindableMemberValue<UA_ToolLifeIndication> Indication;
  BindableMemberValue<bool> IsCountingUp;
  BindableMemberValue<T> LimitValue;
  BindableMemberValue<T> StartValue;
  BindableMemberValue<T> WarningValue;
};
}  // namespace machineTool

REFL_TEMPLATE(
  (typename T),
  (machineTool::ToolLife_t<T>),
  UmatiServerLib::attribute::UaVariableType(UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_TOOLLIFETYPE)))
REFL_FIELD(Value, UmatiServerLib::attribute::UaVariableTypeValue())
REFL_FIELD(EngineeringUnits)
REFL_FIELD(Indication)
REFL_FIELD(IsCountingUp)
REFL_FIELD(
  LimitValue,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_TOOLLIFETYPE_LIMITVALUE)))
REFL_FIELD(
  StartValue,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_TOOLLIFETYPE_STARTVALUE)))
REFL_FIELD(
  WarningValue,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_TOOLLIFETYPE_WARNINGVALUE)))
REFL_END
