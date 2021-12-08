/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2021 (c) Christoph Ruckstetter, Michael Weinig AG
 */

#pragma once
#include "../../UmatiServerLib/BindableMemberValue.hpp"
#include "../TypeDefinition.hpp"
#include "../ns0/BaseObject.hpp"
#include "../ns0/Constants.hpp"
#include "Constants.hpp"

namespace woodworking {

struct IWwEventMessage_t {
  BindableMemberValue<UA_WwMessageArgumentDataType> Arguments;
  BindableMemberValue<UA_WwEventCategoryEnumeration> EventCategory;
  BindableMemberValue<std::string> Group;
  BindableMemberValue<UmatiServerLib::LocalizedText_t> LocalizedMessages;
  BindableMemberValue<std::string> MessageId;
  BindableMemberValue<std::string> MessageName;
  BindableMemberValue<std::string> PathParts;
};

}  // namespace woodworking

REFL_TYPE(
  woodworking::IWwEventMessage_t,
  UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_IWWEVENTMESSAGETYPE)))
REFL_FIELD(
  Arguments,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_IWWEVENTMESSAGETYPE_ARGUMENTS)))
REFL_FIELD(EventCategory)
REFL_FIELD(
  Group,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_IWWEVENTMESSAGETYPE_GROUP)))
REFL_FIELD(
  LocalizedMessages,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(
    UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_IWWEVENTMESSAGETYPE_LOCALIZEDMESSAGES)))
REFL_FIELD(MessageId)
REFL_FIELD(
  MessageName,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(
    UmatiServerLib::constexp::NodeId(constants::NsWoodworkingUri, UA_WOODWORKINGID_IWWEVENTMESSAGETYPE_MESSAGENAME)))
REFL_FIELD(PathParts)
REFL_END
