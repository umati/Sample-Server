#pragma once
#include "../TypeDefiniton.hpp"
#include "Constants.hpp"

namespace di
{

struct ITagNameplate_t
{
  BindableMemberValue<std::string> AssetId;
  BindableMemberValue<open62541Cpp::LocalizedText_t> ComponentName;
};

} // namespace di

REFL_TYPE(di::ITagNameplate_t,
          open62541Cpp::attribute::UaObjectType{
              .NodeId = open62541Cpp::constexp::NodeId(constants::NsDIUri, UA_DIID_ITAGNAMEPLATETYPE)})
REFL_FIELD(AssetId, open62541Cpp::attribute::PlaceholderOptional())
REFL_FIELD(ComponentName, open62541Cpp::attribute::PlaceholderOptional())
REFL_END
