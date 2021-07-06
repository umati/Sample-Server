#pragma once
#include "../DI/IVendorNameplate.hpp"
#include "../TypeDefinition.hpp"
#include "Constants.hpp"

namespace machinery {

struct IMachineryItemVendorNameplateType_t : public di::IVendorNameplate_t {
  BindableMemberValue<std::uint16_t> YearOfConstruction;
  BindableMemberValue<std::uint8_t> MonthOfConstruction;
  BindableMemberValue<UmatiServerLib::DateTime_t> InitialOperationDate;
};

}  // namespace machinery

REFL_TYPE(
  machinery::IMachineryItemVendorNameplateType_t,
  Bases<di::IVendorNameplate_t>(),
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsMachineryUri, UA_MACHINERYID_IMACHINERYITEMVENDORNAMEPLATETYPE)))
REFL_FIELD(
  YearOfConstruction,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(
    UmatiServerLib::constexp::NodeId(constants::NsMachineryUri, UA_MACHINERYID_IMACHINERYITEMVENDORNAMEPLATETYPE_YEAROFCONSTRUCTION)))
REFL_FIELD(
  MonthOfConstruction,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(
    UmatiServerLib::constexp::NodeId(constants::NsMachineryUri, UA_MACHINERYID_IMACHINERYITEMVENDORNAMEPLATETYPE_MONTHOFCONSTRUCTION)))
REFL_FIELD(
  InitialOperationDate,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(
    UmatiServerLib::constexp::NodeId(constants::NsMachineryUri, UA_MACHINERYID_IMACHINERYITEMVENDORNAMEPLATETYPE_INITIALOPERATIONDATE)))
REFL_FIELD(Manufacturer)
REFL_FIELD(ProductInstanceUri)
REFL_FIELD(SerialNumber)
REFL_END
