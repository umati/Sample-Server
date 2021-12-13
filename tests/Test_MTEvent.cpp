/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2020-2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 */

#include <gtest/gtest.h>

#include <Open62541Cpp/UA_QualifiedName.hpp>

#include "../TypeDefinition/MachineTool/ProductionJobTransitionEvent.hpp"
#include "../UmatiServerLib/OpcUaEvent.hpp"
#include "../UmatiServerLib/Util.hpp"
#include "../src_generated/machinetool_nodeids.h"
#include "util/MTServer_Test.hpp"

class MTEvent : public MTServer_Test {};

TEST_F(MTEvent, ProductionJobTransitionEvent) {
  machineTool::ProductionJobTransitionEvent_t jtevent;
  jtevent.Message = {"en", "Transition to Running triggered"};

  jtevent.Severity = 20;

  jtevent.SourceName = "MRMachineTool";
  jtevent.Identifier = "Any Identifier";
  jtevent.RunsPlanned->Value = 56;
  jtevent.RunsPlanned->IsValid = false;
  jtevent.RunsCompleted = 23;
  jtevent.Transition->Value = {"en", "InitializingToRunning"};
  UA_NodeId id = UA_NODEID_NUMERIC(nsFromUri(pServer, constants::NsMachineToolUri), UA_MACHINETOOLID_PRODUCTIONSTATEMACHINETYPE_INITIALIZINGTORUNNING);
  jtevent.Transition->Id =
    UA_NODEID_NUMERIC(nsFromUri(pServer, constants::NsMachineToolUri), UA_MACHINETOOLID_PRODUCTIONSTATEMACHINETYPE_INITIALIZINGTORUNNING);
  jtevent.FromState->Value = {"en", "Initializing"};
  jtevent.FromState->Id = UA_NODEID_NUMERIC(nsFromUri(pServer, constants::NsMachineToolUri), UA_MACHINETOOLID_PRODUCTIONSTATEMACHINETYPE_INITIALIZING);
  jtevent.ToState->Value = {"en", "Running"};
  jtevent.ToState->Id = UA_NODEID_NUMERIC(nsFromUri(pServer, constants::NsMachineToolUri), UA_MACHINETOOLID_PRODUCTIONSTATEMACHINETYPE_RUNNING);

  OpcUaEvent ev(jtevent, pServer, open62541Cpp::UA_NodeId(UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER)));

  {
    open62541Cpp::UA_QualifiedName qnFromState(nsFromUri(pServer, constants::Ns0Uri), "FromState");
    UA_BrowsePath p;
    UA_RelativePathElement el;
    p.startingNode = *ev.EventNodeId.NodeId;
    p.relativePath.elementsSize = 1;
    p.relativePath.elements = &el;
    el.isInverse = UA_FALSE;
    el.referenceTypeId = UA_NODEID_NUMERIC(0, UA_NS0ID_HASCOMPONENT);
    el.includeSubtypes = UA_FALSE;
    el.targetName = *qnFromState.QualifiedName;
    UA_BrowsePathResult ret = UA_Server_translateBrowsePathToNodeIds(pServer, &p);
    ASSERT_EQ(ret.statusCode, UA_STATUSCODE_GOOD);
    ASSERT_EQ(ret.targetsSize, 1);
    UA_Variant fromState;
    UA_Variant_init(&fromState);
    ASSERT_EQ(UA_Server_readValue(pServer, ret.targets->targetId.nodeId, &fromState), UA_STATUSCODE_GOOD);
    ASSERT_FALSE(UA_Variant_isEmpty(&fromState));
    ASSERT_TRUE(UA_Variant_isScalar(&fromState));
    ASSERT_TRUE(UA_NodeId_equal(&fromState.type->typeId, &UA_TYPES[UA_TYPES_LOCALIZEDTEXT].typeId));
    UA_Variant_clear(&fromState);
    UA_BrowsePathResult_clear(&ret);
  }

  {
    open62541Cpp::UA_QualifiedName qnFromState(nsFromUri(pServer, constants::Ns0Uri), "Id");
    UA_Variant IdVariant;
    UA_Variant_init(&IdVariant);
    ASSERT_EQ(UA_Server_readObjectProperty(pServer, *ev.Data.FromState.NodeId.NodeId, *qnFromState.QualifiedName, &IdVariant), UA_STATUSCODE_GOOD);
    ASSERT_FALSE(UA_Variant_isEmpty(&IdVariant));
    ASSERT_TRUE(UA_Variant_isScalar(&IdVariant));
    ASSERT_TRUE(UA_NodeId_equal(&IdVariant.type->typeId, &UA_TYPES[UA_TYPES_NODEID].typeId));
    ASSERT_TRUE(UA_NodeId_equal((UA_NodeId*)IdVariant.data, &jtevent.FromState->Id.value));
    UA_Variant_clear(&IdVariant);
  }

  {
    open62541Cpp::UA_QualifiedName qnFromState(nsFromUri(pServer, constants::Ns0Uri), "Message");
    UA_Variant MsgVariant;
    UA_Variant_init(&MsgVariant);
    ASSERT_EQ(UA_Server_readObjectProperty(pServer, *ev.EventNodeId.NodeId, *qnFromState.QualifiedName, &MsgVariant), UA_STATUSCODE_GOOD);
    ASSERT_FALSE(UA_Variant_isEmpty(&MsgVariant));
    ASSERT_TRUE(UA_Variant_isScalar(&MsgVariant));
    ASSERT_TRUE(UA_NodeId_equal(&MsgVariant.type->typeId, &UA_TYPES[UA_TYPES_LOCALIZEDTEXT].typeId));

    auto readValue = UmatiServerLib::LocalizedText_t::fromUa(*(UA_LocalizedText*)MsgVariant.data);
    ASSERT_EQ(readValue, jtevent.Message);
    UA_Variant_clear(&MsgVariant);
  }
}
