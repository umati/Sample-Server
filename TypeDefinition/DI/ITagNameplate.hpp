/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2020-2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 * Copyright 2020 (c) Tonja Heinemann, ISW University of Stuttgart (for umati and VDW e.V.)
 */

#pragma once
#include "../TypeDefinition.hpp"
#include "Constants.hpp"

namespace di {

struct ITagNameplate_t {
  BindableMemberValue<std::string> AssetId;
  BindableMemberValue<UmatiServerLib::LocalizedText_t> ComponentName;
};

}  // namespace di

REFL_TYPE(di::ITagNameplate_t, UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(constants::NsDIUri, UA_DIID_ITAGNAMEPLATETYPE)))
REFL_FIELD(
  AssetId,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsDIUri, UA_DIID_ITAGNAMEPLATETYPE_ASSETID)))
REFL_FIELD(
  ComponentName,
  UmatiServerLib::attribute::PlaceholderOptional(),
  UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(constants::NsDIUri, UA_DIID_ITAGNAMEPLATETYPE_COMPONENTNAME)))
REFL_END
