#pragma once
#include "../TypeDefiniton.hpp"
#include "Constants.hpp"
#include "../ns0/OrderedList.hpp"
#include "StackElementLight.hpp"
#include "../../src_generated/types_industrial_automation_generated.h"

namespace ia
{

  struct BasicStacklight_t : public ns0::OrderedList_t<StackElementLight_t>
  {
    BindableMemberValue<UA_StacklightOperationMode> StacklightMode;
  };

} // namespace ia

REFL_TYPE(ia::BasicStacklight_t,
          Bases<ns0::OrderedList_t<ia::StackElementLight_t>>(),
          UmatiServerLib::attribute::UaObjectType{
              .NodeId = UmatiServerLib::constexp::NodeId(constants::NsIAUri, UA_INDUSTRIAL_AUTOMATIONID_BASICSTACKLIGHTTYPE)})
REFL_FIELD(StacklightMode)
REFL_END
