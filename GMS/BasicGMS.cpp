/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2022 (c) Sebastian Friedl, ISW University of Stuttgart (for VDMA e.V.)
 */


#include "BasicGMS.hpp"
#include "../TypeDefinition/GMS/Constants.hpp"
#include "../TypeDefinition/GMS/GMSType.hpp"

BasicGMS::BasicGMS(UA_Server *pServer) : InstantiatedMachineTool(pServer) {
  MachineName = "BasicGMS";
  CreateObject();
}

void BasicGMS::CreateObject() {
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
    UmatiServerLib::Bind::MembersRefl(mt, m_pServer, m_nodeId, n);
    InstantiateIdentification();
    InstantiateOptional(mt.ResultManagement->Results, m_pServer, n);
    auto &result = mt.ResultManagement->Results->ResultVariable.Add<machinery_result::ResultType_t>(m_pServer, n, {m_nsIndex, "Result1"});
    UA_init(&result.Value.value,&UA_TYPES_MACHINERY_RESULT[UA_TYPES_MACHINERY_RESULT_RESULTDATATYPE]);
    result.Value->resultMetaData.resultId = UA_String_fromChars("test");


    InstantiateMonitoring();
  InstantiateProduction();
}

void BasicGMS::InstantiateIdentification() {
    {
        std::stringstream ss;
        ss << "http://www.isw.uni-stuttgart.de/#" << MachineName;
        mt.Identification->ProductInstanceUri = ss.str();
    }

    mt.Identification->Manufacturer = {"", "ISW Sebastian Friedl"};

    {
        std::hash<std::string> hasher;
        std::stringstream ss;
        ss << "2022-" << hasher(MachineName);
        mt.Identification->SerialNumber = ss.str();
    }
}

void BasicGMS::InstantiateMonitoring() {
  mt.Monitoring->MachineTool->OperationMode = UA_MachineOperationMode::UA_MACHINEOPERATIONMODE_AUTOMATIC;
}

void BasicGMS::InstantiateProduction() {
  InstantiateOptional(mt.Production->ActiveProgram->State, m_pServer, n);
  mt.Production->ActiveProgram->NumberInList = 0;
  mt.Production->ActiveProgram->Name = "Basic Program";
  mt.Production->ActiveProgram->State->CurrentState->Value = {"en", "Running"};
  mt.Production->ActiveProgram->State->CurrentState->Number = 1;
  mt.Production->ActiveProgram->State->CurrentState->Id =
    UA_NODEID_NUMERIC(nsFromUri(m_pServer, constants::NsMachineToolUri), UA_MACHINETOOLID_PRODUCTIONSTATEMACHINETYPE_RUNNING);
}

void BasicGMS::Simulate() {
  ++m_simStep;
  if ((m_simStep % 2) == 1) {
    SimulateStacklight();
  }
  if ((m_simStep % 10) < 8) {
      mt.Production->ActiveProgram->State->CurrentState->Value = {"en", "Running"};
      mt.Production->ActiveProgram->State->CurrentState->Number = 1;
      mt.Production->ActiveProgram->State->CurrentState->Id =
              UA_NODEID_NUMERIC(nsFromUri(m_pServer, constants::NsMachineToolUri), UA_MACHINETOOLID_PRODUCTIONSTATEMACHINETYPE_RUNNING);
  } else {
      mt.Production->ActiveProgram->State->CurrentState->Value = {"en", "Interrupted"};
      mt.Production->ActiveProgram->State->CurrentState->Number = 3;
      mt.Production->ActiveProgram->State->CurrentState->Id =
              UA_NODEID_NUMERIC(nsFromUri(m_pServer, constants::NsMachineToolUri), UA_MACHINETOOLID_PRODUCTIONSTATEMACHINETYPE_INTERRUPTED);
  }

  mt.Monitoring->MachineTool->PowerOnDuration = m_simStep / 3600;
}
