///
/// \file NodeValue.hpp
/// \author Christian von Arnim
/// \date 17.12.2019
///

#pragma once

#include <open62541/types.h>
#include <functional>

class NodeValue {
  typedef std::function<UA_Variant()> GetValue_t;
  GetValue_t fCallback;

 public:
  // Use variadict expression https://stackoverflow.com/questions/9242234/c11-variadic-stdfunction-parameter
  void operator=(GetValue_t F);

  UA_Variant Value();
};
