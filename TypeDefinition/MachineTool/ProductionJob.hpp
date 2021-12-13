/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2020-2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 * Copyright 2020 (c) Tonja Heinemann, ISW University of Stuttgart (for umati and VDW e.V.)
 */

#pragma once
#include "../TypeDefinition.hpp"
#include "../ns0/IOrderedObject.hpp"
#include "../ns0/OrderedList.hpp"
#include "Constants.hpp"
#include "ProductionPartSet.hpp"
#include "ProductionProgram.hpp"

namespace machineTool {

struct ProductionJob_ProductionPrograms_t : public ns0::OrderedList_t<ProductionProgram_t> {};

struct ProductionJob_PartSets_t {
  BindableMemberPlaceholder<BindableMember, ProductionPartSet_t> PartSet;
};

struct ProductionJob_RunsPlanned_t {
  BindableMemberValue<bool> IsValid;
  BindableMemberValue<std::uint32_t> Value;
};

struct ProductionJob_t : public ns0::IOrderedObject_t<std::uint16_t> {
  BindableMemberValue<std::string> Identifier;
  BindableMemberValue<std::uint32_t> RunsCompleted;
  BindableMember<ProductionJob_RunsPlanned_t> RunsPlanned;
  BindableMember<ProductionStateMachine_t> State;
  BindableMember<ProductionJob_ProductionPrograms_t> ProductionPrograms;
  BindableMember<ProductionJob_PartSets_t> PartSets;
};

}  // namespace machineTool

REFL_TYPE(
  machineTool::ProductionJob_ProductionPrograms_t,
  Bases<ns0::OrderedList_t<machineTool::ProductionProgram_t>>(),
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::Ns0Uri, UA_NS0ID_ORDEREDLISTTYPE)))
REFL_END

REFL_TYPE(
  machineTool::ProductionJob_PartSets_t, UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::Ns0Uri, UA_NS0ID_BASEOBJECTTYPE)))
REFL_FIELD(
  PartSet,
  UmatiServerLib::attribute::MemberInTypeNodeId(
    UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PRODUCTIONJOBTYPE_PARTSETS_PARTSET)),
  UmatiServerLib::attribute::PlaceholderOptional())
REFL_END

REFL_TYPE(
  machineTool::ProductionJob_RunsPlanned_t,
  UmatiServerLib::attribute::UaVariableType(UmatiServerLib::constexp::NodeId(constants::Ns0Uri, UA_NS0ID_BASEVARIABLETYPE)))
REFL_FIELD(
  Value,
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PRODUCTIONJOBTYPE_RUNSPLANNED)),
  UmatiServerLib::attribute::UaVariableTypeValue())
REFL_FIELD(
  IsValid,
  UmatiServerLib::attribute::UaBrowseName(constants::NsMachineToolUri),
  UmatiServerLib::attribute::MemberInTypeNodeId(
    UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PRODUCTIONJOBTYPE_RUNSPLANNED_ISVALID)))
REFL_END

REFL_TYPE(
  machineTool::ProductionJob_t,
  Bases<ns0::IOrderedObject_t<std::uint16_t>>(),
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PRODUCTIONJOBTYPE)))
REFL_FIELD(Identifier)
REFL_FIELD(RunsCompleted)
REFL_FIELD(RunsPlanned)
REFL_FIELD(State)
REFL_FIELD(ProductionPrograms)
REFL_FIELD(
  PartSets,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PRODUCTIONJOBTYPE_PARTSETS)))
REFL_END
