#pragma once
#include "../TypeDefinition.hpp"
#include "Constants.hpp"

namespace di {

struct TopologyElement_t {};

}  // namespace di

REFL_TYPE(di::TopologyElement_t, UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsDIUri, UA_DIID_TOPOLOGYELEMENTTYPE)))
REFL_END
