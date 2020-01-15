///
/// \file RelativPathBase.hpp
/// \author Christian von Arnim
/// \date 15.01.2020
///

#pragma once

#include "RelativPathElement.hpp"
#include <list>

class RelativPathBase {
  std::list<RelativPathElement> relElements;

 public:
  RelativPathBase(std::list<RelativPathElement> base = std::list<RelativPathElement>());

  std::list<RelativPathElement> operator()(const RelativPathElement &el) const;
};
