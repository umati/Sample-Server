/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2020-2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 */

#pragma once

#include <Open62541Cpp/UA_RelativPathElement.hpp>
#include <optional>

#include "BindableMember.hpp"

template <typename T>
class BindableMemberValue : public BindableMember<T> {
 public:
  BindableMemberValue() = default;
  explicit BindableMemberValue(const T &val);
  UA_StatusCode StatusCode = UA_STATUSCODE_GOOD;
  std::optional<UA_DateTime> SourceTimestamp;
  std::optional<UA_DateTime> ServerTimestamp;

  BindableMemberValue<T> &operator=(const T &other) {
    this->value = other;
    return *this;
  }
};

template <typename T>
BindableMemberValue<T>::BindableMemberValue(const T &val) : BindableMember<T>(val) {}
