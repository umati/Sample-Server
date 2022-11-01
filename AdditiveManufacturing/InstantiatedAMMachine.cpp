/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2022 (c) Patrick Moeller, 3Yourmind GmbH
 * Copyright 2022 (c) Dennis Hezel, 3Yourmind GmbH
 */

#include "InstantiatedAMMachine.hpp"

InstantiatedAMMachine::InstantiatedAMMachine(UA_Server *pServer) : m_pServer(pServer), NsIndex(m_nsIndex), n(pServer) {}

void InstantiatedAMMachine::CreateObject() {
  std::stringstream ss;
  ss << "http://www.3yourmind.com/" << MachineName << "/";
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
    UA_NODEID_NUMERIC(nsFromUri(m_pServer, constants::NsAMUri), UA_ADDITIVEMANUFACTURINGID_ADDITIVEMANUFACTURINGTYPE),
    objAttr,
    nullptr,
    m_nodeId.NodeId);

  UA_ObjectAttributes_clear(&objAttr);
  UmatiServerLib::Bind::MembersRefl(mt, m_pServer, m_nodeId, n);
  InstantiateIdentification();
  InstantiateMonitoring();
  InstantiateProduction();
}

void InstantiatedAMMachine::InstantiateIdentification() {
  {
    std::stringstream ss;
    ss << "http://www.3yourmind.com/#" << MachineName;
    mt.Identification->ProductInstanceUri = ss.str();
  }

  InstantiateOptional(mt.Identification->YearOfConstruction, m_pServer, n);
  InstantiateOptional(mt.Identification->ProductCode, m_pServer, n);
  InstantiateOptional(mt.Identification->SoftwareRevision, m_pServer, n);
  InstantiateOptional(mt.Identification->DeviceClass, m_pServer, n);
  InstantiateOptional(mt.Identification->Location, m_pServer, n);
  InstantiateOptional(mt.Identification->Model, m_pServer, n);
  InstantiateOptional(mt.Identification->AMTechnologyIdentifier, m_pServer, n);

  mt.Identification->Manufacturer = {"", "3YOURMIND"};
  mt.Identification->ProductCode = "AM_123";
  mt.Identification->YearOfConstruction = 2022;
  mt.Identification->SoftwareRevision = "v1.0.0";
  mt.Identification->DeviceClass = "Additive Manufacturing machine";
  mt.Identification->Location = "Berlin, Germany";
  mt.Identification->Model = {"", MachineName};
  mt.Identification->AMTechnologyIdentifier = "MEX";

  {
    std::hash<std::string> hasher;
    std::stringstream ss;
    ss << "2022-" << hasher(MachineName);
    mt.Identification->SerialNumber = ss.str();
  }
}

void InstantiatedAMMachine::InstantiateMonitoring() {
  mt.Monitoring->MachineTool->OperationMode = UA_MachineOperationMode::UA_MACHINEOPERATIONMODE_AUTOMATIC;
  InstantiateOptional(mt.Monitoring->MachineTool->PowerOnDuration, m_pServer, n);
  mt.Monitoring->MachineTool->PowerOnDuration = 10;
  InstantiateMonitoringStacklight({UA_SignalColor::UA_SIGNALCOLOR_RED, UA_SignalColor::UA_SIGNALCOLOR_YELLOW, UA_SignalColor::UA_SIGNALCOLOR_GREEN});
}

void InstantiatedAMMachine::InstantiateMonitoringStacklight(std::list<UA_SignalColor> stacklightColors) {
  InstantiateOptional(mt.Monitoring->Stacklight, m_pServer, n);
  InstantiateOptional(mt.Monitoring->Stacklight->NodeVersion, m_pServer, n);
  mt.Monitoring->Stacklight->StacklightMode = UA_StacklightOperationMode::UA_STACKLIGHTOPERATIONMODE_SEGMENTED;

  // Store size, as the list will become shorter
  std::size_t s = stacklightColors.size();
  for (std::size_t i = 0; i < s; ++i) {
    std::stringstream ss;
    ss << "Light " << i;
    auto &light = mt.Monitoring->Stacklight->OrderedObjects.Add(m_pServer, n, {m_nsIndex, ss.str()});
    InstantiateOptional(light.IsPartOfBase, m_pServer, n);
    InstantiateOptional(light.SignalOn, m_pServer, n);
    InstantiateOptional(light.SignalMode, m_pServer, n);

    light.SignalMode = UA_SignalModeLight::UA_SIGNALMODELIGHT_CONTINUOUS;
    light.IsPartOfBase = false;
    light.NumberInList = i;
    light.SignalOn = true;
    if (!stacklightColors.empty()) {
      light.SignalColor = stacklightColors.front();
      InstantiateOptional(light.SignalColor, m_pServer, n);
      stacklightColors.pop_front();
    }
  }
}

void InstantiatedAMMachine::InstantiateProduction() {
  InstantiateOptional(mt.Production->ActiveProgram->State, m_pServer, n);
  mt.Production->ActiveProgram->NumberInList = 0;
  mt.Production->ActiveProgram->Name = "Print Job";
  mt.Production->ActiveProgram->State->CurrentState->Value = {"en", "Running"};
  mt.Production->ActiveProgram->State->CurrentState->Number = 1;
  mt.Production->ActiveProgram->State->CurrentState->Id =
    UA_NODEID_NUMERIC(nsFromUri(m_pServer, constants::NsMachineToolUri), UA_MACHINETOOLID_PRODUCTIONSTATEMACHINETYPE_RUNNING);
}

void InstantiatedAMMachine::SimulateStacklight() {
  for (auto &light : mt.Monitoring->Stacklight->OrderedObjects.value) {
    light->SignalOn = (rnd() % 2) == 0;
  }
}

// void InstantiatedAMMachine::Simulate() {
//   ++m_simStep;
//   if ((m_simStep % 2) == 1) {
//     SimulateStacklight();
//   }

//   mt.Monitoring->MachineTool->PowerOnDuration = m_simStep / 3600;
// }