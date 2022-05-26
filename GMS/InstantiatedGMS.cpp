/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2022 (c) mindonwarp
 */

#include "InstantiatedGMS.hpp"

#include <sstream>

InstantiatedGMS::InstantiatedGMS(UA_Server* pServer) : m_pServer(pServer), NsIndex(m_nsIndex), n(pServer) {}

void InstantiatedGMS::CreateObject() {
  std::stringstream ss;
  ss << "http://example.com/" << MachineName << "/";
  m_nsIndex = UA_Server_addNamespace(m_pServer, ss.str().c_str());
  UA_ObjectAttributes objAttr = UA_ObjectAttributes_default;

  objAttr.displayName = UA_LOCALIZEDTEXT_ALLOC("", MachineName.c_str());
  objAttr.eventNotifier = UA_EVENTNOTIFIERTYPE_SUBSCRIBETOEVENTS;

  auto status = UA_Server_addObjectNode(
    m_pServer,
    UA_NODEID_NUMERIC(m_nsIndex, 0),
    UA_NODEID_NUMERIC(nsFromUri(m_pServer, constants::NsMachineryUri), UA_MACHINERYID_MACHINES),
    UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
    *open62541Cpp::UA_QualifiedName(m_nsIndex, MachineName).QualifiedName,
    UA_NODEID_NUMERIC(nsFromUri(m_pServer, constants::NsGMSUri), UA_GMSID_GMSTYPE),
    objAttr,
    nullptr,
    m_nodeId.NodeId);

  UA_ObjectAttributes_clear(&objAttr);
  UmatiServerLib::Bind::MembersRefl(gms, m_pServer, m_nodeId, n);
}

void InstantiatedGMS::Simulate() {}

void InstantiatedGMS::InstantiateIdentification() {
  {
    std::stringstream ss;
    ss << "http://www.isw.uni-stuttgart.de/#" << MachineName;
    gms.Identification->ProductInstanceUri = ss.str();
  }

  gms.Identification->Manufacturer = {"", "mindonwarp"};

  {
    std::hash<std::string> hasher;
    std::stringstream ss;
    ss << "2022-" << hasher(MachineName);
    gms.Identification->SerialNumber = ss.str();
  }
}

void InstantiatedGMS::InstantiateEquipment() {}

void InstantiatedGMS::InstantiateNotification() {}

void InstantiatedGMS::InstantiateProduction() {}

void InstantiatedGMS::InstantiateResultManagement() {}

void InstantiatedGMS::InstantiateMonitoring() {}
