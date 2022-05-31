/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2019-2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 * Copyright 2022 (c) Sebastian Friedl, ISW University of Stuttgart (for VDMA e.V.)
 */

#pragma once

#include <open62541/types.h>

#include <cstdint>
#include <functional>
#include <string>
#include <variant>

#include "../OpcUaTypes/DateTime.hpp"
#include "../OpcUaTypes/StatusCode.hpp"
#include "NodeValue.hpp"

namespace UmatiServerLib {

using primitivTypes_t = std::
  variant<std::int8_t *, std::int16_t *, std::int32_t *, std::int64_t *, std::uint8_t *, std::uint16_t *, std::uint32_t *, std::uint64_t *, float *, double *>;

// UA_Variant* dst
typedef std::function<void(UA_Variant *)> copyToVariantFunc;

class ConvertSimpleValue {
 public:
  static copyToVariantFunc asVariantFunc(primitivTypes_t pVariable);
  static copyToVariantFunc asVariantFunc(std::string *variable);
  static copyToVariantFunc asVariantFuncArray(std::vector<std::string> *variable);
  static copyToVariantFunc asVariantFuncArray(std::vector<std::int32_t> *variable);
  static copyToVariantFunc asVariantFuncArray(std::vector<std::uint32_t> *variable);
  static copyToVariantFunc asVariantFunc(UmatiServerLib::DateTime_t *variable);
  static copyToVariantFunc asVariantFunc(UmatiServerLib::StatusCode_t *variable);
  static copyToVariantFunc asVariantFunc(bool *pVariable);
};

}  // namespace UmatiServerLib
