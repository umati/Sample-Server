#pragma once
#include "../TypeDefinition.hpp"
#include "Constants.hpp"

namespace di
{

  struct IVendorNameplate_t
  {
    BindableMemberValue<std::string> ProductInstanceUri;
    BindableMemberValue<UmatiServerLib::LocalizedText_t> Model;
    BindableMemberValue<std::string> SoftwareRevision;
    BindableMemberValue<UmatiServerLib::LocalizedText_t> Manufacturer;
    BindableMemberValue<std::string> SerialNumber;
    BindableMemberValue<std::string> DeviceClass;
    BindableMemberValue<std::string> ProductCode;
  };

} // namespace di

REFL_TYPE(di::IVendorNameplate_t,
          UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsDIUri, UA_DIID_IVENDORNAMEPLATETYPE)))
REFL_FIELD(ProductInstanceUri, UmatiServerLib::attribute::PlaceholderOptional(),
           UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsDIUri, UA_DIID_IVENDORNAMEPLATETYPE_PRODUCTINSTANCEURI)))
REFL_FIELD(ProductCode, UmatiServerLib::attribute::PlaceholderOptional(),
           UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsDIUri, UA_DIID_IVENDORNAMEPLATETYPE_PRODUCTCODE)))
REFL_FIELD(Model, UmatiServerLib::attribute::PlaceholderOptional(),
           UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsDIUri, UA_DIID_IVENDORNAMEPLATETYPE_MODEL)))
REFL_FIELD(SoftwareRevision, UmatiServerLib::attribute::PlaceholderOptional(),
           UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsDIUri, UA_DIID_IVENDORNAMEPLATETYPE_SOFTWAREREVISION)))
REFL_FIELD(Manufacturer, UmatiServerLib::attribute::PlaceholderOptional(),
           UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsDIUri, UA_DIID_IVENDORNAMEPLATETYPE_MANUFACTURER)))
REFL_FIELD(SerialNumber, UmatiServerLib::attribute::PlaceholderOptional(),
           UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsDIUri, UA_DIID_IVENDORNAMEPLATETYPE_SERIALNUMBER)))
REFL_FIELD(DeviceClass, UmatiServerLib::attribute::PlaceholderOptional(),
           UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsDIUri, UA_DIID_IVENDORNAMEPLATETYPE_DEVICECLASS)))
REFL_END
