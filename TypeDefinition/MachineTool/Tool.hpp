#pragma once
#include "../TypeDefinition.hpp"
#include "Constants.hpp"
#include "BaseTool.hpp"
#include "../../src_generated/types_machinetool_generated.h"
#include "ToolLife.hpp"

namespace machineTool
{
  struct Tool_Locked_t
  {
    BindableMemberValue<bool> Value;
    BindableMemberValue<UA_ToolLocked> ReasonForLocking;
  };

  struct Tool_ToolLife_t
  {
    BindableMemberPlaceholder<BindableMember, std::variant<ToolLife_t<std::int32_t>, ToolLife_t<double>>> ToolLifeEntry;
  };

  struct Tool_t : public BaseTool_t
  {
    BindableMemberValue<std::uint32_t> ControlIdentifier1;
    BindableMemberValue<std::uint32_t> ControlIdentifier2;
    BindableMemberValue<UA_ToolManagement> ControlIdentifierInterpretation;
    BindableMemberValue<UmatiServerLib::DateTime_t> LastUsage;
    BindableMember<Tool_Locked_t> Locked;
    BindableMemberValue<bool> PlannedForOperating;
    BindableMember<Tool_ToolLife_t> ToolLife;
    //Location;
  };

} // namespace machineTool

REFL_TYPE(machineTool::Tool_Locked_t,
          UmatiServerLib::attribute::UaVariableType{
              .NodeId = UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_TOOLTYPE)})
REFL_FIELD(Value,
           UmatiServerLib::attribute::MemberInTypeNodeId{
               .NodeId = UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_TOOLTYPE_LOCKED)},
           UmatiServerLib::attribute::UaVariableTypeValue())
REFL_FIELD(ReasonForLocking,
           UmatiServerLib::attribute::MemberInTypeNodeId{
               .NodeId = UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_TOOLTYPE_LOCKED_REASONFORLOCKING)})
REFL_END

REFL_TYPE(machineTool::Tool_ToolLife_t,
          UmatiServerLib::attribute::UaObjectType{ // TOOD handle correctly when this is pointing to an object instead of an object type
              .NodeId = UmatiServerLib::constexp::NodeId(constants::Ns0Uri, UA_NS0ID_BASEOBJECTTYPE)})
REFL_FIELD(ToolLifeEntry,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId{
               .NodeId = UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_TOOLTYPE_TOOLLIFE_TOOLLIFEENTRY)})
REFL_END

REFL_TYPE(machineTool::Tool_t,
          Bases<machineTool::BaseTool_t>(),
          UmatiServerLib::attribute::UaObjectType{
              .NodeId = UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_TOOLTYPE)})
REFL_FIELD(ControlIdentifier1,
           UmatiServerLib::attribute::MemberInTypeNodeId{
               .NodeId = UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_TOOLTYPE_CONTROLIDENTIFIER1)})
REFL_FIELD(ControlIdentifier2,
           UmatiServerLib::attribute::MemberInTypeNodeId{.NodeId = UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_TOOLTYPE_CONTROLIDENTIFIER2)},
           UmatiServerLib::attribute::PlaceholderOptional())
REFL_FIELD(ControlIdentifierInterpretation,
           UmatiServerLib::attribute::MemberInTypeNodeId{
               .NodeId = UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_TOOLTYPE_CONTROLIDENTIFIERINTERPRETATION)})
REFL_FIELD(LastUsage,
           UmatiServerLib::attribute::MemberInTypeNodeId{.NodeId = UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_TOOLTYPE_LASTUSAGE)},
           UmatiServerLib::attribute::PlaceholderOptional())
REFL_FIELD(Locked,
           UmatiServerLib::attribute::MemberInTypeNodeId{
               .NodeId = UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_TOOLTYPE_LOCKED)})
REFL_FIELD(PlannedForOperating,
           UmatiServerLib::attribute::MemberInTypeNodeId{
               .NodeId = UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_TOOLTYPE_PLANNEDFOROPERATING)},
           UmatiServerLib::attribute::PlaceholderOptional())
REFL_FIELD(ToolLife,
           UmatiServerLib::attribute::MemberInTypeNodeId{
               .NodeId = UmatiServerLib::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_TOOLTYPE_TOOLLIFE)},
           UmatiServerLib::attribute::PlaceholderOptional())
REFL_END
