/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2019-2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 * Copyright 2022 (c) Sebastian Friedl, ISW University of Stuttgart (for VDMA e.V.)
 */

#include "ConvertSimpleValue.hpp"

#include <open62541/types_generated.h>

#include <Open62541Cpp/UA_String.hpp>
#include <iostream>

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

copyToVariantFunc ConvertSimpleValue::asVariantFuncArray(std::vector<std::string> *variable) {
  auto pVariable = variable;
  return [pVariable](UA_Variant *dst) {
    UA_String *tmp = new UA_String[pVariable->size()];
    for (size_t i = 0; i < pVariable->size(); ++i) {
      tmp[i] = UA_String_fromChars(pVariable->at(i).c_str());
    }
    UA_Variant_setArray(dst, tmp, pVariable->size(), &UA_TYPES[UA_TYPES_STRING]);
  };
}

copyToVariantFunc ConvertSimpleValue::asVariantFuncArray(std::vector<UmatiServerLib::LocalizedText_t> *variable) {
  auto pVariable = variable;
  return [pVariable](UA_Variant *dst) {
    UA_LocalizedText *tmp = new UA_LocalizedText[pVariable->size()];
    for (size_t i = 0; i < pVariable->size(); ++i) {
      tmp[i] = UA_LOCALIZEDTEXT_ALLOC(pVariable->at(i).locale.c_str(), pVariable->at(i).text.c_str());
    }
    UA_Variant_setArray(dst, tmp, pVariable->size(), &UA_TYPES[UA_TYPES_LOCALIZEDTEXT]);
  };
}

copyToVariantFunc ConvertSimpleValue::asVariantFuncArray(std::vector<std::int32_t> *variable) {
  auto pVariable = variable;
  return [pVariable](UA_Variant *dst) {
    UA_Int32 *tmp = new UA_Int32[pVariable->size()];
    for (size_t i = 0; i < pVariable->size(); ++i) {
      tmp[i] = pVariable->at(i);
    }
    UA_Variant_setArray(dst, tmp, pVariable->size(), &UA_TYPES[UA_TYPES_INT32]);
  };
}

copyToVariantFunc ConvertSimpleValue::asVariantFuncArray(std::vector<std::uint32_t> *variable) {
  auto pVariable = variable;
  return [pVariable](UA_Variant *dst) {
    UA_UInt32 *tmp = new UA_UInt32[pVariable->size()];
    for (size_t i = 0; i < pVariable->size(); ++i) {
      tmp[i] = pVariable->at(i);
    }
    UA_Variant_setArray(dst, tmp, pVariable->size(), &UA_TYPES[UA_TYPES_UINT32]);
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
