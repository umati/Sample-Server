#pragma once
#include "../TypeDefinition.hpp"
#include "Constants.hpp"

namespace di {

struct IVendorNameplate_t {
  BindableMemberValue<UmatiServerLib::LocalizedText_t> Manufacturer;
  BindableMemberValue<std::string> ManufacturerUri;
  BindableMemberValue<UmatiServerLib::LocalizedText_t> Model;
  BindableMemberValue<std::string> ProductCode;
  BindableMemberValue<std::string> HardwareRevision;
  BindableMemberValue<std::string> SoftwareRevision;
  BindableMemberValue<std::string> DeviceRevision;
  BindableMemberValue<std::string> DeviceManual;
  BindableMemberValue<std::string> DeviceClass;
  BindableMemberValue<std::string> SerialNumber;
  BindableMemberValue<std::string> ProductInstanceUri;
  BindableMemberValue<std::int32_t> RevisionCounter;
};

}  // namespace di

REFL_TYPE(di::IVendorNameplate_t, UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsDIUri, UA_DIID_IVENDORNAMEPLATETYPE)))
REFL_FIELD(
  Manufacturer,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsDIUri, UA_DIID_IVENDORNAMEPLATETYPE_MANUFACTURER)))
REFL_FIELD(
  ManufacturerUri,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsDIUri, UA_DIID_IVENDORNAMEPLATETYPE_MANUFACTURERURI)))
REFL_FIELD(
  Model,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsDIUri, UA_DIID_IVENDORNAMEPLATETYPE_MODEL)))
REFL_FIELD(
  ProductCode,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsDIUri, UA_DIID_IVENDORNAMEPLATETYPE_PRODUCTCODE)))
REFL_FIELD(
  HardwareRevision,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsDIUri, UA_DIID_IVENDORNAMEPLATETYPE_HARDWAREREVISION)))
REFL_FIELD(
  SoftwareRevision,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsDIUri, UA_DIID_IVENDORNAMEPLATETYPE_SOFTWAREREVISION)))
REFL_FIELD(
  DeviceRevision,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsDIUri, UA_DIID_IVENDORNAMEPLATETYPE_DEVICEREVISION)))
REFL_FIELD(
  DeviceManual,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsDIUri, UA_DIID_IVENDORNAMEPLATETYPE_DEVICEMANUAL)))
REFL_FIELD(
  DeviceClass,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsDIUri, UA_DIID_IVENDORNAMEPLATETYPE_DEVICECLASS)))
REFL_FIELD(
  SerialNumber,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsDIUri, UA_DIID_IVENDORNAMEPLATETYPE_SERIALNUMBER)))
REFL_FIELD(
  ProductInstanceUri,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsDIUri, UA_DIID_IVENDORNAMEPLATETYPE_PRODUCTINSTANCEURI)))
REFL_FIELD(
  RevisionCounter,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsDIUri, UA_DIID_IVENDORNAMEPLATETYPE_REVISIONCOUNTER)))
REFL_END
