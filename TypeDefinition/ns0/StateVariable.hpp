/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2020-2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 */

#pragma once
#include "../TypeDefinition.hpp"
#include "Constants.hpp"

namespace ns0 {

template <typename T>
struct StateVariable_t {
  BindableMemberValue<UmatiServerLib::LocalizedText_t> Value;
  BindableMemberValue<UmatiServerLib::LocalizedText_t> EffectiveDisplayName;
  BindableMemberValue<T> Id;
  BindableMemberValue<UA_QualifiedName> Name;
  BindableMemberValue<UA_UInt32> Number;
};

}  // namespace ns0

REFL_TEMPLATE(
  (typename T),
  (ns0::StateVariable_t<T>),
  UmatiServerLib::attribute::UaVariableType(UmatiServerLib::constexp::NodeId(constants::Ns0Uri, UA_NS0ID_STATEVARIABLETYPE)))
REFL_FIELD(Id)
REFL_FIELD(Value, UmatiServerLib::attribute::UaVariableTypeValue())
REFL_FIELD(Number, UmatiServerLib::attribute::PlaceholderOptional())
REFL_FIELD(Name, UmatiServerLib::attribute::PlaceholderOptional())
REFL_FIELD(EffectiveDisplayName, UmatiServerLib::attribute::PlaceholderOptional())
REFL_END
