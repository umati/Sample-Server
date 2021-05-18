#pragma once
#include <open62541/server.h>
#include <open62541/types.h>

#include <Open62541Cpp/UA_NodeId.hpp>

namespace UmatiServerLib::constexp {

class NodeId {
 public:
  const char *NsUri = nullptr;
  ::UA_NodeIdType IdentifierType = ::UA_NodeIdType::UA_NODEIDTYPE_NUMERIC;
  union {
    UA_UInt32 numeric;
    const char *string;
    UA_Guid guid;
    const char *byteString;
  } Identifier = {.numeric = 0};

  constexpr NodeId() = default;
  inline constexpr NodeId(const char *nsUri, UA_UInt32 numeric)
    : NsUri(nsUri), IdentifierType(::UA_NodeIdType::UA_NODEIDTYPE_NUMERIC), Identifier{.numeric = numeric} {}
  inline constexpr NodeId(const char *nsUri, UA_Guid guid) : NsUri(nsUri), IdentifierType(::UA_NodeIdType::UA_NODEIDTYPE_GUID), Identifier{.guid = guid} {}

  inline constexpr NodeId(const char *nsUri, ::UA_NodeIdType identifierType, const char *string)
    : NsUri(nsUri), IdentifierType(identifierType), Identifier{.string = string} {}

  open62541Cpp::UA_NodeId UANodeId(UA_Server *pServer) const;
};

}  // namespace UmatiServerLib::constexp
