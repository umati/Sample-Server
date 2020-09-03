
#include <open62541/types.h>

namespace UmatiServerLib::TypeToUaDatatype {
  template <typename T>
  inline const UA_DataType* GetType()
  {
    return nullptr;
  }

  template<>
  inline const UA_DataType* GetType<UA_NodeId>()
  {
    return &UA_TYPES[UA_TYPES_NODEID];
  }
}
