///
/// \file ReflectionHelper.hpp
/// \author Christian von Arnim
/// \date 12.02.2020
///

#pragma once

#include <refl.hpp>
#include <open62541/types.h>

#define OPEN62541CPP_PADDING(TYPE, MEMBER, PREVMEMBER) (offsetof(TYPE,MEMBER) - offsetof(TYPE,PREVMEMBER) - sizeof(TYPE::MEMBER))

namespace open62541Cpp::attribute {
  struct UaDataType : refl::attr::usage::type{
   //public:
    const UA_DataType *pDataType;
    constexpr UaDataType(const UA_DataType *pdataType) : pDataType(pdataType){}
  };
}
