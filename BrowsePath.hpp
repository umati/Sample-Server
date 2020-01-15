///
/// \file BrowsePath.hpp
/// \author Christian von Arnim
/// \date 14.01.2020
///

#pragma once

#include <open62541/types_generated.h>
#include "RelativPathElement.hpp"
#include <list>

class BrowsePath {
  BrowsePath() = default;
 public:
  UA_BrowsePath *BPath = nullptr;

  ~BrowsePath();
  BrowsePath (const BrowsePath& other);
  BrowsePath(const UA_NodeId startNodeId, std::list<RelativPathElement> pathElements);

};
