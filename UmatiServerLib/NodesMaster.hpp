///
/// \file NodesMaster.hpp
/// \author Christian von Arnim
/// \date 17.12.2019
///

#pragma once

#include <open62541/server.h>

#include <Open62541Cpp/UA_NodeId.hpp>
#include <map>

#include "NodeValue.hpp"

/**
 * @brief Stores a list of NodeId <-> Member and handle read value calls
 *
 */
class NodesMaster {
 protected:
  std::map<open62541Cpp::UA_NodeId, NodeValue> m_Nodes;
  UA_Server *m_pServer;
  NodesMaster(const NodesMaster &other);
  void setCallback(const open62541Cpp::UA_NodeId &nodeId);

 public:
  NodesMaster(UA_Server *pServer);

  NodeValue &operator()(int nsIndex, int nsIntId);
  NodeValue &operator()(const UA_NodeId &nodeId);
  NodeValue &operator()(const open62541Cpp::UA_NodeId &nodeId);

  void Remove(const open62541Cpp::UA_NodeId &nodeId);

  // Read callback, which will be resolved to the member internally
  static void callback(
    UA_Server *pServer,
    const UA_NodeId *sessionId,
    void *sessionContext,
    const UA_NodeId *nodeid,
    void *nodeContext,
    const UA_NumericRange *range,
    const UA_DataValue *oldValue);
};
