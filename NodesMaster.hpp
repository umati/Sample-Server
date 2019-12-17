///
/// \file NodesMaster.hpp
/// \author Christian von Arnim
/// \date 17.12.2019
///

#pragma once

#include "NodeValue.hpp"
#include <open62541/server.h>
#include <map>

class NodesMaster {
 protected:
  std::map<int, NodeValue> m_Nodes;
  UA_Server *m_pServer;
 public:
  NodesMaster(UA_Server *pServer);

  NodeValue &operator()(int nsIndex, int nsIntId);

  static void callback(UA_Server *pServer, const UA_NodeId *sessionId,
                       void *sessionContext, const UA_NodeId *nodeid,
                       void *nodeContext, const UA_NumericRange *range,
                       const UA_DataValue *oldValue);

  void SetCallbacks();
};