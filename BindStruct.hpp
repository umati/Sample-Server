///
/// \file BindStruct.hpp
/// \author Christian von Arnim
/// \date 11.02.2020
///

#pragma once
#include "NodeValue.hpp"
#include <open62541/types.h>
#include "BindValue.hpp"
#include "UaTypeHelper.hpp"
#include <open62541/types_generated_handling.h>
#include <Open62541Cpp/UA_String.hpp>
#include <refl.hpp>
#include <iostream>
#include "OpcUaTypes/ReflectionHelper.hpp"

copyToVariantFunc bindStruct(void *pVariable, const UA_DataType *typeDefinition);

namespace internal_bindStruct
{

template <typename T>
static void setByRefl(const T &src, void *trg, const UA_DataType *typeDefinition);

template <typename T>
inline void copyValue(const T &src, void *memberPtr, const UA_DataType *typeDefinition)
{
  if constexpr (!std::is_fundamental<T>::value && refl::trait::is_reflectable<decltype(src)>::value)
  {
    setByRefl(src, memberPtr, typeDefinition);
  }
  else
  {
    UA_copy(&src, memberPtr, typeDefinition);
  }
}

template <>
inline void copyValue(const std::string &src, void *memberPtr, const UA_DataType *typeDefinition)
{
  open62541Cpp::UA_String str(src);
  UA_copy(str.String, memberPtr, &UA_TYPES[UA_TYPES_STRING]);
}

template <typename T>
static void setByRefl(const T &src, void *trg, const UA_DataType *typeDefinition)
{

  const UA_DataType *pTypeDefinition = typeDefinition;

  if constexpr (refl::descriptor::has_attribute<open62541Cpp::attribute::UaDataType>(refl::reflect<T>()))
  {
    pTypeDefinition = refl::descriptor::get_attribute<open62541Cpp::attribute::UaDataType>(refl::reflect<T>()).pDataType;
  }

  std::size_t numMembersSet = 0;
  for_each(refl::reflect(src).members, [&](auto reflMember) {
    std::size_t uaMemberIndex = -1;
    std::size_t ptrIncrement = 0;
    for (std::size_t i = 0; i < pTypeDefinition->membersSize; ++i)
    {
      auto &uaMember = pTypeDefinition->members[i];
      ptrIncrement += uaMember.padding;
      if (uaMember.memberName == std::string(reflMember.name))
      {
        uaMemberIndex = i;
        break;
      }

      ptrIncrement += UA_TYPES[uaMember.memberTypeIndex].memSize;
    }

    if (uaMemberIndex == -1)
    {
      std::cout << "Could not find UA member for '" << reflMember.name << "' in '" << refl::reflect(src).name << "'"
                << std::endl;
      return;
    }

    void *memberPtr = reinterpret_cast<std::uint8_t *>(trg) + ptrIncrement;
    auto &uaMember = pTypeDefinition->members[uaMemberIndex];
    internal_bindStruct::copyValue(reflMember(src), memberPtr, &UA_TYPES[uaMember.memberTypeIndex]);
  });
}

template <typename T, typename NATIV_T>
static void setNativ(const T &src, void *trg)
{
  static_assert(refl::is_reflectable<T>());
  static_assert(refl::is_reflectable<NATIV_T>());
}

template <typename T>
static void convertToVariantRefl(T *pVariable, const UA_DataType *typeDefinition, UA_Variant* dst)
{

  void *structData = UA_new(typeDefinition);
  UA_init(structData, typeDefinition);
  setByRefl(*pVariable, structData, typeDefinition);

  UA_Variant_setScalar(dst, structData, typeDefinition);
}

}; // namespace internal_bindStruct

template <typename T>
copyToVariantFunc bindStructRefl(T &variable, const UA_DataType *typeDefinition)
{
  auto pVariable = &variable;
  return [pVariable, typeDefinition] (UA_Variant* dst) {
    internal_bindStruct::convertToVariantRefl(pVariable, typeDefinition, dst);
  };
}
