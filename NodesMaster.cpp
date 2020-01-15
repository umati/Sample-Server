///
/// \file NodeMaster.cpp
/// \author Christian von Arnim
/// \date 17.12.2019
///


#include "NodesMaster.hpp"

NodesMaster::NodesMaster(UA_Server *pServer) : m_pServer(pServer) {

}

NodeValue& NodesMaster::operator()(int nsIndex, int nsIntId) {
    return m_Nodes[open62541Cpp::UA_NodeId(nsIndex, nsIntId)];
}

NodeValue &NodesMaster::operator()(const UA_NodeId &nodeId) {
  return m_Nodes[open62541Cpp::UA_NodeId(nodeId)];
}

void NodesMaster::callback(UA_Server *pServer,
                           const UA_NodeId *sessionId,
                           void *sessionContext,
                           const UA_NodeId *nodeid,
                           void *nodeContext,
                           const UA_NumericRange *range,
                           const UA_DataValue *oldValue) {
  NodesMaster * pThis = reinterpret_cast<NodesMaster*>(nodeContext);
  auto it = pThis->m_Nodes.find(open62541Cpp::UA_NodeId(nodeid));
  if(it != pThis->m_Nodes.end())
  {
    UA_Server_writeValue(pServer, *nodeid, it->second.Value());
  }
}

void NodesMaster::SetCallbacks() {
  UA_ValueCallback valCallback;
  valCallback.onWrite = nullptr;
  valCallback.onRead = NodesMaster::callback;
  for(auto& el : m_Nodes)
  {
    UA_Server_setNodeContext(m_pServer, *el.first.NodeId, this);
    UA_Server_setVariableNode_valueCallback(
        m_pServer,
        *el.first.NodeId,
        valCallback
    );
  }
}
