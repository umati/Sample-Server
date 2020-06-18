#pragma once
#include "../TypeDefiniton.hpp"
#include "Constants.hpp"
#include "PrognosisList.hpp"

namespace machineTool
{

struct Notification_t
{
  BindableMember<PrognosisList_t> Prognoses;
};

} // namespace machineTool

REFL_TYPE(machineTool::Notification_t,
          open62541Cpp::attribute::UaObjectType{
              .NodeId = open62541Cpp::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PROGNOSISLISTTYPE)})
REFL_FIELD(Prognoses,
           open62541Cpp::attribute::MemberInTypeNodeId{
               .NodeId = open62541Cpp::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_NOTIFICATIONTYPE_PROGNOSES)},
           open62541Cpp::attribute::PlaceholderOptional())
REFL_END
