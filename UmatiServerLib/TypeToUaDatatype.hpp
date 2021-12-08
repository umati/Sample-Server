/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2020-2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 */

#include <open62541/types.h>

/**
 * @brief Extendable namespace, which allows using custom UA_DataTypes* for C++ types.
 *
 */
namespace UmatiServerLib::TypeToUaDatatype {
template <typename T>
inline const UA_DataType *GetType() {
  if constexpr (!refl::trait::is_reflectable<T>::value) {
    return nullptr;
  } else {
    if constexpr (refl::descriptor::has_attribute<UmatiServerLib::attribute::UaDataType>(refl::reflect<T>())) {
      return refl::descriptor::get_attribute<UmatiServerLib::attribute::UaDataType>(refl::reflect<T>()).pDataType;
    } else {
      return nullptr;
    }
  }
}

template <>
inline const UA_DataType *GetType<UA_NodeId>() {
  return &UA_TYPES[UA_TYPES_NODEID];
}
}  // namespace UmatiServerLib::TypeToUaDatatype
