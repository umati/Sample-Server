#pragma once
#include "../../UmatiServerLib/BindableMemberPlaceholder.hpp"
#include "../TypeDefinition.hpp"
#include "../ns0/Constants.hpp"
#include "../ns0/BaseObject.hpp"
#include "Constants.hpp"
#include "IWwUnitFlags.hpp"
#include "IWwUnitValues.hpp"
#include "IWwUnitOverview.hpp"

namespace woodworking{

struct IWwBaseState_t {
    BindableMember<IWwUnitOverview_t> Overview;
    BindableMember<IWwUnitFlags_t> Flags;
    BindableMember<IWwUnitValues_t> Values;
};

}  // namespace woodworking

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
REFL_FIELD(
  Values,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_IWWBASESTATETYPE_VALUES))
)

REFL_END
