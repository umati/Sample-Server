#pragma once
#include "../TypeDefiniton.hpp"
#include "Constants.hpp"
#include "BaseEvent.hpp"

namespace ns0
{

struct BaseModelChangeEvent_t : public BaseEvent_t
{
};

} // namespace ns0

REFL_TYPE(ns0::BaseModelChangeEvent_t,
          Bases<ns0::BaseEvent_t>(),
          open62541Cpp::attribute::UaObjectType{
              .NodeId = open62541Cpp::constexp::NodeId(constants::Ns0Uri, UA_NS0ID_BASEMODELCHANGEEVENTTYPE)})
REFL_END
