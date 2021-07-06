#pragma once
#include "../../UmatiServerLib/BindableMemberPlaceholder.hpp"
#include "../TypeDefinition.hpp"
#include "../ns0/BaseObject.hpp"
#include "../ns0/Constants.hpp"
#include "Constants.hpp"
#include "IWwBaseState.hpp"
#include "IWwSubUnits.hpp"

namespace woodworking {

struct IWwState_t {
  BindableMember<IWwBaseState_t> Machine;
  BindableMember<IWwSubUnits_t> SubUnits;
};

}  // namespace woodworking

REFL_TYPE(
  woodworking::IWwState_t,
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_IWWSTATETYPE)))
REFL_FIELD(Machine)
REFL_FIELD(
  SubUnits,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_WWMACHINETYPE_STATE_SUBUNITS)))
REFL_END
