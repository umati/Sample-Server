/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2019-2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 */

#include "NodesMaster.hpp"

#include <iostream>

NodesMaster::NodesMaster(UA_Server* pServer) : m_pServer(pServer) {}

NodeValue& NodesMaster::operator()(int nsIndex, int nsIntId) { return this->operator()(open62541Cpp::UA_NodeId(nsIndex, nsIntId)); }

NodeValue& NodesMaster::operator()(const UA_NodeId& nodeId) { return this->operator()(open62541Cpp::UA_NodeId(nodeId)); }

NodeValue& NodesMaster::operator()(const open62541Cpp::UA_NodeId& nodeId) {
  auto it = m_Nodes.find(nodeId);
  if (it == m_Nodes.end()) {
    auto itNew = m_Nodes.insert(std::make_pair(nodeId, NodeValue(nodeId)));
    setCallback(nodeId);
    return itNew.first->second;
  }

  return it->second;
}

void NodesMaster::callback(
  UA_Server* pServer,
  const UA_NodeId* sessionId,
  void* sessionContext,
  const UA_NodeId* nodeid,
  void* nodeContext,
  const UA_NumericRange* range,
  const UA_DataValue* oldValue) {
  NodesMaster* pThis = reinterpret_cast<NodesMaster*>(nodeContext);
  if (pThis == nullptr || nodeid == nullptr) {
    return;
  }

  const auto nodeKey = open62541Cpp::UA_NodeId(nodeid);

  {
    std::lock_guard<std::mutex> lock(pThis->m_CallbackInProgressMutex);
    if (pThis->m_CallbackInProgress.find(nodeKey) != pThis->m_CallbackInProgress.end()) {
      return;
    }
    pThis->m_CallbackInProgress.insert(nodeKey);
  }

  auto it = pThis->m_Nodes.find(nodeKey);
  if (it != pThis->m_Nodes.end()) {
    auto val = it->second.Value();
    if (auto ret = UA_Server_writeDataValue(pServer, *nodeid, val); ret != UA_STATUSCODE_GOOD) {
      std::cerr << "Writing value failed: " << UA_StatusCode_name(ret) << std::endl;
    }
    UA_DataValue_clear(&val);
  }

  {
    std::lock_guard<std::mutex> lock(pThis->m_CallbackInProgressMutex);
    pThis->m_CallbackInProgress.erase(nodeKey);
  }
}

void NodesMaster::Remove(const open62541Cpp::UA_NodeId& nodeId) {
  auto it = m_Nodes.find(nodeId);
  if (it != m_Nodes.end()) {
    UA_Server_setNodeContext(m_pServer, *it->first.NodeId, NULL);
    UA_ValueCallback valCallback;
    valCallback.onWrite = nullptr;
    valCallback.onRead = nullptr;
    UA_Server_setVariableNode_valueCallback(m_pServer, *it->first.NodeId, valCallback);
    m_Nodes.erase(it);
  }
}

void NodesMaster::setCallback(const open62541Cpp::UA_NodeId& nodeId) {
  ///\todo Use DataSource instead?
  // UA_DataSource dsCallback;
  // dsCallback.read = NodesMaster::callback;
  // dsCallback.write = nullptr;
  // UA_Server_setVariableNode_dataSource(m_pServer, el.first.NodeId, dsCallback);

  UA_ValueCallback valCallback;
  valCallback.onWrite = nullptr;
  valCallback.onRead = NodesMaster::callback;
  UA_Server_setNodeContext(m_pServer, *nodeId.NodeId, this);
  UA_Server_setVariableNode_valueCallback(m_pServer, *nodeId.NodeId, valCallback);
}
