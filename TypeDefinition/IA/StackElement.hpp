#pragma once
#include "../TypeDefinition.hpp"
#include "Constants.hpp"
#include "../ns0/IOrderedObject.hpp"
#include <cstdint>

namespace ia
{

  struct StackElement_t : public ns0::IOrderedObject_t<std::uint16_t>
  {
    BindableMemberValue<bool> IsPartOfBase;
    BindableMemberValue<bool> SignalOn;
  };

} // namespace ia

REFL_TYPE(ia::StackElement_t,
          Bases<ns0::IOrderedObject_t<std::uint16_t>>(),
          UmatiServerLib::attribute::UaObjectType{
              .NodeId = UmatiServerLib::constexp::NodeId(constants::NsIAUri, UA_INDUSTRIAL_AUTOMATIONID_STACKELEMENTTYPE)})

REFL_FIELD(IsPartOfBase, UmatiServerLib::attribute::MemberInTypeNodeId{
               .NodeId = UmatiServerLib::constexp::NodeId(constants::NsIAUri, UA_INDUSTRIAL_AUTOMATIONID_STACKELEMENTTYPE_ISPARTOFBASE)},
           UmatiServerLib::attribute::PlaceholderOptional())
REFL_FIELD(SignalOn, UmatiServerLib::attribute::MemberInTypeNodeId{
               .NodeId = UmatiServerLib::constexp::NodeId(constants::NsIAUri, UA_INDUSTRIAL_AUTOMATIONID_STACKELEMENTTYPE_SIGNALON)},
           UmatiServerLib::attribute::PlaceholderOptional())
REFL_END
