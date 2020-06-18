#pragma once
#include "../TypeDefiniton.hpp"
#include "Constants.hpp"
#include "../DI/IVendorNameplate.hpp"

namespace machinery
{

struct IMachineVendorNameplate_t : public di::IVendorNameplate_t
{
  BindableMemberValue<std::uint16_t> YearOfConstruction;
};

} // namespace machinery

REFL_TYPE(machinery::IMachineVendorNameplate_t,
          Bases<di::IVendorNameplate_t>(),
          open62541Cpp::attribute::UaObjectType{
              .NodeId = open62541Cpp::constexp::NodeId(constants::NsMachineryUri, UA_MACHINERY_ID_IMACHINEVENDORNAMEPLATETYPE)})
REFL_FIELD(YearOfConstruction, open62541Cpp::attribute::PlaceholderOptional(),
           open62541Cpp::attribute::MemberInTypeNodeId{
               .NodeId = open62541Cpp::constexp::NodeId(constants::NsMachineryUri, UA_MACHINERY_ID_IMACHINEVENDORNAMEPLATETYPE_YEAROFCONSTRUCTION)})
REFL_FIELD(Manufacturer);
REFL_FIELD(ProductInstanceUri);
REFL_FIELD(SerialNumber);
REFL_END
