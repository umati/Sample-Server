#pragma once
#include "../TypeDefiniton.hpp"
#include "Constants.hpp"
#include "ITagNameplate.hpp"

namespace di
{
struct IMachineTagNameplate_t : public ITagNameplate_t
{
  BindableMemberValue<std::string> Location;
};

} // namespace di

REFL_TYPE(di::IMachineTagNameplate_t,
          Bases<di::ITagNameplate_t>(),
          open62541Cpp::attribute::UaObjectType{
              .NodeId = open62541Cpp::constexp::NodeId(constants::NsDIUri, UA_DIID_ITAGNAMEPLATETYPE)})
REFL_FIELD(Location, open62541Cpp::attribute::PlaceholderOptional())
REFL_END
