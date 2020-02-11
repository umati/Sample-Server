///
/// \file BindStruct.cpp
/// \author Christian von Arnim
/// \date 11.02.2020
///

#include "BindStruct.hpp"
#include <open62541/server.h>

void bindStruct(NodeValue &nodeValue, void *pVariable, const UA_DataType *typeDefinition) {

  nodeValue = [pVariable, typeDefinition] {
    UA_Variant value;
    UA_Variant_setScalarCopy(&value, pVariable, typeDefinition);
    return value;
  };
}
