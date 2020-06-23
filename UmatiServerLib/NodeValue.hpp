///
/// \file NodeValue.hpp
/// \author Christian von Arnim
/// \date 17.12.2019
///

#pragma once

#include <open62541/types.h>
#include <functional>
#include <Open62541Cpp/UA_NodeId.hpp>

class NodeValue {
  typedef std::function<UA_DataValue()> GetValue_t;
  GetValue_t fCallback = nullptr;
  open62541Cpp::UA_NodeId NodeId;
  NodeValue(const NodeValue&) = delete;
 public:
  open62541Cpp::UA_NodeId GetNodeId() { return NodeId;}
  NodeValue(NodeValue&& other);
  NodeValue(const open62541Cpp::UA_NodeId &nodeId);
  // Use variadict expression https://stackoverflow.com/questions/9242234/c11-variadic-stdfunction-parameter
  void operator=(GetValue_t F);

  UA_DataValue Value();
};
