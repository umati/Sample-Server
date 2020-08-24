#pragma once
#include "../TypeDefinition.hpp"
#include "Constants.hpp"
#include "ProductionProgram.hpp"

namespace machineTool
{

  struct ProductionActiveProgram_t : public ProductionProgram_t
  {
    BindableMemberValue<std::string> JobIdentifier;
    BindableMemberValue<UA_NodeId> JobNodeId;
  };

} // namespace machineTool

REFL_TYPE(machineTool::ProductionActiveProgram_t,
          Bases<machineTool::ProductionProgram_t>(),
          UmatiServerLib::attribute::UaObjectType{
              .NodeId = UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PRODUCTIONACTIVEPROGRAMTYPE)})
//REFL_FIELD(State) // Override mandatory ///\todo requires fix in nodeset, i=5030 needs to be ProductionActiveProgramType not ProductionProgramType
REFL_FIELD(JobIdentifier,
           UmatiServerLib::attribute::MemberInTypeNodeId{
               .NodeId = UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PRODUCTIONACTIVEPROGRAMTYPE_JOBIDENTIFIER)},
           UmatiServerLib::attribute::PlaceholderOptional())
REFL_FIELD(JobNodeId,
           UmatiServerLib::attribute::MemberInTypeNodeId{
               .NodeId = UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PRODUCTIONACTIVEPROGRAMTYPE_JOBNODEID)},
           UmatiServerLib::attribute::PlaceholderOptional())
REFL_END
