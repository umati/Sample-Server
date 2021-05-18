
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
