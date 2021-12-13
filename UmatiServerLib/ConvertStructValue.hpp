/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2020-2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 * Copyright 2021 (c) Götz Görisch, VDW - Verein Deutscher Werkzeugmaschinenfabriken e.V.
 */

#pragma once
#include <open62541/types.h>
#include <open62541/types_generated_handling.h>

#include <Open62541Cpp/UA_String.hpp>
#include <iostream>
#include <refl.hpp>

#include "../OpcUaTypes/ReflectionHelper.hpp"
#include "ConvertSimpleValue.hpp"
#include "NodeValue.hpp"
#include "UaTypeHelper.hpp"

namespace UmatiServerLib {
/**
 * @brief (static) Function set which allows copy from an C++ structural Value to OPC UA value
 *
 * Compares names of OPC UA DataType members to C++ structure names and then write them to a UA_Variant
 * Internally raw memory is copied
 */
class ConvertStructValue {
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
copyToVariantFunc ConvertStructValue::GetToVariantFunc(std::vector<T> *pValue, const UA_DataType *typeDefinition) {
  return [pValue, typeDefinition](UA_Variant *dst) { UA_Variant_setArrayCopy(dst, &(*pValue)[0], pValue->size(), typeDefinition); };
}

template <typename T>
void ConvertStructValue::copyValue(const T &src, void *memberPtr, const UA_DataType *typeDefinition) {
  if constexpr (!std::is_fundamental<T>::value && refl::trait::is_reflectable<decltype(src)>::value) {
    setByRefl(src, memberPtr, typeDefinition);
  } else {
    UA_copy(&src, memberPtr, typeDefinition);
  }
}

template <typename T>
void ConvertStructValue::setByRefl(const T &src, void *trg, const UA_DataType *typeDefinition) {
  const UA_DataType *pTypeDefinition = typeDefinition;

  if constexpr (refl::descriptor::has_attribute<UmatiServerLib::attribute::UaDataType>(refl::reflect<T>())) {
    pTypeDefinition = refl::descriptor::get_attribute<UmatiServerLib::attribute::UaDataType>(refl::reflect<T>()).pDataType;
  }

  std::size_t numMembersSet = 0;
  for_each(refl::reflect(src).members, [&](auto reflMember) {
    std::size_t uaMemberIndex = -1;
    std::size_t ptrIncrement = 0;
    for (std::size_t i = 0; i < pTypeDefinition->membersSize; ++i) {
      auto &uaMember = pTypeDefinition->members[i];
      ptrIncrement += uaMember.padding;
      if (uaMember.memberName == std::string(reflMember.name)) {
        uaMemberIndex = i;
        break;
      }

      ptrIncrement += uaMember.memberType->memSize;
    }

    if (uaMemberIndex == -1) {
      std::cout << "Could not find UA member for '" << reflMember.name << "' in '" << refl::reflect(src).name << "'" << std::endl;
      return;
    }

    void *memberPtr = reinterpret_cast<std::uint8_t *>(trg) + ptrIncrement;
    auto &uaMember = pTypeDefinition->members[uaMemberIndex];
    copyValue(reflMember(src), memberPtr, uaMember.memberType);
  });
}

template <typename T>
void ConvertStructValue::convertToVariantRefl(T *pVariable, const UA_DataType *typeDefinition, UA_Variant *dst) {
  void *structData = UA_new(typeDefinition);
  UA_init(structData, typeDefinition);
  setByRefl(*pVariable, structData, typeDefinition);

  UA_Variant_setScalar(dst, structData, typeDefinition);
}

template <typename T>
copyToVariantFunc ConvertStructValue::GetToVariantFunc(T &variable, const UA_DataType *typeDefinition) {
  auto pVariable = &variable;
  return [pVariable, typeDefinition](UA_Variant *dst) { convertToVariantRefl(pVariable, typeDefinition, dst); };
}

}  // namespace UmatiServerLib
