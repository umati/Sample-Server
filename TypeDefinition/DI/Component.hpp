#pragma once
#include "../../OpcUaTypes/LocalizedText.hpp"
#include "../TypeDefinition.hpp"
#include "Constants.hpp"
#include "TopologyElement.hpp"

namespace di {

struct Component_t : public TopologyElement_t {
  BindableMemberValue<std::string> AssetId;
  BindableMemberValue<UmatiServerLib::LocalizedText_t> ComponentName;
  BindableMemberValue<std::string> DeviceClass;
  BindableMemberValue<std::string> DeviceManual;
  BindableMemberValue<std::string> DeviceRevision;
  BindableMemberValue<std::string> HardwareRevision;
  BindableMemberValue<UmatiServerLib::LocalizedText_t> Manufacturer;
  BindableMemberValue<std::string> ManufacturerUri;
  BindableMemberValue<UmatiServerLib::LocalizedText_t> Model;
  BindableMemberValue<std::string> ProductCode;
  BindableMemberValue<std::string> ProductInstanceUri;
  BindableMemberValue<std::int32_t> RevisionCounter;
  BindableMemberValue<std::string> SerialNumber;
  BindableMemberValue<std::string> SoftwareRevision;
};

}  // namespace di

REFL_TYPE(
  di::Component_t,
  Bases<di::TopologyElement_t>(),
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsDIUri, UA_DIID_COMPONENTTYPE)))
REFL_FIELD(
  AssetId,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsDIUri, UA_DIID_COMPONENTTYPE_ASSETID)))
REFL_FIELD(
  ComponentName,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsDIUri, UA_DIID_COMPONENTTYPE_COMPONENTNAME)))
REFL_FIELD(
  DeviceClass,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsDIUri, UA_DIID_COMPONENTTYPE_DEVICECLASS)))
REFL_FIELD(
  DeviceManual,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsDIUri, UA_DIID_COMPONENTTYPE_DEVICEMANUAL)))
REFL_FIELD(
  DeviceRevision,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsDIUri, UA_DIID_COMPONENTTYPE_DEVICEREVISION)))
REFL_FIELD(
  HardwareRevision,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsDIUri, UA_DIID_COMPONENTTYPE_HARDWAREREVISION)))
REFL_FIELD(
  Manufacturer,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsDIUri, UA_DIID_COMPONENTTYPE_MANUFACTURER)))
REFL_FIELD(
  ManufacturerUri,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsDIUri, UA_DIID_COMPONENTTYPE_MANUFACTURERURI)))
REFL_FIELD(
  Model,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsDIUri, UA_DIID_COMPONENTTYPE_MODEL)))
REFL_FIELD(
  ProductCode,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsDIUri, UA_DIID_COMPONENTTYPE_PRODUCTCODE)))
REFL_FIELD(
  ProductInstanceUri,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsDIUri, UA_DIID_COMPONENTTYPE_PRODUCTINSTANCEURI)))
REFL_FIELD(
  RevisionCounter,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsDIUri, UA_DIID_COMPONENTTYPE_REVISIONCOUNTER)))
REFL_FIELD(
  SerialNumber,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsDIUri, UA_DIID_COMPONENTTYPE_SERIALNUMBER)))
REFL_FIELD(
  SoftwareRevision,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsDIUri, UA_DIID_COMPONENTTYPE_SOFTWAREREVISION)))
REFL_END
