///
/// \file RelativPathElement.cpp
/// \author Christian von Arnim
/// \date 14.01.2020
///

#include <open62541/types.h>
#include <open62541/server_config_default.h>
#include "RelativPathElement.hpp"
#include <open62541/types_generated_handling.h>

const UA_NodeId RelativPathElement::HierarchicalReferences{
    .namespaceIndex = 0,
    .identifierType=UA_NODEIDTYPE_NUMERIC,
    .identifier = {.numeric = UA_NS0ID_HIERARCHICALREFERENCES}
};

RelativPathElement::~RelativPathElement() {
  if(relEl != nullptr)
  {
    UA_RelativePathElement_deleteMembers(relEl);
    UA_RelativePathElement_delete(relEl);
  }
}
RelativPathElement::RelativPathElement(const RelativPathElement &other) {
  relEl = UA_RelativePathElement_new();
  UA_RelativePathElement_copy(other.relEl, relEl);
}
RelativPathElement::RelativPathElement(UA_UInt16 nsIndex,
                                       std::string name,
                                       const UA_NodeId referenceTypeId,
                                       bool includeSubtypes,
                                       bool isInverse) {
  relEl = UA_RelativePathElement_new();

  relEl->includeSubtypes = includeSubtypes;
  relEl->isInverse = isInverse;
  UA_NodeId_copy(&referenceTypeId, &relEl->referenceTypeId);
  UA_String_copy(open62541Cpp::UA_String(name).String, &relEl->targetName.name);
  relEl->targetName.namespaceIndex = nsIndex;
}
