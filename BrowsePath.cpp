///
/// \file BrowsePath.cpp
/// \author Christian von Arnim
/// \date 14.01.2020
///


#include "BrowsePath.hpp"
#include <open62541/types_generated_handling.h>

BrowsePath::~BrowsePath() {
  if(BPath != nullptr)
  {
    UA_BrowsePath_deleteMembers(BPath);
    UA_BrowsePath_delete(BPath);
  }
}
BrowsePath::BrowsePath(const UA_NodeId startNodeId, std::list<RelativPathElement> pathElements) {
  BPath = UA_BrowsePath_new();
  UA_NodeId_copy(&startNodeId, &BPath->startingNode);
  BPath->relativePath.elementsSize = pathElements.size();
  BPath->relativePath.elements = reinterpret_cast<UA_RelativePathElement *>(UA_Array_new(BPath->relativePath.elementsSize, &UA_TYPES[UA_TYPES_RELATIVEPATHELEMENT]));

  int i = 0;
  for (const auto &el : pathElements) {
    UA_RelativePathElement_copy(el.relEl, &BPath->relativePath.elements[i++]);
  }
}

BrowsePath::BrowsePath(const BrowsePath &other) {
  BPath = UA_BrowsePath_new();
  UA_BrowsePath_copy(other.BPath, BPath);
}
