#pragma once
#include "../../UmatiServerLib/BindableMemberPlaceholder.hpp"
#include "../TypeDefinition.hpp"
#include "../ns0/Constants.hpp"
#include "../ns0/BaseObject.hpp"
#include "Constants.hpp"
#include "IWwBaseState.hpp"

namespace woodworking{

struct IWwState_t {
    BindableMember<WwStateMachine_t> Machine;
    //BindableMemberValue<ns0::BaseObject_t> SubUnits;
};


struct WwState_t : public IWwState_t {
};

}  // namespace woodworking

REFL_TYPE(
  woodworking::WwState_t,
  Bases<woodworking::IWwState_t>(),
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_WWMACHINETYPE_STATE))
)
REFL_END

REFL_TYPE(
  woodworking::IWwState_t,
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_IWWSTATETYPE))
)
REFL_FIELD(Machine)
REFL_END
