/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2020-2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 */

#include "LocalizedText.hpp"

namespace UmatiServerLib {

LocalizedText_t LocalizedText_t::fromUa(const ::UA_LocalizedText &other) {
  LocalizedText_t ret{
    .locale = std::string((const char *)other.locale.data, other.locale.length), .text = std::string((const char *)other.text.data, other.text.length)};
  return ret;
}

bool LocalizedText_t::operator==(const LocalizedText_t &other) const { return this->locale == other.locale && this->text == other.text; }

}  // namespace UmatiServerLib
