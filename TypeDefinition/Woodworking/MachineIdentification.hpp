#pragma once
#include "../../UmatiServerLib/BindableMemberPlaceholder.hpp"
#include "../Machinery/IMachineTagNameplate.hpp"
#include "../Machinery/IMachineryItemVendorNameplateType.hpp"
#include "../TypeDefinition.hpp"
#include "../ns0/Constants.hpp"
#include "Constants.hpp"

namespace woodworking{

struct MachineIdentification_t : public machinery::IMachineryItemVendorNameplateType_t, public machinery::IMachineTagNameplate_t {
    BindableMemberValue<std::string> LocationPlant;
    BindableMemberValue<std::string> LocationGPS;
    BindableMemberValue<UmatiServerLib::LocalizedText_t> CustomerCompanyName;
};

}  // namespace woodworking

REFL_TYPE(
  woodworking::MachineIdentification_t,
  Bases<machinery::IMachineryItemVendorNameplateType_t, machinery::IMachineTagNameplate_t>(),
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_WWMACHINETYPE_IDENTIFICATION)))
REFL_FIELD(
  LocationPlant,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_WWMACHINETYPE_IDENTIFICATION_LOCATIONPLANT)))
REFL_FIELD(
  LocationGPS,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_WWMACHINETYPE_IDENTIFICATION_LOCATIONGPS)))
REFL_FIELD(
  CustomerCompanyName,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_WWMACHINETYPE_IDENTIFICATION_CUSTOMERCOMPANYNAME)))
REFL_END
