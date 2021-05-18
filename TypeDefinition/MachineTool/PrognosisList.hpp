#pragma once
#include "../TypeDefinition.hpp"
#include "Constants.hpp"
#include "MaintenancePrognosis.hpp"
#include "ManualActivityPrognosis.hpp"
#include "PartLoadPrognosis.hpp"
#include "PartUnLoadPrognosis.hpp"
#include "ProcessChangeoverPrognosis.hpp"
#include "ProductionJobEndPrognosis.hpp"
#include "ToolChangePrognosis.hpp"
#include "ToolLoadPrognosis.hpp"
#include "ToolUnloadPrognosis.hpp"
#include "UtilityChangePrognosis.hpp"

namespace machineTool {

struct PrognosisList_t {
  BindableMemberValue<std::string> NodeVersion;
  BindableMemberPlaceholder<
    BindableMember,
    std::variant<
      MaintenancePrognosis_t,
      ManualActivityPrognosis_t,
      PartLoadPrognosis_t,
      PartUnLoadPrognosis_t,
      ProcessChangeoverPrognosis_t,
      ProductionJobEndPrognosis_t,
      ToolChangePrognosis_t,
      ToolLoadPrognosis_t,
      ToolUnloadPrognosis_t,
      UtilityChangePrognosis_t>>
    Prognosis;
};

}  // namespace machineTool

REFL_TYPE(
  machineTool::PrognosisList_t,
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PROGNOSISLISTTYPE)))
REFL_FIELD(
  Prognosis,
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PROGNOSISLISTTYPE_PROGNOSIS)),
  UmatiServerLib::attribute::PlaceholderOptional())
REFL_FIELD(
  NodeVersion,
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PROGNOSISLISTTYPE_NODEVERSION)),
  UmatiServerLib::attribute::PlaceholderOptional())
REFL_END
