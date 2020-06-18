#pragma once
#include "../TypeDefiniton.hpp"
#include "Constants.hpp"
#include "MaintenancePrognosis.hpp"
#include "UtilityChangePrognosis.hpp"

namespace machineTool
{

struct PrognosisList_t
{
  BindableMemberValue<std::string> NodeVersion;
  BindableMemberPlaceholder<BindableMember, std::variant<MaintenancePrognosis_t, UtilityChangePrognosis_t>> Prognosis;
};

} // namespace machineTool

REFL_TYPE(machineTool::PrognosisList_t,
          open62541Cpp::attribute::UaObjectType{
              .NodeId = open62541Cpp::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PROGNOSISLISTTYPE)})
REFL_FIELD(Prognosis,
           open62541Cpp::attribute::MemberInTypeNodeId{
               .NodeId = open62541Cpp::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PROGNOSISLISTTYPE_PROGNOSIS)},
           open62541Cpp::attribute::PlaceholderOptional())
REFL_FIELD(NodeVersion,
           open62541Cpp::attribute::MemberInTypeNodeId{
               .NodeId = open62541Cpp::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PROGNOSISLISTTYPE_NODEVERSION)},
           open62541Cpp::attribute::PlaceholderOptional())
REFL_END
