#pragma once
#include "../TypeDefiniton.hpp"
#include "Constants.hpp"

namespace ns0
{

struct BaseObject_t
{
};

} // namespace ns0

REFL_TYPE(ns0::BaseObject_t,
          open62541Cpp::attribute::UaObjectType{
              .NodeId = open62541Cpp::constexp::NodeId(constants::Ns0Uri, UA_NS0ID_BASEOBJECTTYPE)})
REFL_END
