#pragma once
#include "../TypeDefinition.hpp"
#include "Constants.hpp"
#include "PrognosisList.hpp"
#include "../ns0/BaseObject.hpp"

namespace machineTool
{

  struct Notification_t
  {
    BindableMember<PrognosisList_t> Prognoses;
    BindableMember<ns0::BaseObject_t> Messages;
  };

} // namespace machineTool

REFL_TYPE(machineTool::Notification_t,
          UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PROGNOSISLISTTYPE)))
REFL_FIELD(Prognoses,
           UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_NOTIFICATIONTYPE_PROGNOSES)),
           UmatiServerLib::attribute::PlaceholderOptional())
REFL_FIELD(Messages,
           UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_NOTIFICATIONTYPE_MESSAGES)),
           UmatiServerLib::attribute::PlaceholderOptional())
REFL_END
