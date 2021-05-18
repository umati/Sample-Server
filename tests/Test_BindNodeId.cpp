
#include <gtest/gtest.h>

#include <Open62541Cpp/UA_NodeId.hpp>
#include <Open62541Cpp/UA_QualifiedName.hpp>

#include "../UmatiServerLib/BindVariable.hpp"
#include "../UmatiServerLib/NodesMaster.hpp"
#include "util/UAServer_Test.hpp"

class BindNodeId : public UAServer_Test {};

TEST_F(BindNodeId, BindNodeId_Basic) {
  auto nsIndex = UA_Server_addNamespace(pServer, "BindNodeId_Basic");
  open62541Cpp::UA_QualifiedName qn(nsIndex, "MyNodeId");

  UA_VariableAttributes uavar = UA_VariableAttributes_default;
  uavar.dataType = UA_NODEID_NUMERIC(0, UA_NS0ID_NODEID);
  uavar.displayName = UA_LOCALIZEDTEXT_ALLOC("", "MyNodeId");
  open62541Cpp::UA_NodeId varNodeId;
  ASSERT_EQ(
    UA_Server_addVariableNode(
      pServer,
      UA_NODEID_NUMERIC(nsIndex, 0),
      UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
      UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
      *qn.QualifiedName,
      UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
      uavar,
      NULL,
      varNodeId.NodeId),
    UA_STATUSCODE_GOOD);
  UA_VariableAttributes_clear(&uavar);
  NodesMaster n(pServer);

  BindableMemberValue<UA_NodeId> bindedNodeId;
  UmatiServerLib::BindVariable::ToNode(pServer, varNodeId, n, bindedNodeId);
  bindedNodeId = UA_NODEID_NUMERIC(1, 10);
  UA_NodeId cmp = UA_NODEID_NUMERIC(1, 10);
  UA_Variant variant;
  UA_Variant_init(&variant);
  ASSERT_EQ(UA_Server_readValue(pServer, *varNodeId.NodeId, &variant), UA_STATUSCODE_GOOD);
  ASSERT_FALSE(UA_Variant_isEmpty(&variant));
  ASSERT_TRUE(UA_Variant_isScalar(&variant));
  ASSERT_TRUE(UA_NodeId_equal(&variant.type->typeId, &UA_TYPES[UA_TYPES_NODEID].typeId));
  ASSERT_TRUE(UA_NodeId_equal((UA_NodeId *)variant.data, &cmp));

  UA_Variant_clear(&variant);
}
