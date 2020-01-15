///
/// \file RelativPathBase.cpp
/// \author Christian von Arnim
/// \date 15.01.2020
///

#include "RelativPathBase.hpp"

RelativPathBase::RelativPathBase(std::list<RelativPathElement> base)
    : relElements(base) {

}
std::list<RelativPathElement> RelativPathBase::operator()(const RelativPathElement &el) const {
  auto ret = relElements;
  ret.push_back(el);
  return ret;
}
