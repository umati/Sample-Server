#pragma once
#include "../../UmatiServerLib/BindableMemberPlaceholder.hpp"
#include "../TypeDefinition.hpp"
#include "../ns0/Constants.hpp"
#include "../ns0/BaseObject.hpp"
#include "Constants.hpp"
#include "IWwUnitFlags.hpp"
#include "IWwUnitOverview.hpp"

namespace woodworking{

struct IWwBaseState_t {
    BindableMember<WwStateMachineOverview_t> Overview;
    BindableMember<WwStateMachineFlags_t> Flags;
    //BindableMemberValue<BaseObject> Values;
};

struct WwStateMachine_t : public IWwBaseState_t {
};

}  // namespace woodworking

REFL_TYPE(
  woodworking::WwStateMachine_t,
  Bases<woodworking::IWwBaseState_t>(),
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_WWMACHINETYPE_STATE))
)

REFL_END

REFL_TYPE(
  woodworking::IWwBaseState_t,
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_IWWBASESTATETYPE))
)
REFL_FIELD(Overview)
REFL_FIELD(
  Flags,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_IWWBASESTATETYPE_FLAGS))
)
REFL_END
