#pragma once
#include <string>

#include "../../UmatiServerLib/BindableMemberPlaceholder.hpp"
#include "../TypeDefinition.hpp"
#include "../ns0/Constants.hpp"
#include "Constants.hpp"
#include "ProductionPart.hpp"

namespace machineTool {

struct ProductionPartSet_PartsPerRun_t {
  BindableMemberPlaceholder<BindableMember, ProductionPart_t> Part;
};

struct ProductionPartSet_t {
  BindableMemberValue<bool> ContainsMixedParts;
  BindableMemberValue<std::string> Name;
  BindableMemberValue<std::uint32_t> PartsCompletedPerRun;
  BindableMemberValue<std::uint32_t> PartsPlannedPerRun;
  BindableMember<ProductionPartSet_PartsPerRun_t> PartsPerRun;
};

}  // namespace machineTool

REFL_TYPE(
  machineTool::ProductionPartSet_PartsPerRun_t,
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::Ns0Uri, UA_NS0ID_BASEOBJECTTYPE)))
REFL_FIELD(
  Part,
  UmatiServerLib::attribute::MemberInTypeNodeId(
    UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PRODUCTIONPARTSETTYPE_PARTSPERRUN_PART)),
  UmatiServerLib::attribute::PlaceholderOptional())
REFL_END

REFL_TYPE(
  machineTool::ProductionPartSet_t,
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PRODUCTIONPARTSETTYPE)))
REFL_FIELD(ContainsMixedParts)
REFL_FIELD(
  Name,
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PRODUCTIONPARTSETTYPE_NAME)),
  UmatiServerLib::attribute::PlaceholderOptional())
REFL_FIELD(PartsCompletedPerRun)
REFL_FIELD(PartsPlannedPerRun)
REFL_FIELD(
  PartsPerRun,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(
    UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PRODUCTIONPARTSETTYPE_PARTSPERRUN)))
REFL_END
