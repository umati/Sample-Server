/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2020-2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 */

#include "EUInformation.hpp"

#include "LocalizedText.hpp"

namespace UmatiServerLib {

EUInformation_t EUInformation_t::fromUa(const ::UA_EUInformation &other) {
  EUInformation_t ret{
    .NamespaceUri = std::string((const char *)other.namespaceUri.data, other.namespaceUri.length),
    .UnitId = other.unitId,
    .DisplayName = LocalizedText_t::fromUa(other.displayName),
    .Description = LocalizedText_t::fromUa(other.description),
  };
  return ret;
}

bool EUInformation_t::operator==(const EUInformation_t &other) const {
  return this->NamespaceUri == other.NamespaceUri && this->UnitId == other.UnitId && this->Description == other.Description &&
         this->DisplayName == other.DisplayName;
}

}  // namespace UmatiServerLib
