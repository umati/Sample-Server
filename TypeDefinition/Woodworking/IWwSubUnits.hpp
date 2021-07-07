#pragma once
#include "../../UmatiServerLib/BindableMemberPlaceholder.hpp"
#include "../Machinery/IMachineTagNameplate.hpp"
#include "../Machinery/IMachineryItemVendorNameplateType.hpp"
#include "../TypeDefinition.hpp"
#include "../ns0/Constants.hpp"
#include "Constants.hpp"
#include "IWwBaseState.hpp"

namespace woodworking {

struct IWwSubUnits_t {
  // BindableMemberPlaceholder<BindableMember, IWwBaseState_t> SubUnit;
  BindableMember<IWwBaseState_t> SubUnit;
};

}  // namespace woodworking

REFL_TYPE(
  woodworking::IWwSubUnits_t,
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_IWWSUBUNITSTYPE)))
REFL_FIELD(
  SubUnit,
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_IWWSUBUNITSTYPE_SUBUNIT)),
  UmatiServerLib::attribute::PlaceholderOptional())

REFL_END
