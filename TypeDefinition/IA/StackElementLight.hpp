#pragma once
#include "../TypeDefinition.hpp"
#include "Constants.hpp"
#include "StackElement.hpp"
#include "../ns0/AnalogItem.hpp"
#include "../../src_generated/types_ia_generated.h"

namespace ia
{

  struct StackElementLight_t : public StackElement_t
  {
    BindableMemberValue<ns0::AnalogItem_t<float>> Intensity;
    BindableMemberValue<UA_SignalColor> SignalColor;
    BindableMemberValue<UA_SignalModeLight> StacklightMode;
  };

} // namespace ia

REFL_TYPE(ia::StackElementLight_t,
          Bases<ia::StackElement_t>(),
          UmatiServerLib::attribute::UaObjectType{
              .NodeId = UmatiServerLib::constexp::NodeId(constants::NsIAUri, UA_IAID_STACKELEMENTLIGHTTYPE)})
REFL_FIELD(Intensity, UmatiServerLib::attribute::MemberInTypeNodeId{
               .NodeId = UmatiServerLib::constexp::NodeId(constants::NsIAUri, UA_IAID_STACKELEMENTLIGHTTYPE_INTENSITY)},
           UmatiServerLib::attribute::PlaceholderOptional())
REFL_FIELD(SignalColor, UmatiServerLib::attribute::MemberInTypeNodeId{
               .NodeId = UmatiServerLib::constexp::NodeId(constants::NsIAUri, UA_IAID_STACKELEMENTLIGHTTYPE_SIGNALCOLOR)},
           UmatiServerLib::attribute::PlaceholderOptional())
REFL_FIELD(StacklightMode, UmatiServerLib::attribute::MemberInTypeNodeId{
               .NodeId = UmatiServerLib::constexp::NodeId(constants::NsIAUri, UA_IAID_STACKELEMENTLIGHTTYPE_STACKLIGHTMODE)},
           UmatiServerLib::attribute::PlaceholderOptional())
REFL_END
