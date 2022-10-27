/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2022 (c) Patrick Moeller, 3Yourmind GmbH
 */

#include "BasicAMMachine.hpp"

BasicAMMachine::BasicAMMachine(UA_Server *pServer) : InstantiatedMachineTool(pServer) {
  MachineName = "BasicAMMachine";
  CreateObject();
}

void BasicAMMachine::CreateObject() {
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
    UA_NODEID_NUMERIC(nsFromUri(m_pServer, constants::NsAMUri), UA_ADDITIVEMANUFACTURINGID_ADDITIVIEMANUFACTURINGTYPE),
    objAttr,
    nullptr,
    m_nodeId.NodeId);

  UA_ObjectAttributes_clear(&objAttr);
  UmatiServerLib::Bind::MembersRefl(machine, m_pServer, m_nodeId, n);
  InstantiateIdentification();
  InstantiateMonitoring();
  InstantiateProduction();
}

void BasicAMMachine::InstantiateIdentification() {
  {
    std::stringstream ss;
    ss << "http://www.3yourmind.com/#" << MachineName;
    machine.Identification->ProductInstanceUri = ss.str();
  }

  InstantiateOptional(machine.Identification->YearOfConstruction, m_pServer, n);
  InstantiateOptional(machine.Identification->ProductCode, m_pServer, n);
  InstantiateOptional(machine.Identification->SoftwareRevision, m_pServer, n);
  InstantiateOptional(machine.Identification->DeviceClass, m_pServer, n);
  InstantiateOptional(machine.Identification->Location, m_pServer, n);
  InstantiateOptional(machine.Identification->Model, m_pServer, n);
  InstantiateOptional(machine.Identification->AMTechnologyIdentifier, m_pServer, n);

  machine.Identification->Manufacturer = {"", "3YOURMIND"};
  machine.Identification->ProductCode = "AM_123";
  machine.Identification->YearOfConstruction = 2022;
  machine.Identification->SoftwareRevision = "v1.0.0";
  machine.Identification->DeviceClass = "Additive Manufacturing machine";
  machine.Identification->Location = "Berlin, Germany";
  machine.Identification->Model = {"", MachineName};
  machine.Identification->AMTechnologyIdentifier = "MEX";

  {
    std::hash<std::string> hasher;
    std::stringstream ss;
    ss << "2022-" << hasher(MachineName);
    machine.Identification->SerialNumber = ss.str();
  }
}

void BasicAMMachine::InstantiateMonitoring() {
  InstantiateMonitoringMT();
  InstantiateMonitoringStacklight({UA_SignalColor::UA_SIGNALCOLOR_RED, UA_SignalColor::UA_SIGNALCOLOR_YELLOW, UA_SignalColor::UA_SIGNALCOLOR_GREEN});
  InstantiateMonitoringChannel(1);  // FIXME: Is this needed?
}

void BasicAMMachine::InstantiateProduction() {
  InstantiateOptional(machine.Production->ActiveProgram->State, m_pServer, n);
  machine.Production->ActiveProgram->NumberInList = 0;
  machine.Production->ActiveProgram->Name = "Basic Program";
  machine.Production->ActiveProgram->State->CurrentState->Value = {"en", "Running"};
  machine.Production->ActiveProgram->State->CurrentState->Number = 1;
  machine.Production->ActiveProgram->State->CurrentState->Id =
    UA_NODEID_NUMERIC(nsFromUri(m_pServer, constants::NsMachineToolUri), UA_MACHINETOOLID_PRODUCTIONSTATEMACHINETYPE_RUNNING);
}

// FIXME: Is that relevant?
void BasicAMMachine::Simulate() {
  ++m_simStep;
  if ((m_simStep % 2) == 1) {
    SimulateStacklight();
  }

  mt.Monitoring->MachineTool->PowerOnDuration = m_simStep / 3600;
}