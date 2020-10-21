#pragma once
#include "../TypeDefinition.hpp"
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
          UmatiServerLib::attribute::UaObjectType{
              .NodeId = UmatiServerLib::constexp::NodeId(constants::NsMachineryUri, UA_MACHINERYID_IMACHINETAGNAMEPLATETYPE)})
REFL_FIELD(Location, UmatiServerLib::attribute::PlaceholderOptional(),
          UmatiServerLib::attribute::MemberInTypeNodeId{
              .NodeId = UmatiServerLib::constexp::NodeId(constants::NsMachineryUri, UA_MACHINERYID_IMACHINETAGNAMEPLATETYPE_LOCATION)})
REFL_END
