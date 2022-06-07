/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2022 (c) Alen Galinec
 */

#include <sstream>

#include "InstantiatedGMS.hpp"

const std::unordered_map<std::uint32_t, UmatiServerLib::LocalizedText_t> InstantiatedGMS::CLASS_MAP = {
  {0, {"en", "NoTool"}},
  {1, {"en", "UnDefTool"}},
  {2, {"en", "TactileTouchTrigger"}},
  {3, {"en", "TactileMeasuring"}},
  {4, {"en", "Optical-1D"}},
  {5, {"en", "Optical-2D"}},
  {6, {"en", "Optical-3D"}},
  {7, {"en", "Roughness"}},
  {8, {"en", "Eddy Current Sensor"}},
  {9, {"en", "TemperatureProbing"}},
  {10, {"en", "PtMeas"}},
  {11, {"en", "Other"}}};

InstantiatedGMS::InstantiatedGMS(UA_Server *pServer) : m_pServer(pServer), NsIndex(m_nsIndex), n(pServer) {}

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

GMS::GMSSensor_t &InstantiatedGMS::InstantiateSensor(std::string const &sensorName) {
  // It seems very wasteful for every instance of a sensor to have its own copy of the "Class" EnumStrings[].
  // I assume this array is the same for all the sensors of the instance.
  // It would be better if we only had one instance of "Class"[] and have all
  // sensor instances point to the same object in memory. Is that doable?
  auto &sensor = gms.Equipment->Tools->Tool.Add<GMS::GMSSensor_t>(m_pServer, n, {m_nsIndex, sensorName});
  for (const auto &localizedClassName : CLASS_MAP) sensor.Class->EnumStrings->push_back(localizedClassName.second);

  return sensor;
}
