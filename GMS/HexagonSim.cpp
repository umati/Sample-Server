/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2022 (c) Sebastian Friedl, ISW University of Stuttgart (for VDMA e.V.)
 */

#include "HexagonSim.hpp"

#include <string>

#include "../TypeDefinition/GMS/Constants.hpp"
#include "../TypeDefinition/GMS/GMSType.hpp"

HexagonSim::HexagonSim(UA_Server *pServer) : InstantiatedMachineTool(pServer) {
  MachineName = "Hexagon PMM Gold";
  CreateObject();
}

void HexagonSim::CreateObject() {
  std::stringstream ss;
  ss << "https://www.hexagonmi.com/" << MachineName << "/";
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
  InstantiateMonitoring();
  InstantiateProduction();
  InstantiateTools();
  InstantiateResultManagement();

  InstantiateOptional(mt.Notification->Prognoses, m_pServer, n);
  InstantiateOptional(mt.Notification->Prognoses->Calibration, m_pServer, n);
  mt.Notification->Prognoses->Calibration.value.Calibrated = true;
  InstantiateOptional(mt.Notification->Prognoses->Calibration->CalibrationInterval, m_pServer, n);
  mt.Notification->Prognoses->Calibration.value.CalibrationInterval = 1000.0 * 60 * 60 * 24 * 356;
  InstantiateOptional(mt.Notification->Prognoses->Calibration->CalibrationPreptime, m_pServer, n);
  mt.Notification->Prognoses->Calibration.value.CalibrationPreptime = 1000.0 * 60 * 60 * 24 * 30;
  mt.Notification->Prognoses->Calibration.value.DateOfCalibration = std::chrono::system_clock::now() - std::chrono::hours(24 * 7);
  mt.Notification->Prognoses->Calibration.value.PredictedTime = std::chrono::system_clock::now() + std::chrono::hours(24 * 356);
}

void HexagonSim::InstantiateResultManagement() {
  InstantiateOptional(mt.ResultManagement->Results, m_pServer, n);
  auto &result = mt.ResultManagement->Results->ResultVariable.Add<machinery_result::ResultType_t>(m_pServer, n, {m_nsIndex, "Result"});
  UA_init(&result.Value.value, &UA_TYPES_MACHINERY_RESULT[UA_TYPES_MACHINERY_RESULT_RESULTDATATYPE]);
  result.Value->resultMetaData.resultId = UA_String_fromChars("-");
  result.Value->resultMetaData.resultUri = UA_String_new();
  // UA_Variant_init(result.Value->resultMetaData.resultUri);
  UA_String_init(result.Value->resultMetaData.resultUri);
  *result.Value->resultMetaData.resultUri = UA_String_fromChars("https://www.hexagonmi.com/report.pdf");
  result.Value->resultMetaData.resultUriSize = 1;
  result.Value->resultMetaData.fileFormat = UA_String_new();
  UA_String_init(result.Value->resultMetaData.fileFormat);
  *result.Value->resultMetaData.fileFormat = UA_String_fromChars("PDF");
  result.Value->resultMetaData.fileFormatSize = 1;
  result.Value->resultMetaData.resultState = UA_Int32_new();
  UA_Int32_init(result.Value->resultMetaData.resultState);
  *result.Value->resultMetaData.resultState = 1;
}

void HexagonSim::InstantiateIdentification() {
  {
    std::stringstream ss;
    ss << "https://www.hexagonmi.com/products/coordinate-measuring-machines/bridge-cmms/ultra-high-accuracy-cmms/leitz-pmm-gold";
    mt.Identification->ProductInstanceUri = ss.str();
  }

  InstantiateOptional(mt.Identification->YearOfConstruction, m_pServer, n);
  InstantiateOptional(mt.Identification->ProductCode, m_pServer, n);
  InstantiateOptional(mt.Identification->SoftwareRevision, m_pServer, n);
  InstantiateOptional(mt.Identification->DeviceClass, m_pServer, n);
  InstantiateOptional(mt.Identification->Location, m_pServer, n);
  InstantiateOptional(mt.Identification->Model, m_pServer, n);

  mt.Identification->Manufacturer = {"", "Hexagon Manufacturing Intelligence"};
  mt.Identification->ProductCode = "PMM_Gold_0104";
  mt.Identification->YearOfConstruction = 2021;
  mt.Identification->SoftwareRevision = "Quindos7 - V 7.22.22096";
  mt.Identification->DeviceClass = "Coordinate Measuring Machine";
  mt.Identification->Location = "JIMTOF E7 E7116/VIRTUAL 1 1/N 50.5514144 E 8.4764728";
  mt.Identification->Model = {"", MachineName};
  mt.Identification->SerialNumber = "0104";
}

