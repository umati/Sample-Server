/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2022 (c) Sebastian Friedl, ISW University of Stuttgart (for VDMA e.V.)
 */

#include "WenzelLH.hpp"

#include <string>

#include "../TypeDefinition/GMS/Constants.hpp"
#include "../TypeDefinition/GMS/GMSType.hpp"

WenzelLH::WenzelLH(UA_Server *pServer) : InstantiatedMachineTool(pServer) {
  MachineName = "Wenzel LH 87";
  CreateObject();
}

void WenzelLH::CreateObject() {
  std::stringstream ss;
  ss << "http://www.wenzel-group.com/" << MachineName << "/";
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
  InstantiateOptional(mt.Notification->Prognoses->Calibration->CalibrationCertificate, m_pServer, n);
  {
    std::stringstream ss;
    ss << "https://www.ptb.de/dcc/#" << MachineName;
    mt.Notification->Prognoses->Calibration.value.CalibrationCertificate->push_back(ss.str());
  }

  InstantiateOptional(mt.Notification->Prognoses->Calibration->CalibrationInterval, m_pServer, n);
  mt.Notification->Prognoses->Calibration.value.CalibrationInterval = 1000.0 * 60 * 60 * 24 * 356;
  InstantiateOptional(mt.Notification->Prognoses->Calibration->CalibrationPreptime, m_pServer, n);
  mt.Notification->Prognoses->Calibration.value.CalibrationPreptime = 1000.0 * 60 * 60 * 24 * 30;
  mt.Notification->Prognoses->Calibration.value.DateOfCalibration = std::chrono::system_clock::now() - std::chrono::hours(24 * 7);
  mt.Notification->Prognoses->Calibration.value.PredictedTime = std::chrono::system_clock::now() + std::chrono::hours(24 * 356);
}

void WenzelLH::InstantiateResultManagement() {
  InstantiateOptional(mt.ResultManagement->Results, m_pServer, n);
  auto &result = mt.ResultManagement->Results->ResultVariable.Add<machinery_result::ResultType_t>(m_pServer, n, {m_nsIndex, "Result"});
  UA_init(&result.Value.value, &UA_TYPES_MACHINERY_RESULT[UA_TYPES_MACHINERY_RESULT_RESULTDATATYPE]);
  result.Value->resultMetaData.resultId = UA_String_fromChars("-");
  result.Value->resultMetaData.resultUri = UA_String_new();
  // UA_Variant_init(result.Value->resultMetaData.resultUri);
  UA_String_init(result.Value->resultMetaData.resultUri);
  *result.Value->resultMetaData.resultUri = UA_String_fromChars("http://www.wenzel-group.com/result");
  result.Value->resultMetaData.resultUriSize = 1;
  result.Value->resultMetaData.fileFormat = UA_String_new();
  UA_String_init(result.Value->resultMetaData.fileFormat);
  *result.Value->resultMetaData.fileFormat = UA_String_fromChars("PDF");
  result.Value->resultMetaData.fileFormatSize = 1;
  result.Value->resultMetaData.resultState = UA_Int32_new();
  UA_Int32_init(result.Value->resultMetaData.resultState);
  *result.Value->resultMetaData.resultState = 1;

  InstantiateOptional(mt.ResultManagement->CorrectionsFolder, m_pServer, n);
  auto &corr1 = mt.ResultManagement->CorrectionsFolder->Corrections.Add<GMS::CorrectionType_t>(m_pServer, n, {m_nsIndex, "Corr1"});
  initCorrection(corr1, "corr1", "c_123", 1.0);

  auto &corr2 = mt.ResultManagement->CorrectionsFolder->Corrections.Add<GMS::CorrectionType_t>(m_pServer, n, {m_nsIndex, "Corr2"});
  initCorrection(corr2, "corr2", "c_123", 0.5);

  auto &corr3 = mt.ResultManagement->CorrectionsFolder->Corrections.Add<GMS::CorrectionType_t>(m_pServer, n, {m_nsIndex, "Corr3"});
  initCorrection(corr3, "corr3", "c_456", 0.75);
}

