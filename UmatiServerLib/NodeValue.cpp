///
/// \file NodeValue.cpp
/// \author Christian von Arnim
/// \date 17.12.2019
///

#include "NodeValue.hpp"

void NodeValue::operator=(NodeValue::GetValue_t F) { fCallback = F; }

UA_DataValue NodeValue::Value() { return fCallback(); }

NodeValue::NodeValue(const open62541Cpp::UA_NodeId &nodeId) : NodeId(nodeId) {}

NodeValue::NodeValue(NodeValue &&other) : NodeId(std::move(other.NodeId)) {}
