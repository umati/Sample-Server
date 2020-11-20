#pragma once
#include "../TypeDefinition.hpp"
#include "Constants.hpp"

namespace di
{

struct ITagNameplate_t
{
  BindableMemberValue<std::string> AssetId;
  BindableMemberValue<UmatiServerLib::LocalizedText_t> ComponentName;
};

} // namespace di

REFL_TYPE(di::ITagNameplate_t,
          UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsDIUri, UA_DIID_ITAGNAMEPLATETYPE)))
REFL_FIELD(AssetId, UmatiServerLib::attribute::PlaceholderOptional(),
          UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsDIUri, UA_DIID_ITAGNAMEPLATETYPE_ASSETID)))
REFL_FIELD(ComponentName, UmatiServerLib::attribute::PlaceholderOptional(),
          UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsDIUri, UA_DIID_ITAGNAMEPLATETYPE_COMPONENTNAME)))
REFL_END
