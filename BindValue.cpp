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

void bindValue(NodeValue &nodeValue, primitivTypes_t variable)
{
  auto pDataType = std::visit(primitivTypeVisitor_getTypePointer(), variable);

  auto pVariable = std::visit(primitivTypeVisitor_getPointer(), variable);
  nodeValue = [pVariable, pDataType] {
    UA_Variant value;
    UA_Variant_setScalarCopy(&value, pVariable, pDataType);
    return value;
  };
}

void bindValue(NodeValue &nodeValue, std::string *variable)
{
  auto pVariable = variable;
  nodeValue = [pVariable] {
    open62541Cpp::UA_String str(*pVariable);
    UA_Variant value;
    UA_Variant_setScalarCopy(&value, str.String, &UA_TYPES[UA_TYPES_STRING]);
    return value;
  };
}

typedef std::ratio<1, 10000000> nano_100;
typedef std::chrono::duration<std::int64_t, nano_100> nanoseconds_100;

void bindValue(NodeValue &nodeValue, open62541Cpp::DateTime_t *variable)
{
  auto pVariable = variable;
  nodeValue = [pVariable] {
    auto var_nano100 = std::chrono::duration_cast<nanoseconds_100>(pVariable->time_since_epoch());

    UA_DateTime var = var_nano100.count() + UA_DATETIME_UNIX_EPOCH;
    UA_Variant value;
    UA_Variant_setScalarCopy(&value, &var, &UA_TYPES[UA_TYPES_DATETIME]);
    return value;
  };
}

void bindValue(NodeValue &nodeValue, bool *variable)
{
  auto pVariable = variable;
  nodeValue = [pVariable] {
    UA_Boolean b;
    // Ensure OPC UA encoding
    b = *pVariable? UA_TRUE : UA_FALSE;
    UA_Variant value;
    UA_Variant_setScalarCopy(&value, &b, &UA_TYPES[UA_TYPES_BOOLEAN]);
    return value;
  };
}
