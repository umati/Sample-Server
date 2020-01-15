///
/// \file RelativPathElement.hpp
/// \author Christian von Arnim
/// \date 14.01.2020
///

#pragma once
#include <Open62541Cpp/UA_String.hpp>
#include <open62541/types.h>
#include <open62541/types_generated.h>
#include <string>

class RelativPathElement {
  RelativPathElement() = default;
 public:
  UA_RelativePathElement *relEl = nullptr;
  ~RelativPathElement();

  static const UA_NodeId HierarchicalReferences;

  RelativPathElement(const RelativPathElement &other);

  RelativPathElement(UA_UInt16 nsIndex,
                     std::string name,
                     const UA_NodeId referenceTypeId = HierarchicalReferences,
                     bool includeSubtypes = true,
                     bool isInverse = false);
};
