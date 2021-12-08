/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2020-2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 */

#include <gtest/gtest.h>

#include <Open62541Cpp/UA_NodeId.hpp>
#include <Open62541Cpp/UA_QualifiedName.hpp>
#include <atomic>
#include <thread>

#include "../OpcUaTypes/LocalizedText.hpp"
#include "../TypeDefinition/TypeDefinition.hpp"
#include "../UmatiServerLib/BindRefl.hpp"
#include "util/UAClient.hpp"
#include "util/UAServer_Test.hpp"

namespace TestTypes {

struct Test_t {
  BindableMemberValue<UmatiServerLib::LocalizedText_t> TestVar;
};

}  // namespace TestTypes

REFL_TYPE(TestTypes::Test_t, UmatiServerLib::attribute::UaObjectType(UmatiServerLib::constexp::NodeId(UAServer_Test::NsTestNs, 1234)))
REFL_FIELD(TestVar, UmatiServerLib::attribute::MemberInTypeNodeId(UmatiServerLib::constexp::NodeId(UAServer_Test::NsTestNs, 1235)))
REFL_END

class WrongDatatype : public UAServer_Test {
 protected:
  void SetUp() override {
    UAServer_Test::SetUp();
    m_run = true;
  }

  UA_StatusCode RunServer() {
    m_serverThread = std::thread(UA_Server_run, pServer, &m_run);
    return UA_STATUSCODE_GOOD;
  }

  void TearDown() override {
    m_run = false;
    if (m_serverThread.joinable()) {
      m_serverThread.join();
    }
    UAServer_Test::TearDown();
  }

  UAClient client;
  bool m_run = true;
  std::thread m_serverThread;
};

TEST_F(WrongDatatype, BindLocalizedTextForString) {
  open62541Cpp::UA_NodeId nIdObjType((std::uint16_t)m_testNs, (std::uint32_t)1234);
  open62541Cpp::UA_QualifiedName ObjTypeQN(m_testNs, "ATestType");
  UA_ObjectTypeAttributes objTypeAttr;
  ASSERT_EQ(UA_ObjectTypeAttributes_copy(&UA_ObjectTypeAttributes_default, &objTypeAttr), UA_STATUSCODE_GOOD);
  objTypeAttr.displayName = UA_LOCALIZEDTEXT_ALLOC("", "ATestType");

  ASSERT_EQ(
    UA_Server_addObjectTypeNode(
      pServer,
      *nIdObjType.NodeId,
      UA_NODEID_NUMERIC(0, UA_NS0ID_BASEOBJECTTYPE),
      UA_NODEID_NUMERIC(0, UA_NS0ID_HASSUBTYPE),
      *ObjTypeQN.QualifiedName,
      objTypeAttr,
      nullptr,
      nIdObjType.NodeId),
    UA_STATUSCODE_GOOD);
  UA_ObjectTypeAttributes_clear(&objTypeAttr);

  open62541Cpp::UA_NodeId nIdObjTypeVar((std::uint16_t)m_testNs, (std::uint32_t)1235);
  open62541Cpp::UA_QualifiedName ObjTypeVarQN(m_testNs, "TestVar");
  UA_VariableAttributes objTypeVarAttr;
  ASSERT_EQ(UA_VariableAttributes_copy(&UA_VariableAttributes_default, &objTypeVarAttr), UA_STATUSCODE_GOOD);
  objTypeVarAttr.displayName = UA_LOCALIZEDTEXT_ALLOC("", "TestVar");
  objTypeVarAttr.dataType = UA_NODEID_NUMERIC(0, UA_NS0ID_STRING);
  ASSERT_EQ(
    UA_Server_addVariableNode(
      pServer,
      *nIdObjTypeVar.NodeId,
      *nIdObjType.NodeId,
      UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT),
      *ObjTypeVarQN.QualifiedName,
      UA_NODEID_NUMERIC(0, UA_NS0ID_BASEDATAVARIABLETYPE),
      objTypeVarAttr,
      nullptr,
      nIdObjTypeVar.NodeId),
    UA_STATUSCODE_GOOD);
  UA_VariableAttributes_clear(&objTypeVarAttr);
  ASSERT_EQ(
    UA_Server_addReference(
      pServer, *nIdObjTypeVar.NodeId, UA_NODEID_NUMERIC(0, UA_NS0ID_HASMODELLINGRULE), UA_EXPANDEDNODEID_NUMERIC(0, UA_NS0ID_MODELLINGRULE_MANDATORY), UA_TRUE),
    UA_STATUSCODE_GOOD);

  open62541Cpp::UA_NodeId nIdObjInst((std::uint16_t)m_testNs, (std::uint32_t)2345);
  open62541Cpp::UA_QualifiedName ObjInstQN(m_testNs, "InstObj");
  UA_ObjectAttributes objInstAttr;
  ASSERT_EQ(UA_ObjectAttributes_copy(&UA_ObjectAttributes_default, &objInstAttr), UA_STATUSCODE_GOOD);
  objInstAttr.displayName = UA_LOCALIZEDTEXT_ALLOC("", "InstObj");
  ASSERT_EQ(
    UA_Server_addObjectNode(
      pServer,
      *nIdObjInst.NodeId,
      UA_NODEID_NUMERIC(0, UA_NS0ID_OBJECTSFOLDER),
      UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
      *ObjInstQN.QualifiedName,
      *nIdObjType.NodeId,
      objInstAttr,
      nullptr,
      nIdObjInst.NodeId),
    UA_STATUSCODE_GOOD);
  UA_ObjectAttributes_clear(&objInstAttr);

  NodesMaster n(pServer);
  TestTypes::Test_t testObj;
  UmatiServerLib::Bind::MembersRefl(testObj, pServer, nIdObjInst, n);
  testObj.TestVar = {"e", "Str"};

  ASSERT_EQ(RunServer(), UA_STATUSCODE_GOOD);
  std::this_thread::sleep_for(std::chrono::milliseconds(100));
  ASSERT_EQ(client.Connect("opc.tcp://localhost:4840"), UA_STATUSCODE_GOOD);
  ASSERT_EQ(client.ReadValue(testObj.TestVar.NodeId), UA_STATUSCODE_GOOD);
}