void WenzelLH::initCorrection(GMS::CorrectionType_t &corr, std::string Identifier, std::string CharacteristicIdentfier, double value) {
  corr.Identifier = Identifier;
  corr.CharacteristicIdentfier = CharacteristicIdentfier;
  InstantiateOptional(corr.CorrectionValueAbsolute, m_pServer, n);
  corr.CorrectionValueAbsolute->Value = value;
  corr.CorrectionValueAbsolute->EngineeringUnits.value.DisplayName.text = "µm";
  corr.CorrectionValueAbsolute->EngineeringUnits.value.Description.text = "micrometre (micron)";
  corr.CorrectionValueAbsolute->EngineeringUnits.value.UnitId = 13384;
  corr.CorrectionValueAbsolute->EngineeringUnits.value.NamespaceUri = "http://www.opcfoundation.org/UA/units/un/cefact";
  InstantiateOptional(corr.LowerControlLimit, m_pServer, n);
  corr.LowerControlLimit->Value = -100.0;
  corr.LowerControlLimit->EngineeringUnits.value.DisplayName.text = "µm";
  corr.LowerControlLimit->EngineeringUnits.value.Description.text = "micrometre (micron)";
  corr.LowerControlLimit->EngineeringUnits.value.UnitId = 13384;
  corr.LowerControlLimit->EngineeringUnits.value.NamespaceUri = "http://www.opcfoundation.org/UA/units/un/cefact";
  InstantiateOptional(corr.UpperControlLimit, m_pServer, n);
  corr.UpperControlLimit->Value = 100.0;
  corr.UpperControlLimit->EngineeringUnits.value.DisplayName.text = "µm";
  corr.UpperControlLimit->EngineeringUnits.value.Description.text = "micrometre (micron)";
  corr.UpperControlLimit->EngineeringUnits.value.UnitId = 13384;
  corr.UpperControlLimit->EngineeringUnits.value.NamespaceUri = "http://www.opcfoundation.org/UA/units/un/cefact";
}

void WenzelLH::InstantiateIdentification() {
  {
    std::stringstream ss;
    ss << "http://www.wenzel-group.com/#" << MachineName;
    mt.Identification->ProductInstanceUri = ss.str();
  }

  InstantiateOptional(mt.Identification->YearOfConstruction, m_pServer, n);
  InstantiateOptional(mt.Identification->ProductCode, m_pServer, n);
  InstantiateOptional(mt.Identification->SoftwareRevision, m_pServer, n);
  InstantiateOptional(mt.Identification->DeviceClass, m_pServer, n);
  InstantiateOptional(mt.Identification->Location, m_pServer, n);
  InstantiateOptional(mt.Identification->Model, m_pServer, n);

  mt.Identification->Manufacturer = {"", "Wenzel Metrology GmbH"};
  mt.Identification->ProductCode = "Wenzel LH87";
  mt.Identification->YearOfConstruction = 2022;
  mt.Identification->SoftwareRevision = "V1.0.0";
  mt.Identification->DeviceClass = "GMS";
  mt.Identification->Location = "CTRL 5 5101/VIRTUAL 0 0/N 48.694241 E 9.183833";
  mt.Identification->Model = {"", MachineName};

  {
    std::hash<std::string> hasher;
    std::stringstream ss;
    ss << "2022-" << hasher(MachineName);
    mt.Identification->SerialNumber = ss.str();
  }
}

void WenzelLH::InstantiateMonitoring() {
  InstantiateOptional(mt.Monitoring->MachineTool->PowerOnDuration, m_pServer, n);
  InstantiateOptional(mt.Monitoring->MachineTool->FeedOverride, m_pServer, n);
  mt.Monitoring->MachineTool->OperationMode = UA_MachineOperationMode::UA_MACHINEOPERATIONMODE_AUTOMATIC;
}

void WenzelLH::InstantiateProduction() {
  InstantiateOptional(mt.Production->ActiveProgram->State, m_pServer, n);
  mt.Production->ActiveProgram->NumberInList = 0;
  mt.Production->ActiveProgram->Name = "Measurment Gear";
  mt.Production->ActiveProgram->State->CurrentState->Value = {"en", "Running"};
  mt.Production->ActiveProgram->State->CurrentState->Number = 1;
  mt.Production->ActiveProgram->State->CurrentState->Id =
    UA_NODEID_NUMERIC(nsFromUri(m_pServer, constants::NsMachineToolUri), UA_MACHINETOOLID_PRODUCTIONSTATEMACHINETYPE_RUNNING);
}

void WenzelLH::InstantiateTools() {
  InstantiateOptional(mt.Equipment->Tools, m_pServer, n);
  InstantiateOptional(mt.Equipment->Tools->NodeVersion, m_pServer, n);
  n.Remove(mt.Equipment->Tools->NodeVersion.NodeId);
  // TODO Warning: This is only the Machine Tool tool not a sensor of the GMS
  for (size_t i = 1; i <= 1; ++i) {
    std::stringstream ss;
    ss << "Camera" << i;
    auto &tool = mt.Equipment->Tools->Tool.Add<machineTool::Tool_t>(m_pServer, n, {m_nsIndex, ss.str()});
    tool.ControlIdentifier1 = i * 10 + 2;
    tool.ControlIdentifierInterpretation = UA_ToolManagement::UA_TOOLMANAGEMENT_NUMBERBASED;
    tool.Locked->Value = true;
    tool.Locked->ReasonForLocking = UA_ToolLocked::UA_TOOLLOCKED_OTHER;
    tool.Name = ss.str();
    InstantiateOptional(tool.Name, m_pServer, n);
  }
}

void WenzelLH::Simulate() {
  ++m_simStep;
  int i = m_simStep;
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
