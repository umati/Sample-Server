#pragma once
#include "../../UmatiServerLib/BindableMemberPlaceholder.hpp"
#include "../TypeDefinition.hpp"
#include "../ns0/Constants.hpp"
#include "Constants.hpp"

namespace woodworking {

struct ManufacturerSpecific_t {
  BindableMemberValue<std::string> LastProgramName;
};

}  // namespace woodworking

REFL_TYPE(
  woodworking::ManufacturerSpecific_t,
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::Ns0Uri, UA_WOODWORKINGID_WWMACHINETYPE_MANUFACTURERSPECIFIC)))
REFL_FIELD(
  LastProgramName,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(
    UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_WWMACHINETYPE_MANUFACTURERSPECIFIC_LASTPROGRAMNAME)))
REFL_END
