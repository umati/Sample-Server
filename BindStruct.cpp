///
/// \file BindStruct.cpp
/// \author Christian von Arnim
/// \date 11.02.2020
///

#include "BindStruct.hpp"
#include <open62541/server.h>

copyToVariantFunc bindStruct(void *pVariable, const UA_DataType *typeDefinition) {

  return [pVariable, typeDefinition] (UA_Variant* dst) {
    UA_Variant_setScalarCopy(dst, pVariable, typeDefinition);
  };
}
