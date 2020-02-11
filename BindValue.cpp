///
/// \file bindVlaue.cpp
/// \author Christian von Arnim
/// \date 17.12.2019
///

#include "NodeValue.hpp"
#include <open62541/types.h>
#include <open62541/types_generated.h>
#include <open62541Cpp/UA_String.hpp>
#include "BindValue.hpp"


struct primitivTypeVisitor_getPointer{

  const void* operator()(std::int8_t* p)
  {
    return p;
  }

  const void* operator()(std::int16_t* p)
  {
    return p;
  }

  const void* operator()(std::int32_t* p)
  {
    return p;
  }

  const void* operator()(std::int64_t* p)
  {
    return p;
  }

  const void* operator()(std::uint8_t* p)
  {
    return p;
  }

  const void* operator()(std::uint16_t* p)
  {
    return p;
  }

  const void* operator()(std::uint32_t* p)
  {
    return p;
  }

  const void* operator()(std::uint64_t* p)
  {
    return p;
  }

  const void* operator()(float* p)
  {
    return p;
  }

  const void* operator()(double* p)
  {
    return p;
  }

};

struct primitivTypeVisitor_getTypePointer{

  const UA_DataType* operator()(std::int8_t*)
  {
    return &UA_TYPES[UA_TYPES_SBYTE];
  }

  const UA_DataType* operator()(std::int16_t*)
  {
    return &UA_TYPES[UA_TYPES_INT16];
  }

  const UA_DataType* operator()(std::int32_t*)
  {
    return &UA_TYPES[UA_TYPES_INT32];
  }

  const UA_DataType* operator()(std::int64_t*)
  {
    return &UA_TYPES[UA_TYPES_INT64];
  }

  const UA_DataType* operator()(std::uint8_t*)
  {
    return &UA_TYPES[UA_TYPES_BYTE];
  }

  const UA_DataType* operator()(std::uint16_t*)
  {
    return &UA_TYPES[UA_TYPES_UINT16];
  }

  const UA_DataType* operator()(std::uint32_t*)
  {
    return &UA_TYPES[UA_TYPES_UINT32];
  }

  const UA_DataType* operator()(std::uint64_t*)
  {
    return &UA_TYPES[UA_TYPES_UINT64];
  }

  const UA_DataType* operator()(float*)
  {
    return &UA_TYPES[UA_TYPES_FLOAT];
  }

  const UA_DataType* operator()(double*)
  {
    return &UA_TYPES[UA_TYPES_DOUBLE];
  }

};

void bindValue(NodeValue &nodeValue, primitivTypes_t variable){
  auto pDataType = std::visit(primitivTypeVisitor_getTypePointer(), variable);

  auto pVariable = std::visit(primitivTypeVisitor_getPointer(), variable);
  nodeValue = [pVariable, pDataType] {
    UA_Variant value;
    UA_Variant_setScalarCopy(&value, pVariable, pDataType);
    return value;
  };
}

/*
void bindValue(NodeValue &nodeValue, std::uint32_t &variable){
  auto pVariable = &variable;
  nodeValue = [pVariable] {
    UA_Variant value;
    UA_Variant_setScalarCopy(&value, pVariable, &UA_TYPES[UA_TYPES_UINT32]);
    return value;
  };
}*/

void bindValue(NodeValue &nodeValue, std::string *variable){
  auto pVariable = variable;
  nodeValue = [pVariable] {
    open62541Cpp::UA_String str(*pVariable);
    UA_Variant value;
    UA_Variant_setScalarCopy(&value, str.String, &UA_TYPES[UA_TYPES_STRING]);
    return value;
  };
}
