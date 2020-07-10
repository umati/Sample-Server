#pragma once
#include "../TypeDefiniton.hpp"
#include "Constants.hpp"
#include "../ns0/IOrderedObject.hpp"
#include "ProductionProgram.hpp"

namespace machineTool
{

struct ProductionJob_ProductionPrograms_t : public ns0::OrderedList_t<ProductionProgram_t>
{

};

struct ProductionJob_t : public ns0::IOrderedObject_t<std::uint16_t>
{
  BindableMemberValue<std::string> Identifier;
  BindableMemberValue<std::uint32_t> RunsCompleted;
  BindableMemberValue<std::uint32_t> RunsPlanned;
  BindableMember<ProductionStateMachine_t> State;
  BindableMember<ProductionJob_ProductionPrograms_t> ProductionPrograms;
};

} // namespace machineTool

REFL_TYPE(machineTool::ProductionJob_ProductionPrograms_t,
        Bases<ns0::OrderedList_t<machineTool::ProductionProgram_t>>(),
        UmatiServerLib::attribute::UaObjectType{
        .NodeId = UmatiServerLib::constexp::NodeId(constants::Ns0Uri, UA_NS0ID_ORDEREDLISTTYPE)})
REFL_END


REFL_TYPE(machineTool::ProductionJob_t,
          Bases<ns0::IOrderedObject_t<std::uint16_t>>(),
          UmatiServerLib::attribute::UaObjectType{
              .NodeId = UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PRODUCTIONJOBTYPE)})
REFL_FIELD(Identifier)
REFL_FIELD(RunsCompleted)
REFL_FIELD(RunsPlanned)
REFL_FIELD(State)
REFL_FIELD(ProductionPrograms)
REFL_END
