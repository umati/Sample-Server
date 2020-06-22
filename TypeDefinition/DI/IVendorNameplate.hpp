#pragma once
#include "../TypeDefiniton.hpp"
#include "Constants.hpp"

namespace di
{

struct IVendorNameplate_t
{
  BindableMemberValue<std::string> ProductInstanceUri;
  BindableMemberValue<open62541Cpp::LocalizedText_t> Model;
  BindableMemberValue<std::string> SoftwareRevision;
  BindableMemberValue<open62541Cpp::LocalizedText_t> Manufacturer;
  BindableMemberValue<std::string> SerialNumber;
  BindableMemberValue<std::string> DeviceClass;
};

} // namespace di

REFL_TYPE(di::IVendorNameplate_t,
          open62541Cpp::attribute::UaObjectType{
              .NodeId = open62541Cpp::constexp::NodeId(constants::NsDIUri, UA_DIID_IVENDORNAMEPLATETYPE)})
REFL_FIELD(ProductInstanceUri, open62541Cpp::attribute::PlaceholderOptional(),
          open62541Cpp::attribute::MemberInTypeNodeId{
              .NodeId = open62541Cpp::constexp::NodeId(constants::NsDIUri, UA_DIID_IVENDORNAMEPLATETYPE_PRODUCTINSTANCEURI)})
REFL_FIELD(Model, open62541Cpp::attribute::PlaceholderOptional(),
          open62541Cpp::attribute::MemberInTypeNodeId{
              .NodeId = open62541Cpp::constexp::NodeId(constants::NsDIUri, UA_DIID_IVENDORNAMEPLATETYPE_MODEL)})
REFL_FIELD(SoftwareRevision, open62541Cpp::attribute::PlaceholderOptional(),
          open62541Cpp::attribute::MemberInTypeNodeId{
              .NodeId = open62541Cpp::constexp::NodeId(constants::NsDIUri, UA_DIID_IVENDORNAMEPLATETYPE_SOFTWAREREVISION)})
REFL_FIELD(Manufacturer, open62541Cpp::attribute::PlaceholderOptional(),
          open62541Cpp::attribute::MemberInTypeNodeId{
              .NodeId = open62541Cpp::constexp::NodeId(constants::NsDIUri, UA_DIID_IVENDORNAMEPLATETYPE_MANUFACTURER)})
REFL_FIELD(SerialNumber, open62541Cpp::attribute::PlaceholderOptional(),
          open62541Cpp::attribute::MemberInTypeNodeId{
              .NodeId = open62541Cpp::constexp::NodeId(constants::NsDIUri, UA_DIID_IVENDORNAMEPLATETYPE_SERIALNUMBER)})
REFL_FIELD(DeviceClass, open62541Cpp::attribute::PlaceholderOptional(),
          open62541Cpp::attribute::MemberInTypeNodeId{
              .NodeId = open62541Cpp::constexp::NodeId(constants::NsDIUri, UA_DIID_IVENDORNAMEPLATETYPE_DEVICECLASS)})
REFL_END
