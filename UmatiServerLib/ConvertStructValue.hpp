///
/// \file ConvertStructValue.hpp
/// \author Christian von Arnim
/// \date 11.02.2020
///

#pragma once
#include <refl.hpp>
#include "NodeValue.hpp"
#include <open62541/types.h>
#include "UaTypeHelper.hpp"
#include "ConvertSimpleValue.hpp"
#include <open62541/types_generated_handling.h>
#include <Open62541Cpp/UA_String.hpp>
#include <iostream>
#include "../OpcUaTypes/ReflectionHelper.hpp"

namespace UmatiServerLib
{
  /**
   * @brief (static) Function set which allows copy from an C++ structural Value to OPC UA value
   * 
   * Compares names of OPC UA DataType members to C++ structure names and then write them to a UA_Variant
   * Internally raw memory is copied
   */
  class ConvertStructValue
  {
  public:
    template <typename T>
    static copyToVariantFunc GetToVariantFunc(T &variable, const UA_DataType *typeDefinition);

    template <typename T>
    static copyToVariantFunc GetToVariantFunc(std::vector<T> *pValue, const UA_DataType *typeDefinition);

    static copyToVariantFunc GetToVariantFunc(void *pVariable, const UA_DataType *typeDefinition);

  protected:
    template <typename T>
    static void setByRefl(const T &src, void *trg, const UA_DataType *typeDefinition);

    template <typename T>
    static void copyValue(const T &src, void *memberPtr, const UA_DataType *typeDefinition);

    static void copyValue(const std::string &src, void *memberPtr, const UA_DataType *typeDefinition);

    template <typename T>
    static void convertToVariantRefl(T *pVariable, const UA_DataType *typeDefinition, UA_Variant *dst);
  };

  template <typename T>
  copyToVariantFunc ConvertStructValue::GetToVariantFunc(std::vector<T> *pValue, const UA_DataType *typeDefinition)
  {
    return [pValue, typeDefinition](UA_Variant *dst) {
      UA_Variant_setArrayCopy(dst, &(*pValue)[0], pValue->size(), typeDefinition);
    };
  }

  template <typename T>
  void ConvertStructValue::copyValue(const T &src, void *memberPtr, const UA_DataType *typeDefinition)
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

  template <typename T>
  void ConvertStructValue::setByRefl(const T &src, void *trg, const UA_DataType *typeDefinition)
  {
    const UA_DataType *pTypeDefinition = typeDefinition;

    if constexpr (refl::descriptor::has_attribute<UmatiServerLib::attribute::UaDataType>(refl::reflect<T>()))
    {
      pTypeDefinition = refl::descriptor::get_attribute<UmatiServerLib::attribute::UaDataType>(refl::reflect<T>()).pDataType;
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
      copyValue(reflMember(src), memberPtr, &UA_TYPES[uaMember.memberTypeIndex]);
    });
  }

  template <typename T>
  void ConvertStructValue::convertToVariantRefl(T *pVariable, const UA_DataType *typeDefinition, UA_Variant *dst)
  {

    void *structData = UA_new(typeDefinition);
    UA_init(structData, typeDefinition);
    setByRefl(*pVariable, structData, typeDefinition);

    UA_Variant_setScalar(dst, structData, typeDefinition);
  }

  template <typename T>
  copyToVariantFunc ConvertStructValue::GetToVariantFunc(T &variable, const UA_DataType *typeDefinition)
  {
    auto pVariable = &variable;
    return [pVariable, typeDefinition](UA_Variant *dst) {
      convertToVariantRefl(pVariable, typeDefinition, dst);
    };
  }

} // namespace UmatiServerLib
