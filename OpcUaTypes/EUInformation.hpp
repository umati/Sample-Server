/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2020-2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 */

#pragma once
#include <open62541/types_generated.h>

#include "LocalizedText.hpp"

namespace UmatiServerLib {
struct EUInformation_t {
  std::string NamespaceUri;
  UA_Int32 UnitId;
  LocalizedText_t DisplayName;
  LocalizedText_t Description;

  static EUInformation_t fromUa(const ::UA_EUInformation &other);
  bool operator==(const EUInformation_t &other) const;
};

}  // namespace UmatiServerLib

REFL_TYPE(UmatiServerLib::EUInformation_t, UmatiServerLib::attribute::UaDataType(&UA_TYPES[UA_TYPES_EUINFORMATION]))
REFL_FIELD(NamespaceUri)
REFL_FIELD(UnitId)
REFL_FIELD(DisplayName)
REFL_FIELD(Description)
REFL_END
