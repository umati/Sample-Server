///
/// \file bindVlaue.cpp
/// \author Christian von Arnim
/// \date 17.12.2019
///

#include "NodeValue.hpp"
#include <open62541/types_generated.h>
#include <open62541Cpp/UA_String.hpp>
#include "BindValue.hpp"
#include "UaTypeHelper.hpp"


void bindValue(NodeValue &nodeValue, primitivTypes_t variable){
  auto pDataType = std::visit(primitivTypeVisitor_getTypePointer(), variable);

  auto pVariable = std::visit(primitivTypeVisitor_getPointer(), variable);
  nodeValue = [pVariable, pDataType] {
    UA_Variant value;
    UA_Variant_setScalarCopy(&value, pVariable, pDataType);
    return value;
  };
}

void bindValue(NodeValue &nodeValue, std::string *variable){
  auto pVariable = variable;
  nodeValue = [pVariable] {
    open62541Cpp::UA_String str(*pVariable);
    UA_Variant value;
    UA_Variant_setScalarCopy(&value, str.String, &UA_TYPES[UA_TYPES_STRING]);
    return value;
  };
}
