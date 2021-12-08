/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2020-2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 * Copyright 2020 (c) Tonja Heinemann, ISW University of Stuttgart (for umati and VDW e.V.)
 */

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
