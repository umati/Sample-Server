#pragma once
#include "../TypeDefiniton.hpp"
#include "Constants.hpp"
#include "ToolList.hpp"

namespace machineTool
{

  struct Equipment_t
  {
    BindableMember<ToolList_t> Tools;
  };

} // namespace machineTool

REFL_TYPE(machineTool::Equipment_t,
          open62541Cpp::attribute::UaObjectType{
              .NodeId = open62541Cpp::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_EQUIPMENTTYPE)})
REFL_FIELD(Tools,
           open62541Cpp::attribute::MemberInTypeNodeId{
               .NodeId = open62541Cpp::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_EQUIPMENTTYPE_TOOLS)},
           open62541Cpp::attribute::PlaceholderOptional())
REFL_END
