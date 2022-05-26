/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2022 (c) mindonwarp
 */

#include "FullGMS.hpp"

#include <string>
#include <sstream>

#include "../TypeDefinition/GMS/Constants.hpp"
#include "../TypeDefinition/GMS/GMSType.hpp"

FullGMS::FullGMS(UA_Server *pServer) : InstantiatedGMS(pServer) {
  MachineName = "FullGMS";
  CreateObject();
}

void FullGMS::CreateObject() {
  std::stringstream ss;
  ss << "http://www.isw.uni-stuttgart.de/" << MachineName << "/";
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

  InstantiateIdentification();
  InstantiateMonitoring();
  InstantiateProduction();
  InstantiateResultManagement();
}

void FullGMS::InstantiateIdentification() {
  {
    std::stringstream ss;
    ss << "http://www.isw.uni-stuttgart.de/#" << MachineName;
    gms.Identification->ProductInstanceUri = ss.str();
  }

  InstantiateOptional(gms.Identification->YearOfConstruction, m_pServer, n);
  InstantiateOptional(gms.Identification->ProductCode, m_pServer, n);
  InstantiateOptional(gms.Identification->SoftwareRevision, m_pServer, n);
  InstantiateOptional(gms.Identification->DeviceClass, m_pServer, n);
  InstantiateOptional(gms.Identification->Location, m_pServer, n);
  InstantiateOptional(gms.Identification->Model, m_pServer, n);

  gms.Identification->Manufacturer = {"", "ISW University of Stuttgart"};
  gms.Identification->ProductCode = "CMM_1234";
  gms.Identification->YearOfConstruction = 2022;
  gms.Identification->SoftwareRevision = "v1.05";
  gms.Identification->DeviceClass = "CoordinateMeasuringMachine";
  gms.Identification->Location = "CTRL 5 5318/VIRTUAL 0 0/N 48.7685303 E 9.2653923";
  gms.Identification->Model = {"", MachineName};

  {
    std::hash<std::string> hasher;
    std::stringstream ss;
    ss << "2022-" << hasher(MachineName);
    gms.Identification->SerialNumber = ss.str();
  }
}

void FullGMS::InstantiateMonitoring() {
  InstantiateOptional(gms.Monitoring->MachineTool->PowerOnDuration, m_pServer, n);
  InstantiateOptional(gms.Monitoring->MachineTool->FeedOverride, m_pServer, n);
  gms.Monitoring->MachineTool->OperationMode = UA_MachineOperationMode::UA_MACHINEOPERATIONMODE_AUTOMATIC;
}

void FullGMS::InstantiateProduction() {
  InstantiateOptional(gms.Production->ActiveProgram->State, m_pServer, n);
  gms.Production->ActiveProgram->NumberInList = 0;
  gms.Production->ActiveProgram->Name = "Basic Measurment Program";
  gms.Production->ActiveProgram->State->CurrentState->Value = {"en", "Running"};
  gms.Production->ActiveProgram->State->CurrentState->Number = 1;
  gms.Production->ActiveProgram->State->CurrentState->Id =
    UA_NODEID_NUMERIC(nsFromUri(m_pServer, constants::NsMachineToolUri), UA_MACHINETOOLID_PRODUCTIONSTATEMACHINETYPE_RUNNING);
}