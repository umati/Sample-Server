/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2020-2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 */

#pragma once

#include <open62541/types.h>

#include <refl.hpp>

#define UMATISERVERLIB_PADDING(TYPE, MEMBER, PREVMEMBER) (offsetof(TYPE, MEMBER) - offsetof(TYPE, PREVMEMBER) - sizeof(TYPE::MEMBER))

namespace UmatiServerLib::attribute {
struct UaDataType : refl::attr::usage::type {
  // public:
  const UA_DataType *pDataType;
  constexpr UaDataType(const UA_DataType *pdataType) : pDataType(pdataType) {}
};
}  // namespace UmatiServerLib::attribute
