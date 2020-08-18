#pragma once
#include "../TypeDefiniton.hpp"
#include "Constants.hpp"
#include <string>

namespace machineTool
{

struct ProductionPart_t
{
  BindableMemberValue<std::string> CustomerOrderIdentifier;
  BindableMemberValue<std::string> Identifier;
  BindableMemberValue<std::string> Name;
  //BindableMemberValue<> PartQuality;
  //BindableMemberValue<> ProcessIrregularity;
};

} // namespace machineTool

REFL_TYPE(machineTool::ProductionPart_t,
          UmatiServerLib::attribute::UaObjectType{
              .NodeId = UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PRODUCTIONPARTTYPE)})
REFL_FIELD(CustomerOrderIdentifier,
            UmatiServerLib::attribute::PlaceholderOptional(),
            UmatiServerLib::attribute::MemberInTypeNodeId{
               .NodeId = UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PRODUCTIONPARTTYPE_CUSTOMERORDERIDENTIFIER)})
REFL_FIELD(Identifier,
            UmatiServerLib::attribute::PlaceholderOptional(),
            UmatiServerLib::attribute::MemberInTypeNodeId{
               .NodeId = UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PRODUCTIONPARTTYPE_IDENTIFIER)})
REFL_FIELD(Name)
//REFL_FIELD(PartQuality)
//REFL_FIELD(ProcessIrregularity)
REFL_END
