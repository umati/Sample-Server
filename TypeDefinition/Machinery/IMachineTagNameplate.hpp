#pragma once
#include "../DI/ITagNameplate.hpp"
#include "../TypeDefinition.hpp"
#include "Constants.hpp"

namespace machinery {
struct IMachineTagNameplate_t : public di::ITagNameplate_t {
  BindableMemberValue<std::string> Location;
};

}  // namespace machinery

REFL_TYPE(
  machinery::IMachineTagNameplate_t,
  Bases<di::ITagNameplate_t>(),
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsMachineryUri, UA_MACHINERYID_IMACHINETAGNAMEPLATETYPE)))
REFL_FIELD(
  Location,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsMachineryUri, UA_MACHINERYID_IMACHINETAGNAMEPLATETYPE_LOCATION)))
REFL_END
