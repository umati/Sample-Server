/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2020-2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 */

#include "ConvertStructValue.hpp"

#include <open62541/server.h>

namespace UmatiServerLib {

copyToVariantFunc ConvertStructValue::GetToVariantFunc(void *pVariable, const UA_DataType *typeDefinition) {
  return [pVariable, typeDefinition](UA_Variant *dst) { UA_Variant_setScalarCopy(dst, pVariable, typeDefinition); };
}

void ConvertStructValue::copyValue(const std::string &src, void *memberPtr, const UA_DataType *typeDefinition) {
  open62541Cpp::UA_String str(src);
  UA_copy(str.String, memberPtr, &UA_TYPES[UA_TYPES_STRING]);
}

}  // namespace UmatiServerLib
