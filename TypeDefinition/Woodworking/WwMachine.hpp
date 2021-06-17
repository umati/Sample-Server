#pragma once
#include "../TypeDefinition.hpp"
#include "Constants.hpp"
#include "MachineIdentification.hpp"
#include "../ns0/BaseObject.hpp"
#include "../ns0/StateVariable.hpp"
#include "IWwState.hpp"


namespace woodworking{

struct WwMachine_t {
  BindableMember<MachineIdentification_t> Identification;
  BindableMember<WwState_t> State;
  //BindableMember<ns0::BaseObject_t> ManufacturerSpecific;
};
}  // namespace woodworking

REFL_TYPE(
  woodworking::WwMachine_t ,
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_WWMACHINETYPE))
)
REFL_FIELD(
  Identification,
  UmatiServerLib::attribute::UaBrowseName(constants::NsDIUri)
)
REFL_FIELD(State)

//REFL_FIELD(
//  ManufacturerSpecific,
//  UmatiServerLib::attribute::PlaceholderOptional(),
//  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_WWMACHINETYPE_MANUFACTURERSPECIFIC)))
REFL_END
