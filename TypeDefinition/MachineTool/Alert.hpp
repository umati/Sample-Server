#pragma once
#include "../TypeDefiniton.hpp"
#include "Constants.hpp"
#include "../ns0/AlarmCondition.hpp"

namespace machineTool
{

struct Alert_t : public ns0::AlarmCondition_t
{
  std::string ErrorCode;
};

} // namespace machineTool

REFL_TYPE(machineTool::Alert_t,
          Bases<ns0::AlarmCondition_t>(),
          UmatiServerLib::attribute::UaObjectType{
              .NodeId = UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_ALERTTYPE)})
REFL_FIELD(ErrorCode)
REFL_END
