///
/// \file ConvertStructValue.cpp
/// \author Christian von Arnim
/// \date 11.02.2020
///

#include "ConvertStructValue.hpp"
#include <open62541/server.h>

namespace UmatiServerLib
{

  copyToVariantFunc ConvertStructValue::GetToVariantFunc(void *pVariable, const UA_DataType *typeDefinition)
  {
    return [pVariable, typeDefinition](UA_Variant *dst) {
      UA_Variant_setScalarCopy(dst, pVariable, typeDefinition);
    };
  }

  void ConvertStructValue::copyValue(const std::string &src, void *memberPtr, const UA_DataType *typeDefinition)
  {
    open62541Cpp::UA_String str(src);
    UA_copy(str.String, memberPtr, &UA_TYPES[UA_TYPES_STRING]);
  }

} // namespace UmatiServerLib
