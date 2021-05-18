#pragma once
#include "../TypeDefinition.hpp"
#include "../ns0/OrderedList.hpp"
#include "Constants.hpp"
#include "StackElementLight.hpp"

namespace ia {

struct BasicStacklight_t : public ns0::OrderedList_t<StackElementLight_t> {
  BindableMemberValue<UA_StacklightOperationMode> StacklightMode;
};

}  // namespace ia

REFL_TYPE(
  ia::BasicStacklight_t,
  Bases<ns0::OrderedList_t<ia::StackElementLight_t>>(),
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsIAUri, UA_IAID_BASICSTACKLIGHTTYPE)))
REFL_FIELD(StacklightMode)
REFL_END
