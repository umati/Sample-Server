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
#include "../TypeDefinition/TypeDefinition.hpp"

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
  InstantiateEquipment();
  InstantiateNotification();
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

  InstantiateOptional(gms.Identification->SubDeviceClass, m_pServer, n);
  InstantiateOptional(gms.Identification->Workspace, m_pServer, n);

  gms.Identification->SubDeviceClass = "CoordinateMeasuringMachine";
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
void FullGMS::InstantiateEquipment() {
  InstantiateOptional(gms.Equipment->Tools, m_pServer, n);

  auto &tool1 = gms.Equipment->Tools->Tool.Add<machineTool::Tool_t>(m_pServer, n, {m_nsIndex, "Tool 1"});
  tool1.ControlIdentifier1 = 12;
  tool1.ControlIdentifierInterpretation = UA_ToolManagement::UA_TOOLMANAGEMENT_NUMBERBASED;
  tool1.Name = "My Tool";
  InstantiateOptional(tool1.Name, m_pServer, n);

  auto &sensor1 = gms.Equipment->Tools->Tool.Add<GMS::GMSSensor_t>(m_pServer, n, {m_nsIndex, "Sensor 1"});
  sensor1.Class = 2; /* TactileTouchTrigger */
  sensor1.ControlIdentifier1 = 11;
  sensor1.ControlIdentifier2 = 3;
  sensor1.ControlIdentifierInterpretation = UA_ToolManagement::UA_TOOLMANAGEMENT_GROUPBASED;
  sensor1.Locked->Value = true;
  sensor1.Locked->ReasonForLocking = UA_ToolLocked::UA_TOOLLOCKED_BYOPERATOR;
  sensor1.Name = "My Sensor";
  InstantiateOptional(sensor1.ControlIdentifier2, m_pServer, n);
  InstantiateOptional(sensor1.Name, m_pServer, n);

  // Additional SensorType members
  InstantiateOptional(sensor1.Alignment, m_pServer, n);
  sensor1.Alignment = UA_ToolAlignmentState::UA_TOOLALIGNMENTSTATE_INDEXED;

  InstantiateOptional(sensor1.Axes, m_pServer, n);
  sensor1.Axes->push_back("X,Y,Z");

  InstantiateOptional(sensor1.Capabilities, m_pServer, n);
  sensor1.Capabilities->push_back(UA_ToolCapabilities::UA_TOOLCAPABILITIES_PTMEAS);

  InstantiateOptional(sensor1.IsQualifiedStatus, m_pServer, n);
  sensor1.IsQualifiedStatus = UA_ToolIsQualifiedStatus::UA_TOOLISQUALIFIEDSTATUS_QUALIFIED;

  // EUInformation for Days according to CEFACT
  const UmatiServerLib::EUInformation_t EU_Days{
    .NamespaceUri = "http://www.opcfoundation.org/UA/units/un/cefact", .UnitId = 4473177, .DisplayName = {"", "d"}, .Description = {"", "days"}};

  sensor1.ToolLife->Qualified->EngineeringUnits = EU_Days;
  sensor1.ToolLife->Qualified->IsCountingUp = true;
  sensor1.ToolLife->Qualified->StartValue = 0;
  sensor1.ToolLife->Qualified->WarningValue = 5;
  sensor1.ToolLife->Qualified->LimitValue = 7;
  sensor1.ToolLife->Qualified->Value = 1;
  InstantiateOptional(sensor1.ToolLife, m_pServer, n);
  InstantiateOptional(sensor1.ToolLife->Qualified, m_pServer, n);
  InstantiateOptional(sensor1.ToolLife->Qualified->Value, m_pServer, n);
  InstantiateOptional(sensor1.ToolLife->Qualified->StartValue, m_pServer, n);
  InstantiateOptional(sensor1.ToolLife->Qualified->WarningValue, m_pServer, n);
  InstantiateOptional(sensor1.ToolLife->Qualified->LimitValue, m_pServer, n);
}

void FullGMS::InstantiateNotification() {
  InstantiateOptional(gms.Notification->Prognoses, m_pServer, n);
  InstantiateOptional(gms.Notification->Prognoses->Calibration, m_pServer, n);
  gms.Notification->Prognoses->Calibration.value.Calibrated = true;
  InstantiateOptional(gms.Notification->Prognoses->Calibration->CalibrationCertificate, m_pServer, n);
  {
    std::stringstream ss;
    ss << "https://www.ptb.de/dcc/#" << MachineName;
    gms.Notification->Prognoses->Calibration.value.CalibrationCertificate->push_back(ss.str());
  }

  InstantiateOptional(gms.Notification->Prognoses->Calibration->CalibrationInterval, m_pServer, n);
  gms.Notification->Prognoses->Calibration.value.CalibrationInterval = 1000.0 * 60 * 60 * 24 * 356;
  InstantiateOptional(gms.Notification->Prognoses->Calibration->CalibrationPreptime, m_pServer, n);
  gms.Notification->Prognoses->Calibration.value.CalibrationPreptime = 1000.0 * 60 * 60 * 24 * 30;
  gms.Notification->Prognoses->Calibration.value.DateOfCalibration = std::chrono::system_clock::now() - std::chrono::hours(24 * 7);
  gms.Notification->Prognoses->Calibration.value.PredictedTime = std::chrono::system_clock::now() + std::chrono::hours(24 * 356);
}
