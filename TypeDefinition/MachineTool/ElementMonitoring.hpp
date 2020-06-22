#pragma once
#include "../TypeDefiniton.hpp"
#include "Constants.hpp"

namespace machineTool
{
struct ElementMonitoring_t
{
  BindableMemberValue<std::string> Name;
};
} // namespace machineTool

REFL_TYPE(machineTool::ElementMonitoring_t,
          open62541Cpp::attribute::UaObjectType{
              .NodeId = open62541Cpp::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_ELEMENTMONITORINGTYPE)})
REFL_FIELD(Name)
REFL_END
