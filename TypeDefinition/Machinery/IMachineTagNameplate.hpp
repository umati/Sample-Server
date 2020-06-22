#pragma once
#include "../TypeDefiniton.hpp"
#include "Constants.hpp"
#include "../DI/ITagNameplate.hpp"

namespace machinery
{
struct IMachineTagNameplate_t : public di::ITagNameplate_t
{
  BindableMemberValue<std::string> Location;
};

} // namespace machinery

REFL_TYPE(machinery::IMachineTagNameplate_t,
          Bases<di::ITagNameplate_t>(),
          open62541Cpp::attribute::UaObjectType{
              .NodeId = open62541Cpp::constexp::NodeId(constants::NsMachineryUri, UA_MACHINERY_ID_IMACHINETAGNAMEPLATETYPE)})
REFL_FIELD(Location, open62541Cpp::attribute::PlaceholderOptional(),
          open62541Cpp::attribute::MemberInTypeNodeId{
              .NodeId = open62541Cpp::constexp::NodeId(constants::NsMachineryUri, UA_MACHINERY_ID_IMACHINETAGNAMEPLATETYPE_LOCATION)})
REFL_END
