/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2020-2021 Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 * Copyright (c) 2020-2022 Götz Görisch, VDW - Verein Deutscher Werkzeugmaschinenfabriken e.V.
 */
#include "ShowcaseMachineTool.hpp"

ShowcaseMachineTool::ShowcaseMachineTool(UA_Server *pServer) : ShowcaseMachineTool::ShowcaseMachineTool(pServer, {}) {}

ShowcaseMachineTool::ShowcaseMachineTool(UA_Server *pServer, MqttSettings mqttSettings) : InstantiatedMachineTool(pServer, mqttSettings) {
  MachineName = "ShowcaseMachineTool";
  CreateObject();
}

void ShowcaseMachineTool::CreateObject() {
  InstantiatedMachineTool::CreateObject();
  InstantiateIdentification();
  InstantiateMonitoring();
  InstantiateTools();
  InstantiateProduction();
  if (m_mqttSettings.connectionIdent != nullptr) {
    m_publisher.Publish(mt, m_pServer, m_mqttSettings.connectionIdent, n, m_mqttSettings.prefix, m_mqttSettings.publisherId, "MachineTool", 2000);
  }
}

void ShowcaseMachineTool::InstantiateIdentification() {
  InstantiatedMachineTool::InstantiateIdentification();
  InstantiateOptional(mt.Identification->YearOfConstruction, m_pServer, n);
  InstantiateOptional(mt.Identification->ProductCode, m_pServer, n);
  InstantiateOptional(mt.Identification->SoftwareRevision, m_pServer, n);
  InstantiateOptional(mt.Identification->DeviceClass, m_pServer, n);
  InstantiateOptional(mt.Identification->Location, m_pServer, n);
  InstantiateOptional(mt.Identification->Model, m_pServer, n);
  InstantiateOptional(mt.Identification->ComponentName, m_pServer, n);
  InstantiateOptional(mt.Identification->MonthOfConstruction, m_pServer, n);


  mt.Identification->Manufacturer = {"", "umati Showcase"};
  mt.Identification->ComponentName = {"", "umati Showcase Component"};
  mt.Identification->ProductCode = "2653837gg1548";
  mt.Identification->YearOfConstruction = 2021;
  mt.Identification->MonthOfConstruction = 11;
  mt.Identification->SoftwareRevision = "v1.02.1";
  mt.Identification->DeviceClass = "Machining centre (other)";
  mt.Identification->ProductInstanceUri = "https://showcase.umati.org/Specs/Machinetools.html";
  mt.Identification->Location = "METAV 16 F02/VIRTUAL 0 0/N 49.871215 E 8.654204";
  mt.Identification->Model = {"", MachineName};
}

void ShowcaseMachineTool::InstantiateMonitoring() {
  InstantiateMonitoringMT();
  InstantiateMonitoringStacklight({UA_SignalColor::UA_SIGNALCOLOR_RED, UA_SignalColor::UA_SIGNALCOLOR_YELLOW, UA_SignalColor::UA_SIGNALCOLOR_GREEN});

  InstantiateMonitoringChannel(1);

  auto &spindle = mt.Monitoring->MonitoredElement.Add<machineTool::SpindleMonitoring_t>(m_pServer, n, {m_nsIndex, "Spindle"});
  InstantiateOptional(spindle.Override, m_pServer, n);
  InstantiateOptional(spindle.IsUsedAsAxis, m_pServer, n);
  spindle.Override->Value = 103.0;
  spindle.Override->EURange->high = 125.0;
  spindle.Override->EURange->low = 0.0;
  spindle.Override->EngineeringUnits->DisplayName = {"", "%"};
  spindle.IsRotating = true;
  spindle.Name = "Spindle";
  spindle.IsUsedAsAxis = false;
}

void ShowcaseMachineTool::InstantiateTools() {
  InstantiateOptional(mt.Equipment->Tools, m_pServer, n);
  InstantiateOptional(mt.Equipment->Tools->NodeVersion, m_pServer, n);
  n.Remove(mt.Equipment->Tools->NodeVersion.NodeId);
  for (size_t i = 1; i <= 1; ++i) {
    std::stringstream ss;
    ss << "Tool" << i;
    auto &tool = mt.Equipment->Tools->Tool.Add<machineTool::Tool_t>(m_pServer, n, {m_nsIndex, ss.str()});
    tool.ControlIdentifier1 = i * 10 + 2;
    tool.ControlIdentifierInterpretation = UA_ToolManagement::UA_TOOLMANAGEMENT_NUMBERBASED;
    tool.Locked->Value = false;
    tool.Locked->ReasonForLocking = UA_ToolLocked::UA_TOOLLOCKED_OTHER;
    tool.Name = ss.str();
    InstantiateOptional(tool.Name, m_pServer, n);
  }
}

void ShowcaseMachineTool::InstantiateProduction() {
  InstantiateOptional(mt.Production->ActiveProgram->State, m_pServer, n);
  mt.Production->ActiveProgram->NumberInList = 0;
  mt.Production->ActiveProgram->Name = "Basic Program";
  mt.Production->ActiveProgram->State->CurrentState->Value = {"en", "Running"};
  mt.Production->ActiveProgram->State->CurrentState->Number = 1;
  mt.Production->ActiveProgram->State->CurrentState->Id =
    UA_NODEID_NUMERIC(nsFromUri(m_pServer, constants::NsMachineToolUri), UA_MACHINETOOLID_PRODUCTIONSTATEMACHINETYPE_RUNNING);
}

void ShowcaseMachineTool::Simulate() {
  ++m_simStep;
  if ((m_simStep % 2) == 1) {
    SimulateStacklight();
  }

  mt.Monitoring->MachineTool->PowerOnDuration = m_simStep / 3600;
}
