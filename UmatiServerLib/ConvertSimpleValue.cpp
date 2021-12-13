/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2019-2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 */

#include "ConvertSimpleValue.hpp"

#include <open62541/types_generated.h>

#include <Open62541Cpp/UA_String.hpp>

#include "NodeValue.hpp"
#include "UaTypeHelper.hpp"

namespace UmatiServerLib {

copyToVariantFunc ConvertSimpleValue::asVariantFunc(primitivTypes_t variable) {
  auto pDataType = std::visit(primitivTypeVisitor_getTypePointer(), variable);
  auto pVariable = std::visit(primitivTypeVisitor_getPointer(), variable);

  return [pVariable, pDataType](UA_Variant *dst) { UA_Variant_setScalarCopy(dst, pVariable, pDataType); };
}

copyToVariantFunc ConvertSimpleValue::asVariantFunc(std::string *variable) {
  auto pVariable = variable;
  return [pVariable](UA_Variant *dst) {
    open62541Cpp::UA_String str(*pVariable);
    UA_Variant_setScalarCopy(dst, str.String, &UA_TYPES[UA_TYPES_STRING]);
  };
}

typedef std::ratio<1, 10000000> nano_100;
typedef std::chrono::duration<std::int64_t, nano_100> nanoseconds_100;

copyToVariantFunc ConvertSimpleValue::asVariantFunc(UmatiServerLib::DateTime_t *variable) {
  auto pVariable = variable;
  return [pVariable](UA_Variant *dst) {
    auto var_nano100 = std::chrono::duration_cast<nanoseconds_100>(pVariable->time_since_epoch());
    UA_DateTime var = var_nano100.count() + UA_DATETIME_UNIX_EPOCH;
    UA_Variant_setScalarCopy(dst, &var, &UA_TYPES[UA_TYPES_DATETIME]);
  };
}

copyToVariantFunc ConvertSimpleValue::asVariantFunc(bool *variable) {
  auto pVariable = variable;
  return [pVariable](UA_Variant *dst) {
    UA_Boolean b;
    // Ensure OPC UA encoding
    b = *pVariable ? UA_TRUE : UA_FALSE;
    UA_Variant_setScalarCopy(dst, &b, &UA_TYPES[UA_TYPES_BOOLEAN]);
  };
}

copyToVariantFunc ConvertSimpleValue::asVariantFunc(UmatiServerLib::StatusCode_t *variable) {
  auto pVariable = variable;
  return [pVariable](UA_Variant *dst) {
    UA_StatusCode s = pVariable->code;
    UA_Variant_setScalarCopy(dst, &s, &UA_TYPES[UA_TYPES_STATUSCODE]);
  };
}

}  // namespace UmatiServerLib
