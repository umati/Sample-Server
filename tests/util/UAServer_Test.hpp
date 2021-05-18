#pragma once
#include <gtest/gtest.h>
#include <open62541/server.h>
#include <open62541/server_config_default.h>

class UAServer_Test : public ::testing::Test {
 public:
  UA_Server *pServer = nullptr;
  static constexpr const char *NsTestNs = "TestNs";

 protected:
  static UA_StatusCode generateChildNodeIdInParentNs(
    UA_Server *server,
    const UA_NodeId *sessionId,
    void *sessionContext,
    const UA_NodeId *sourceNodeId,
    const UA_NodeId *targetParentNodeId,
    const UA_NodeId *referenceTypeId,
    UA_NodeId *targetNodeId) {
    if (UA_NodeId_equal(targetNodeId, &UA_NODEID_NULL) && !UA_NodeId_equal(targetParentNodeId, &UA_NODEID_NULL)) {
      targetNodeId->namespaceIndex = targetParentNodeId->namespaceIndex;
    }
    return UA_STATUSCODE_GOOD;
  }

  void SetUp() override {
    pServer = UA_Server_new();
    auto pConfig = UA_Server_getConfig(pServer);
    UA_ServerConfig_setDefault(pConfig);
    pConfig->nodeLifecycle.generateChildNodeId = generateChildNodeIdInParentNs;
    m_testNs = UA_Server_addNamespace(pServer, NsTestNs);
  }

  void TearDown() override {
    UA_Server_run_shutdown(pServer);
    UA_Server_delete(pServer);
  }

  std::uint16_t m_testNs;
};
