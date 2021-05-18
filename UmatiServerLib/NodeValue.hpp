///
/// \file NodeValue.hpp
/// \author Christian von Arnim
/// \date 17.12.2019
///

#pragma once

#include <open62541/types.h>

#include <Open62541Cpp/UA_NodeId.hpp>
#include <functional>

/**
 * @brief Class for accessting the value of a single node.
 *
 * Allow setting an callback function, which is called, when the value is needed.
 * @todo Use new External DataValue concept of open62541
 */
class NodeValue {
  typedef std::function<UA_DataValue()> GetValue_t;
  GetValue_t fCallback = nullptr;
  open62541Cpp::UA_NodeId NodeId;
  NodeValue(const NodeValue&) = delete;

 public:
  open62541Cpp::UA_NodeId GetNodeId() { return NodeId; }
  NodeValue(NodeValue&& other);
  NodeValue(const open62541Cpp::UA_NodeId& nodeId);
  // Use variadict expression https://stackoverflow.com/questions/9242234/c11-variadic-stdfunction-parameter
  void operator=(GetValue_t F);

  UA_DataValue Value();
};
