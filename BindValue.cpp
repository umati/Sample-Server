///
/// \file bindVlaue.cpp
/// \author Christian von Arnim
/// \date 17.12.2019
///

#include "NodeValue.hpp"
#include <open62541/types.h>
#include <open62541/types_generated.h>
#include <open62541Cpp/UA_String.hpp>

void bindValue(NodeValue &nodeValue, std::uint32_t &variable){
  auto pVariable = &variable;
  nodeValue = [pVariable] {
    UA_Variant value;
    UA_Variant_setScalarCopy(&value, pVariable, &UA_TYPES[UA_TYPES_UINT32]);
    return value;
  };
}

void bindValue(NodeValue &nodeValue, std::string &variable){
  auto pVariable = &variable;
  nodeValue = [pVariable] {
    open62541Cpp::UA_String str(*pVariable);
    UA_Variant value;
    UA_Variant_setScalarCopy(&value, str.String, &UA_TYPES[UA_TYPES_STRING]);
    return value;
  };
}
