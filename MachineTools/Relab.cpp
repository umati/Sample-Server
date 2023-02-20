/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2020-2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 * Copyright 2020-2021 (c) Götz Görisch, VDW - Verein Deutscher Werkzeugmaschinenfabriken e.V.
 */
#include <variant>
#include "Relab.hpp"

#include <windows.h>

Relab::Relab(UA_Server *pServer, AdsSettings adsSettings) : Relab::Relab(pServer, {}, adsSettings) {}

Relab::Relab(UA_Server *pServer, MqttSettings mqttSettings, AdsSettings adsSettings) : InstantiatedMachineTool(pServer, mqttSettings) {
  try {
    MachineName = "Relab";
    // Kommunikationsport auf dem ADS Router öffnen
    AmsNetId localNetId = adsSettings.LocalNetId;
    route = new AdsDevice({remoteAddr.c_str(), Addr.netId, AMSPORT_R0_PLC_TC3});
    AdsSetLocalAddress(localNetId);
    nPort = AdsPortOpenEx();
    nErr = AdsGetLocalAddressEx(nPort, &Addr);
    remoteAddr = adsSettings.RemoteNetIdStr;
    if (nErr) std::cerr << "Error: AdsGetLocalAddress: " << nErr << '\n';
    // TwinCAT2 RTS1 Port = 801
    Addr.port = adsSettings.RemotePort;
    Addr.netId =  adsSettings.RemoteNetId;
  } catch (AdsException ex) {
    std::cout << ex.what();
    exit(1);
  }
  CreateObject();
}


void Relab::CreateObject() {
  InstantiatedMachineTool::CreateObject();
  InstantiateIdentification();
  InstantiateMonitoring();
  InstantiateTools();
  InstantiateProduction();
  if (m_mqttSettings.connectionIdent != nullptr) {
    std::cout << "Setting up publisher " << m_mqttSettings.connectionIdent->namespaceIndex << '\n';
    m_publisher.Publish(mt, m_pServer, m_mqttSettings.connectionIdent, n, m_mqttSettings.prefix, m_mqttSettings.publisherId, "MachineTool", 2000);
  }
}

void Relab::InstantiateIdentification() {
  InstantiatedMachineTool::InstantiateIdentification();
  InstantiateOptional(mt.Identification->YearOfConstruction, m_pServer, n);
  InstantiateOptional(mt.Identification->ProductCode, m_pServer, n);
  InstantiateOptional(mt.Identification->SoftwareRevision, m_pServer, n);
  InstantiateOptional(mt.Identification->DeviceClass, m_pServer, n);
  InstantiateOptional(mt.Identification->Location, m_pServer, n);
  InstantiateOptional(mt.Identification->Model, m_pServer, n);

  mt.Identification->Manufacturer = {"", "umati Showcase"};
  mt.Identification->ProductCode = "2653837gg1548";
  mt.Identification->YearOfConstruction = 2021;
  mt.Identification->SoftwareRevision = "v1.02.1";
  mt.Identification->DeviceClass = "Machining centre (other)";
  mt.Identification->ProductInstanceUri = "https://showcase.umati.org/Specs/Machinetools.html";
  mt.Identification->Location = "CIMT E8 B014/VIRTUAL 0 0/N 49.871215 E 8.654204";
  mt.Identification->Model = {"", MachineName};
}

void Relab::InstantiateMonitoring() {
  InstantiateMonitoringMT();
  // InstantiateMonitoringStacklight({UA_SignalColor::UA_SIGNALCOLOR_RED, UA_SignalColor::UA_SIGNALCOLOR_YELLOW, UA_SignalColor::UA_SIGNALCOLOR_GREEN});
  auto &lsr = mt.Monitoring->MonitoredElement.Add<machineTool::LaserMonitoring_t>(m_pServer, n, {m_nsIndex, "Laser"});
  lsr.ControllerIsOn = false;
  lsr.Name = "Laser";
  lsr.LaserState = UA_LaserState::UA_LASERSTATE_READY;




  // Werte aus MD0 auslesen und anzeigen
  /* InstantiateMonitoringChannel(1);

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
  */
}

void Relab::InstantiateTools() {
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

void Relab::InstantiateProduction() {
  InstantiateOptional(mt.Production->ActiveProgram->State, m_pServer, n);
  mt.Production->ActiveProgram->NumberInList = 0;
  mt.Production->ActiveProgram->Name = "Basic Program";
  mt.Production->ActiveProgram->State->CurrentState->Value = {"en", "Running"};
  mt.Production->ActiveProgram->State->CurrentState->Number = 1;
  mt.Production->ActiveProgram->State->CurrentState->Id =
    UA_NODEID_NUMERIC(nsFromUri(m_pServer, constants::NsMachineToolUri), UA_MACHINETOOLID_PRODUCTIONSTATEMACHINETYPE_RUNNING);
}

void Relab::Simulate() {
  ++m_simStep;
  /*
  if ((m_simStep % 2) == 1) {
    SimulateStacklight();
  }
  
  mt.Monitoring->MachineTool->PowerOnDuration = m_simStep / 3600;
  */
  try  {
    AdsVariable<int16_t> readVar {*route, "GVL_Main.laser_power_potty"};
    auto potLsr = std::get_if<machineTool::LaserMonitoring_t>(&mt.Monitoring->MonitoredElement.value.front().value);
    potLsr->LaserState.value = UA_LaserState((int32_t) readVar);
  }
  catch (AdsException ex) {
    std::cout << "ADS read error " << ex.what() << '\n';
  }
}