void HexagonSim::InstantiateMonitoring() {
  InstantiateOptional(mt.Monitoring->MachineTool->PowerOnDuration, m_pServer, n);
  InstantiateOptional(mt.Monitoring->MachineTool->FeedOverride, m_pServer, n);
  mt.Monitoring->MachineTool->OperationMode = UA_MachineOperationMode::UA_MACHINEOPERATIONMODE_AUTOMATIC;
}

void HexagonSim::InstantiateProduction() {
  InstantiateOptional(mt.Production->ActiveProgram->State, m_pServer, n);
  mt.Production->ActiveProgram->NumberInList = 0;
  mt.Production->ActiveProgram->Name = "Basic Measurment Program";
  mt.Production->ActiveProgram->State->CurrentState->Value = {"en", "Running"};
  mt.Production->ActiveProgram->State->CurrentState->Number = 1;
  mt.Production->ActiveProgram->State->CurrentState->Id =
    UA_NODEID_NUMERIC(nsFromUri(m_pServer, constants::NsMachineToolUri), UA_MACHINETOOLID_PRODUCTIONSTATEMACHINETYPE_RUNNING);
}

void HexagonSim::InstantiateTools() {
  InstantiateOptional(mt.Equipment->Tools, m_pServer, n);
  InstantiateOptional(mt.Equipment->Tools->NodeVersion, m_pServer, n);
  n.Remove(mt.Equipment->Tools->NodeVersion.NodeId);
  // TODO Warning: This is only the Machine Tool tool not a sensor of the GMS
  for (size_t i = 1; i <= 1; ++i) {
    std::stringstream ss;
    ss << "Sensor" << i;
    auto &tool = mt.Equipment->Tools->Tool.Add<machineTool::Tool_t>(m_pServer, n, {m_nsIndex, ss.str()});
    tool.ControlIdentifier1 = i * 10 + 2;
    tool.ControlIdentifierInterpretation = UA_ToolManagement::UA_TOOLMANAGEMENT_NUMBERBASED;
    tool.Locked->Value = false;
    tool.Locked->ReasonForLocking = UA_ToolLocked::UA_TOOLLOCKED_OTHER;
    tool.Name = ss.str();
    InstantiateOptional(tool.Name, m_pServer, n);
  }
}

void HexagonSim::Simulate() {
  ++m_simStep;
  int i = m_simStep;
  if ((m_simStep % 2) == 1) {
    SimulateStacklight();
  }

  {
    mt.Production->ActiveProgram->State->CurrentState->Value = {"en", "Running"};
    mt.Production->ActiveProgram->State->CurrentState->Number = 1;
    mt.Production->ActiveProgram->State->CurrentState->Id =
      UA_NODEID_NUMERIC(nsFromUri(m_pServer, constants::NsMachineToolUri), UA_MACHINETOOLID_PRODUCTIONSTATEMACHINETYPE_RUNNING);
  }

  if ((m_simStep % 10) == 8) {
    int i = m_simStep;
    for (auto &corr : mt.ResultManagement->CorrectionsFolder->Corrections.value) {
      corr->CorrectionValueAbsolute->Value = round(1.3 * (sin(i)) * 10000) / 100;
      i = (i + (m_simStep + (int)corr->CorrectionValueAbsolute->Value.value * 452) % 18 * 100);
    }
    auto &result = mt.ResultManagement->Results->ResultVariable->front();
    UA_String_clear(&result->Value->resultMetaData.resultId);
    result->Value->resultMetaData.resultId = UA_String_fromChars(std::to_string(i * 123 % 100).c_str());
  }

  mt.Monitoring->MachineTool->PowerOnDuration = m_simStep / 3600;

  mt.Monitoring->MachineTool->FeedOverride->Value = 100.0;
  mt.Monitoring->MachineTool->FeedOverride->EURange->high = 100.0;
  mt.Monitoring->MachineTool->FeedOverride->EURange->low = 0.0;
  mt.Monitoring->MachineTool->FeedOverride->EngineeringUnits->DisplayName = {"", "%"};
}
