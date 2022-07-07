/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2020-2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 * Copyright 2020 (c) Tonja Heinemann, ISW University of Stuttgart (for umati and VDW e.V.)
 * Copyright 2020-2021 (c) Götz Görisch, VDW - Verein Deutscher Werkzeugmaschinenfabriken e.V.
 */

#include "FullMachineTool.hpp"

#include <open62541/types_generated.h>

#include <sstream>

#include "../TypeDefinition/MachineTool/Alert.hpp"
#include "../TypeDefinition/MachineTool/NotificationEvent.hpp"
#include "../UmatiServerLib/BindRefl.hpp"
#include "../UmatiServerLib/OpcUaCondition.hpp"
#include "../UmatiServerLib/ServerHelper.hpp"
#include "../arch/gmtime.hpp"

FullMachineTool::FullMachineTool(UA_Server *pServer) : FullMachineTool(pServer, true, MqttSettings{}) {}

FullMachineTool::FullMachineTool(UA_Server *pServer, bool initialize, MqttSettings mqttSettings)
  : InstantiatedMachineTool(pServer, mqttSettings), JobStateMachine(mt.Production->ActiveProgram->State.value, pServer) {
  if (initialize) {
    MachineName = "FullMachineTool";
    CreateObject();
  }
}

void FullMachineTool::CreateObject() {
  InstantiatedMachineTool::CreateObject();

  InstantiateIdentification();
  InstantiateMonitoring();
  InstantiateTools();
  InstantiatePrognosis();
  InstantiateProduction();

  InstantiateOptional(mt.Notification->Messages, m_pServer, n);
  writeEventNotifier(m_pServer, mt.Notification->Messages.NodeId);
}

void FullMachineTool::InstantiateProduction() {
  /*
  if (m_mqttSettings.connectionIdent != nullptr) {
    std::string topic = m_mqttSettings.prefix + "/json/data/" + m_mqttSettings.publisherId + "/Production_WriterGroup";  
    auto retval = addWriterGroup(m_pServer, const_cast<char*>(topic.c_str()), 2000, &m_mqttSettings.productionWriterGroupIdent, m_mqttSettings.connectionIdent);
    if (retval != UA_STATUSCODE_GOOD) {
      std::cout << "Error adding WriterGroup " << UA_StatusCode_name(retval) << "\n";
    }
  }
  */

  InstantiateOptional(mt.Production->ActiveProgram->State, m_pServer, n);
  mt.Production->ActiveProgram->NumberInList = 0;
  mt.Production->ActiveProgram->Name = "Full Program";
  InstantiateOptional(mt.Production->ActiveProgram->JobNodeId, m_pServer, n);

  InstantiateOptional(mt.Production->ActiveProgram->JobIdentifier, m_pServer, n);
  InstantiateOptional(mt.Production->ProductionPlan, m_pServer, n);
  writeEventNotifier(m_pServer, mt.Production->ProductionPlan.NodeId);

  InstantiateOptional(mt.Production->ProductionPlan->NodeVersion, m_pServer, n);
  // Hack: Remove from bindings (Will be written by BindMemberPlaceholder)
  // Can keep binding when writing is supported.
  n.Remove(mt.Production->ProductionPlan->NodeVersion.NodeId);
  auto &job = mt.Production->ProductionPlan->OrderedObjects.Add(m_pServer, n, {m_nsIndex, "MyJob 1"});
  job.Identifier = std::string("MyJob 1");
  job.RunsCompleted = 0;
  job.RunsPlanned->Value = 1;
  job.RunsPlanned->IsValid = true;

  auto t = std::time(nullptr);
  std::tm tm;
  std::memset(&tm, 0, sizeof(tm));
  UMATI_GMTIME(&t, &tm);

  std::ostringstream oss;
  oss << std::put_time(&tm, "%Y-%m-%dT%H:%M:%SZ");

  open62541Cpp::UA_String newUaStr(oss.str());
  UA_Server_writeObjectProperty_scalar(
    m_pServer,
    *mt.Production->ProductionPlan.NodeId.NodeId,
    *open62541Cpp::UA_QualifiedName(0, "NodeVersion").QualifiedName,
    newUaStr.String,
    &UA_TYPES[UA_TYPES_STRING]);

  mt.Production->ActiveProgram->JobIdentifier = job.Identifier;
  mt.Production->ActiveProgram->JobNodeId = *mt.Production->ProductionPlan->OrderedObjects.value.front().NodeId.NodeId;

  InstantiateOptional(job.PartSets, m_pServer, n);
  auto &set1 = job.PartSets->PartSet.Add(m_pServer, n, {m_nsIndex, "Set1"});
  set1.ContainsMixedParts = false;
  set1.Name = "Set1";
  set1.PartsCompletedPerRun = 3;
  // set1.PartsPerRun = 4;
  set1.PartsPlannedPerRun = 5;

  InstantiateOptional(set1.PartsPerRun, m_pServer, n);

  std::vector<machineTool::ProductionPart_t*> parts; 
  for (std::size_t i = 1; i <= 5; ++i) {
    std::stringstream ss;
    ss << "Part " << i;
    auto &part = set1.PartsPerRun->Part.Add(m_pServer, n, {m_nsIndex, ss.str()});
    parts.push_back(&part);
    part.Name = ss.str();
    part.PartQuality = static_cast<UA_PartQuality>(i % (UA_PartQuality::UA_PARTQUALITY_WILLNOTBEMEASURED + 1));
    part.ProcessIrregularity = static_cast<UA_ProcessIrregularity>(i % (UA_ProcessIrregularity::UA_PROCESSIRREGULARITY_NOTYETDETERMINED + 1));
  }

  /*
  if (m_mqttSettings.connectionIdent != nullptr) {
    UA_NodeId* publishedDataSetIdent = UA_NodeId_new();
    UA_NodeId* dataSetWriterIdent = UA_NodeId_new();
    TopicCreator tc{m_mqttSettings.prefix, m_mqttSettings.publisherId, "Production_WriterGroup", "ActiveProgram"};     
    addPublishedDataSet(m_pServer, publishedDataSetIdent, tc.publishedDataSetName, UA_FALSE);
    addDataSetField(m_pServer, publishedDataSetIdent, *mt.Production->ActiveProgram->Name.NodeId.NodeId, "Name");
    addDataSetField(m_pServer, publishedDataSetIdent, *mt.Production->ActiveProgram->NumberInList.NodeId.NodeId, "NumberInList");
    addDataSetField(m_pServer, publishedDataSetIdent, *mt.Production->ActiveProgram->JobIdentifier.NodeId.NodeId, "JobIdentifier");
    addDataSetField(m_pServer, publishedDataSetIdent, *mt.Production->ActiveProgram->JobNodeId.NodeId.NodeId, "JobNodeId");
    addDataSetField(m_pServer, publishedDataSetIdent, *mt.Production->ActiveProgram->State->CurrentState.NodeId.NodeId, "State.CurrentState");
    addDataSetWriter(m_pServer, tc.getWriterName() , tc.getMetaDataTopic(), tc.getDataTopic(), publishedDataSetIdent, &m_mqttSettings.productionWriterGroupIdent, dataSetWriterIdent);
  }

  if (m_mqttSettings.connectionIdent != nullptr) {
    UA_NodeId* publishedDataSetIdent = UA_NodeId_new();
    UA_NodeId* dataSetWriterIdent = UA_NodeId_new();
    TopicCreator tc{m_mqttSettings.prefix, m_mqttSettings.publisherId, "Production_WriterGroup", "ProductionPlan"};     
    addPublishedDataSet(m_pServer, publishedDataSetIdent, tc.publishedDataSetName);
    addDataSetField(m_pServer, publishedDataSetIdent, *mt.Production->ProductionPlan->NodeVersion.NodeId.NodeId, "NodeVersion");
    addDataSetWriter(m_pServer, tc.getWriterName() , tc.getMetaDataTopic(), tc.getDataTopic(), publishedDataSetIdent, &m_mqttSettings.productionWriterGroupIdent, dataSetWriterIdent);
  }

   if (m_mqttSettings.connectionIdent != nullptr) {
    UA_NodeId* publishedDataSetIdent = UA_NodeId_new();
    UA_NodeId* dataSetWriterIdent = UA_NodeId_new();
    TopicCreator tc{m_mqttSettings.prefix, m_mqttSettings.publisherId, "Production_WriterGroup", "ProductionPlan.MyJob1"};     
    addPublishedDataSet(m_pServer, publishedDataSetIdent, tc.publishedDataSetName);
    addDataSetField(m_pServer, publishedDataSetIdent, *job.Identifier.NodeId.NodeId, "Identifier");
    addDataSetField(m_pServer, publishedDataSetIdent, *job.NumberInList.NodeId.NodeId, "NumberInList");
    addDataSetField(m_pServer, publishedDataSetIdent, *job.RunsCompleted.NodeId.NodeId, "RunsCompleted");
    addDataSetField(m_pServer, publishedDataSetIdent, *job.RunsPlanned.NodeId.NodeId, "RunsPlanned");
    addDataSetField(m_pServer, publishedDataSetIdent, *job.State->CurrentState.NodeId.NodeId, "State.CurrentState");
    addDataSetWriter(m_pServer, tc.getWriterName() , tc.getMetaDataTopic(), tc.getDataTopic(), publishedDataSetIdent, &m_mqttSettings.productionWriterGroupIdent, dataSetWriterIdent);
  }

  if (m_mqttSettings.connectionIdent != nullptr) {
    UA_NodeId* publishedDataSetIdent = UA_NodeId_new();
    UA_NodeId* dataSetWriterIdent = UA_NodeId_new();
    TopicCreator tc{m_mqttSettings.prefix, m_mqttSettings.publisherId, "Production_WriterGroup", "ProductionPlan.MyJob1.PartSets.Set1"};     
    addPublishedDataSet(m_pServer, publishedDataSetIdent, tc.publishedDataSetName);
    addDataSetField(m_pServer, publishedDataSetIdent, *set1.ContainsMixedParts.NodeId.NodeId, "ContainsMixedParts");
    addDataSetField(m_pServer, publishedDataSetIdent, *set1.PartsCompletedPerRun.NodeId.NodeId, "PartsCompletedPerRun");
    addDataSetField(m_pServer, publishedDataSetIdent, *set1.PartsPerRun.NodeId.NodeId, "PartsPerRun");
    addDataSetWriter(m_pServer, tc.getWriterName() , tc.getMetaDataTopic(), tc.getDataTopic(), publishedDataSetIdent, &m_mqttSettings.productionWriterGroupIdent, dataSetWriterIdent);
  }

  for(machineTool::ProductionPart_t *part: parts) {
    if (m_mqttSettings.connectionIdent != nullptr) {
      UA_NodeId* publishedDataSetIdent = UA_NodeId_new();
      UA_NodeId* dataSetWriterIdent = UA_NodeId_new();
      std::string name = std::string("ProductionPlan.MyJob1.PartSets.Set1.PartsPerRun.") + part->Name.value;
      TopicCreator tc{m_mqttSettings.prefix, m_mqttSettings.publisherId, "Production_WriterGroup", name};     
      addPublishedDataSet(m_pServer, publishedDataSetIdent, tc.publishedDataSetName);
      addDataSetField(m_pServer, publishedDataSetIdent, *part->Name.NodeId.NodeId, "Name");
      addDataSetField(m_pServer, publishedDataSetIdent, *part->PartQuality.NodeId.NodeId, "PartQuality");
      addDataSetField(m_pServer, publishedDataSetIdent, *part->ProcessIrregularity.NodeId.NodeId, "ProcessIrregularity");
      addDataSetWriter(m_pServer, tc.getWriterName() , tc.getMetaDataTopic(), tc.getDataTopic(), publishedDataSetIdent, &m_mqttSettings.productionWriterGroupIdent, dataSetWriterIdent);
    }
  }
  */

  if (m_mqttSettings.connectionIdent != nullptr) {
    m_publisher.Publish(mt.Production.value, m_pServer, m_mqttSettings.connectionIdent, n, m_mqttSettings.prefix, m_mqttSettings.publisherId, "Production", 2000);
  }
}

void FullMachineTool::InstantiateIdentification() {
  InstantiatedMachineTool::InstantiateIdentification();

  InstantiateOptional(mt.Identification->YearOfConstruction, m_pServer, n);
  InstantiateOptional(mt.Identification->MonthOfConstruction, m_pServer, n);
  InstantiateOptional(mt.Identification->DeviceClass, m_pServer, n);
  InstantiateOptional(mt.Identification->ComponentName, m_pServer, n);
  InstantiateOptional(mt.Identification->SoftwareIdentification, m_pServer, n);
  InstantiateOptional(mt.Identification->ProductCode, m_pServer, n);
  InstantiateOptional(mt.Identification->SoftwareRevision, m_pServer, n);
  InstantiateOptional(mt.Identification->Location, m_pServer, n);
  InstantiateOptional(mt.Identification->Model, m_pServer, n);

  mt.Identification->YearOfConstruction = 2020;
  mt.Identification->MonthOfConstruction = 6;
  mt.Identification->DeviceClass = "Machining centre (other)";
  mt.Identification->ComponentName = {"", MachineName};
  mt.Identification->ProductCode = "2021-47110815";
  mt.Identification->SoftwareRevision = "1.00.1";
  mt.Identification->Location = "METAV 16 F02/VIRTUAL 0 0/N 48.781340 E 9.165731";
  mt.Identification->Model = {"", MachineName};

  auto &swOS = mt.Identification->SoftwareIdentification->SoftwareItem.Add(m_pServer, n, {m_nsIndex, "OS"});
  swOS.Identifier = "Alpine Container";
  swOS.SoftwareRevision = "3.15.4";  // Match with DOCKERFILE version

  if (m_mqttSettings.connectionIdent != nullptr)
    m_publisher.Publish(mt.Identification.value, m_pServer, m_mqttSettings.connectionIdent, n, m_mqttSettings.prefix, m_mqttSettings.publisherId, "Identification", 2000, UA_TRUE, UA_FALSE);
}

void FullMachineTool::InstantiateTools() {
  /*
  if (m_mqttSettings.connectionIdent != nullptr) {
    std::string topic = m_mqttSettings.prefix + "/json/data/" + m_mqttSettings.publisherId + "/Equipment_WriterGroup";  
    auto retval = addWriterGroup(m_pServer, const_cast<char*>(topic.c_str()), 2000, &m_mqttSettings.equipmentWriterGroupIdent, m_mqttSettings.connectionIdent);
    if (retval != UA_STATUSCODE_GOOD) {
      std::cout << "Error adding WriterGroup " << UA_StatusCode_name(retval) << "\n";
    }
  }
  */

  InstantiateOptional(mt.Equipment->Tools, m_pServer, n);
  InstantiateOptional(mt.Equipment->Tools->NodeVersion, m_pServer, n);
  n.Remove(mt.Equipment->Tools->NodeVersion.NodeId);
  auto &tool = mt.Equipment->Tools->Tool.Add<machineTool::Tool_t>(m_pServer, n, {m_nsIndex, "Tool1"});
  tool.ControlIdentifier1 = 2;
  tool.ControlIdentifierInterpretation = UA_ToolManagement::UA_TOOLMANAGEMENT_NUMBERBASED;
  tool.Locked->Value = false;
  tool.Locked->ReasonForLocking = UA_ToolLocked::UA_TOOLLOCKED_OTHER;
  tool.Locked->ReasonForLocking.StatusCode = UA_STATUSCODE_BADNOTHINGTODO;
  tool.Name = "Tool1";
  InstantiateOptional(tool.Name, m_pServer, n);
  InstantiateOptional(tool.ToolLife, m_pServer, n);
  auto &toolLifeRotations = tool.ToolLife->ToolLifeEntry.Add<machineTool::ToolLife_t<std::int32_t>>(m_pServer, n, {m_nsIndex, "Rotations"});
  toolLifeRotations.Indication = UA_ToolLifeIndication::UA_TOOLLIFEINDICATION_OTHER;
  toolLifeRotations.Value = 512;
  toolLifeRotations.IsCountingUp = true;
  toolLifeRotations.LimitValue = 1 << 20;
  InstantiateOptional(toolLifeRotations.LimitValue, m_pServer, n);

  auto &multiTool = mt.Equipment->Tools->Tool.Add<machineTool::MultiTool_t>(m_pServer, n, {m_nsIndex, "Multi 1"});
  multiTool.Name = {"", "Multi 1"};
  multiTool.Identifier = "Multi01-ID";
  std::vector<machineTool::Tool_t*> subtools;
  for (std::size_t i = 0; i < 3; ++i) {
    std::stringstream ss;
    ss << "SubTool " << i;
    auto &subTool = multiTool.Tool.Add(m_pServer, n, {m_nsIndex, ss.str()});
    subtools.push_back(&subTool);
    subTool.ControlIdentifier1 = i * 1021 % 881;
    subTool.ControlIdentifierInterpretation = UA_ToolManagement::UA_TOOLMANAGEMENT_NUMBERBASED;
    subTool.Locked->Value = false;
    subTool.Locked->ReasonForLocking = UA_ToolLocked::UA_TOOLLOCKED_OTHER;
    subTool.Locked->ReasonForLocking.StatusCode = UA_STATUSCODE_BADNOTHINGTODO;
    subTool.Identifier = ss.str();
    InstantiateOptional(subTool.Identifier, m_pServer, n);
    InstantiateOptional(subTool.ToolLife, m_pServer, n);
    auto &toolLifeRotations = subTool.ToolLife->ToolLifeEntry.Add<machineTool::ToolLife_t<std::int32_t>>(m_pServer, n, {m_nsIndex, "Rotations"});
    toolLifeRotations.Indication = UA_ToolLifeIndication::UA_TOOLLIFEINDICATION_OTHER;
    toolLifeRotations.Value = 512;
    toolLifeRotations.IsCountingUp = true;
    toolLifeRotations.LimitValue = 1 << 20;
    InstantiateOptional(toolLifeRotations.LimitValue, m_pServer, n);
  }

  if (m_mqttSettings.connectionIdent != nullptr)
    m_publisher.Publish(mt.Equipment.value, m_pServer, m_mqttSettings.connectionIdent, n, m_mqttSettings.prefix, m_mqttSettings.publisherId, "Equipment", 2000);

  /*
  if (m_mqttSettings.connectionIdent != nullptr) {
    UA_NodeId* publishedDataSetIdent = UA_NodeId_new();
    UA_NodeId* dataSetWriterIdent = UA_NodeId_new();
    TopicCreator tc{m_mqttSettings.prefix, m_mqttSettings.publisherId, "Equipment", "Tools"};     
    addPublishedDataSet(m_pServer, publishedDataSetIdent, tc.publishedDataSetName);
    addDataSetField(m_pServer, publishedDataSetIdent, *mt.Equipment->Tools->NodeVersion.NodeId.NodeId, "NodeVersion");
    addDataSetWriter(m_pServer, tc.getWriterName() , tc.getMetaDataTopic(), tc.getDataTopic(), publishedDataSetIdent, &m_mqttSettings.equipmentWriterGroupIdent, dataSetWriterIdent);
  }

  if (m_mqttSettings.connectionIdent != nullptr) {
    UA_NodeId* publishedDataSetIdent = UA_NodeId_new();
    UA_NodeId* dataSetWriterIdent = UA_NodeId_new();
    TopicCreator tc{m_mqttSettings.prefix, m_mqttSettings.publisherId, "Equipment", "Tools.Multi 1"};     
    addPublishedDataSet(m_pServer, publishedDataSetIdent, tc.publishedDataSetName);
    addDataSetField(m_pServer, publishedDataSetIdent, *multiTool.Identifier.NodeId.NodeId, "Identifier");
    addDataSetField(m_pServer, publishedDataSetIdent, *multiTool.Name.NodeId.NodeId, "Identifier");
    addDataSetWriter(m_pServer, tc.getWriterName() , tc.getMetaDataTopic(), tc.getDataTopic(), publishedDataSetIdent, &m_mqttSettings.equipmentWriterGroupIdent, dataSetWriterIdent);
  }

  if (m_mqttSettings.connectionIdent != nullptr) {
    UA_NodeId* publishedDataSetIdent = UA_NodeId_new();
    UA_NodeId* dataSetWriterIdent = UA_NodeId_new();
    TopicCreator tc{m_mqttSettings.prefix, m_mqttSettings.publisherId, "Equipment", std::string("Tools.Tool 1")};     
    addPublishedDataSet(m_pServer, publishedDataSetIdent, tc.publishedDataSetName);
    addDataSetField(m_pServer, publishedDataSetIdent, *tool.ControlIdentifier1.NodeId.NodeId, "ControlIdentifier1");
    addDataSetField(m_pServer, publishedDataSetIdent, *tool.ControlIdentifier2.NodeId.NodeId, "ControlIdentifier2");
    addDataSetField(m_pServer, publishedDataSetIdent, *tool.Identifier.NodeId.NodeId, "Identifier");
    addDataSetField(m_pServer, publishedDataSetIdent, *tool.Locked.NodeId.NodeId, "Locked");
    addDataSetField(m_pServer, publishedDataSetIdent, *tool.ToolLife->ToolLifeEntry.NodeId.NodeId, "ToolLife.Rotations");
    addDataSetWriter(m_pServer, tc.getWriterName() , tc.getMetaDataTopic(), tc.getDataTopic(), publishedDataSetIdent, &m_mqttSettings.equipmentWriterGroupIdent, dataSetWriterIdent);
  }

  int i = 0;
  for(auto tool: subtools) {
      if (m_mqttSettings.connectionIdent != nullptr) {
    UA_NodeId* publishedDataSetIdent = UA_NodeId_new();
    UA_NodeId* dataSetWriterIdent = UA_NodeId_new();
    TopicCreator tc{m_mqttSettings.prefix, m_mqttSettings.publisherId, "Equipment", std::string("Tools.Multi 1.") + tool->Identifier.value};     
    addPublishedDataSet(m_pServer, publishedDataSetIdent, tc.publishedDataSetName);
    addDataSetField(m_pServer, publishedDataSetIdent, *tool->ControlIdentifier1.NodeId.NodeId, "ControlIdentifier1");
    addDataSetField(m_pServer, publishedDataSetIdent, *tool->ControlIdentifier2.NodeId.NodeId, "ControlIdentifier2");
    addDataSetField(m_pServer, publishedDataSetIdent, *tool->Identifier.NodeId.NodeId, "Identifier");
    addDataSetField(m_pServer, publishedDataSetIdent, *tool->Locked.NodeId.NodeId, "Locked");
    addDataSetField(m_pServer, publishedDataSetIdent, *tool->ToolLife->ToolLifeEntry.NodeId.NodeId, "ToolLife.Rotations");
    addDataSetWriter(m_pServer, tc.getWriterName() , tc.getMetaDataTopic(), tc.getDataTopic(), publishedDataSetIdent, &m_mqttSettings.equipmentWriterGroupIdent, dataSetWriterIdent);
    i++;
  }
  }
  */
}

void FullMachineTool::InstantiateMonitoring() {
  InstantiateMonitoringMT();
  InstantiateMonitoringStacklight({
    UA_SignalColor::UA_SIGNALCOLOR_RED,
    UA_SignalColor::UA_SIGNALCOLOR_YELLOW,
    UA_SignalColor::UA_SIGNALCOLOR_GREEN,
    UA_SignalColor::UA_SIGNALCOLOR_WHITE,
  });

  InstantiateMonitoringChannel(4);

  auto &spindle1 = mt.Monitoring->MonitoredElement.Add<machineTool::SpindleMonitoring_t>(m_pServer, n, {m_nsIndex, "Spindle 1"});
  InstantiateOptional(spindle1.Override, m_pServer, n);
  InstantiateOptional(spindle1.IsUsedAsAxis, m_pServer, n);
  spindle1.Override->Value = 78;
  spindle1.Override->EURange->high = 127.99;
  spindle1.Override->EURange->low = 0.0;
  spindle1.IsRotating = true;
  spindle1.Name = "Spindle 1";
  spindle1.IsUsedAsAxis = false;

  /*
  if (m_mqttSettings.connectionIdent != nullptr) {
    UA_NodeId* publishedDataSetIdent = UA_NodeId_new();
    UA_NodeId* dataSetWriterIdent = UA_NodeId_new();
    TopicCreator tc{m_mqttSettings.prefix, m_mqttSettings.publisherId, "Monitoring_WriterGroup", spindle1.Name.value};       
    addPublishedDataSet(m_pServer, publishedDataSetIdent, tc.getPublishedDataSetName());
    addDataSetField(m_pServer, publishedDataSetIdent, *spindle1.Override.NodeId.NodeId, "Override");
    addDataSetField(m_pServer, publishedDataSetIdent, *spindle1.IsRotating.NodeId.NodeId, "IsRotating");
    addDataSetField(m_pServer, publishedDataSetIdent, *spindle1.IsUsedAsAxis.NodeId.NodeId, "IsUsedAsAxis");
    addDataSetWriter(m_pServer, tc.getWriterName() , tc.getMetaDataTopic(), tc.getDataTopic(), publishedDataSetIdent, &m_mqttSettings.monitoringWriterGroupIdent, dataSetWriterIdent);
  }
  */

  auto &edm = mt.Monitoring->MonitoredElement.Add<machineTool::EDMGeneratorMonitoring_t>(m_pServer, n, {m_nsIndex, "EDM"});
  edm.IsOn = true;
  edm.Name = "EDM";
  edm.EDMGeneratorState = UA_EDMGeneratorState::UA_EDMGENERATORSTATE_ACTIVE_HIGH_VOLTAGE;

  /*
  if (m_mqttSettings.connectionIdent != nullptr) {
    UA_NodeId* publishedDataSetIdent = UA_NodeId_new();
    UA_NodeId* dataSetWriterIdent = UA_NodeId_new();
    TopicCreator tc{m_mqttSettings.prefix, m_mqttSettings.publisherId, "Monitoring_WriterGroup", edm.Name.value};       
    addPublishedDataSet(m_pServer, publishedDataSetIdent, tc.getPublishedDataSetName(), UA_FALSE);
    addDataSetField(m_pServer, publishedDataSetIdent, *edm.Name.NodeId.NodeId, "Name");
    addDataSetField(m_pServer, publishedDataSetIdent, *edm.IsOn.NodeId.NodeId, "IsOn");
    addDataSetField(m_pServer, publishedDataSetIdent, *edm.EDMGeneratorState.NodeId.NodeId, "EDMGeneratorState");
    addDataSetWriter(m_pServer, tc.getWriterName() , tc.getMetaDataTopic(), tc.getDataTopic(), publishedDataSetIdent, &m_mqttSettings.monitoringWriterGroupIdent, dataSetWriterIdent);
  } 
  */

  auto &lsr = mt.Monitoring->MonitoredElement.Add<machineTool::LaserMonitoring_t>(m_pServer, n, {m_nsIndex, "Laser"});
  lsr.ControllerIsOn = false;
  lsr.Name = "Laser";
  lsr.LaserState = UA_LaserState::UA_LASERSTATE_READY;

  /*
  if (m_mqttSettings.connectionIdent != nullptr) {
    UA_NodeId* publishedDataSetIdent = UA_NodeId_new();
    UA_NodeId* dataSetWriterIdent = UA_NodeId_new();
    TopicCreator tc{m_mqttSettings.prefix, m_mqttSettings.publisherId, "Monitoring_WriterGroup", lsr.Name.value};       
    addPublishedDataSet(m_pServer, publishedDataSetIdent, tc.publishedDataSetName);
    addDataSetField(m_pServer, publishedDataSetIdent, *lsr.ControllerIsOn.NodeId.NodeId, "IsOn");
    addDataSetField(m_pServer, publishedDataSetIdent, *lsr.LaserState.NodeId.NodeId, "LaserState");
    addDataSetField(m_pServer, publishedDataSetIdent, *lsr.Name.NodeId.NodeId, "Name");
    addDataSetWriter(m_pServer, tc.getWriterName() ,tc.getMetaDataTopic(), tc.getDataTopic(), publishedDataSetIdent, &m_mqttSettings.monitoringWriterGroupIdent, dataSetWriterIdent);
  }
  */
  if (m_mqttSettings.connectionIdent != nullptr)
    m_publisher.Publish(mt.Monitoring.value, m_pServer, m_mqttSettings.connectionIdent, n, m_mqttSettings.prefix, m_mqttSettings.publisherId, "Monitoring", 2000);
}

void FullMachineTool::InstantiatePrognosis() {
  InstantiateOptional(mt.Notification->Prognoses, m_pServer, n);
  InstantiateOptional(mt.Notification->Prognoses->NodeVersion, m_pServer, n);
  // Hack: Remove from bindings (Will be written by BindMemberPlaceholder)
  // Can keep binding when writing is supported.
  n.Remove(mt.Notification->Prognoses->NodeVersion.NodeId);
  writeEventNotifier(m_pServer, mt.Notification->Prognoses.NodeId);
  auto &maintenancePrognosis = mt.Notification->Prognoses->Prognosis.Add<machineTool::MaintenancePrognosis_t>(m_pServer, n, {m_nsIndex, "Maintenance"});
  maintenancePrognosis.Activity = {"en", "Replace actuator."};
  auto &manualPrognosis = mt.Notification->Prognoses->Prognosis.Add<machineTool::ManualActivityPrognosis_t>(m_pServer, n, {m_nsIndex, "Manual"});
  manualPrognosis.Activity = {"en", "Open Window"};
  auto &partLoadPrognosis = mt.Notification->Prognoses->Prognosis.Add<machineTool::PartLoadPrognosis_t>(m_pServer, n, {m_nsIndex, "PartLoad"});
  partLoadPrognosis.Location = {"en", "Workspace Left"};
  partLoadPrognosis.PartName = "Circle";
  auto &partUnLoadPrognosis = mt.Notification->Prognoses->Prognosis.Add<machineTool::PartUnLoadPrognosis_t>(m_pServer, n, {m_nsIndex, "PartUnLoad"});
  partUnLoadPrognosis.Location = {"en", "Workspace Right"};
  partUnLoadPrognosis.PartName = "Smiley";
  auto &processChangeoverPrognosis =
    mt.Notification->Prognoses->Prognosis.Add<machineTool::ProcessChangeoverPrognosis_t>(m_pServer, n, {m_nsIndex, "ProcessChangeover"});
  processChangeoverPrognosis.Location = {"en", "Shaft Mid"};
  processChangeoverPrognosis.Activity = {"en", "Flip Part"};
  auto &productionJobEndPrognosis =
    mt.Notification->Prognoses->Prognosis.Add<machineTool::ProductionJobEndPrognosis_t>(m_pServer, n, {m_nsIndex, "ProductionJobEnd"});
  productionJobEndPrognosis.SourceIdentifier = "100x Smiley Job";
  auto &toolChangePrognosis = mt.Notification->Prognoses->Prognosis.Add<machineTool::ToolChangePrognosis_t>(m_pServer, n, {m_nsIndex, "ToolChange"});
  toolChangePrognosis.Location = {"en", "Magazine 1"};
  auto &toolLoadPrognosis = mt.Notification->Prognoses->Prognosis.Add<machineTool::ToolLoadPrognosis_t>(m_pServer, n, {m_nsIndex, "ToolLoad"});
  toolLoadPrognosis.Location = {"en", "Magazine 2"};
  auto &toolUnloadPrognosis = mt.Notification->Prognoses->Prognosis.Add<machineTool::ToolUnloadPrognosis_t>(m_pServer, n, {m_nsIndex, "ToolUnLoad"});
  toolUnloadPrognosis.Location = {"en", "Magazine 3"};
  auto &utilityPrognosis = mt.Notification->Prognoses->Prognosis.Add<machineTool::UtilityChangePrognosis_t>(m_pServer, n, {m_nsIndex, "UtilityChange"});
  utilityPrognosis.UtilityName = "H²";

  if (m_mqttSettings.connectionIdent != nullptr)
    m_publisher.Publish(mt.Notification.value, m_pServer, m_mqttSettings.connectionIdent, n, m_mqttSettings.prefix, m_mqttSettings.publisherId, "Notification", 2000, UA_FALSE);
}

void FullMachineTool::Simulate() {
  ++m_simStep;
  if ((m_simStep % 2) == 1) {
    SimulateStacklight();
  }

  switch (m_simStep % 10) {
    case 0: {
      JobStateMachine.SetState(0);  // Initializing
      break;
    }
    case 2: {
      JobStateMachine.SetState(1);  // Running
      break;
    }
    case 4: {
      JobStateMachine.SetState(3);  // Interrupted
      break;
    }
    case 6: {
      JobStateMachine.SetState(2);  // Ended
      break;
    }
    case 8: {
      JobStateMachine.SetState(4);  // Aborted
      break;
    }
  }

  if ((m_simStep % 10) == 1) {
    m_pAlert = std::make_shared<OpcUaCondition<machineTool::Alert_t>>(m_pServer, mt.Notification->Messages.NodeId);
    m_pAlert->Data.ErrorCode = "ERR404";
    {
      std::stringstream ss;
      ss << "Cond Message: " << m_simStep;
      m_pAlert->Data.Message = {"en", ss.str()};
    }
    m_pAlert->Data.SourceName = "SrcCond";
    m_pAlert->Data.Severity = 523;
    m_pAlert->Data.Retain = true;
    m_pAlert->Data.EnabledState->Value = {"", "Enabled"};
    m_pAlert->Data.EnabledState->Id = true;
    m_pAlert->Data.AckedState->Id = false;
    m_pAlert->Data.AckedState->Value = {"en", "Unacknowledged"};
    m_pAlert->Data.ConfirmedState->Id = false;
    m_pAlert->Data.ConfirmedState->Value = {"en", "Unconfirmed"};

    m_pAlert->Trigger();
  } else if ((m_simStep % 10) == 8 && m_pAlert) {
    m_pAlert->Data.Retain = false;
    m_pAlert->Data.EnabledState->Id = true;
    m_pAlert->Data.EnabledState->Value = {"en", "Disabled"};
    m_pAlert->Data.AckedState->Id = true;
    m_pAlert->Data.AckedState->Value = {"en", "Acknowledged"};
    m_pAlert->Data.ConfirmedState->Id = true;
    m_pAlert->Data.ConfirmedState->Value = {"en", "Confirmed"};
    m_pAlert->Trigger();
    m_pAlert = nullptr;
  }

  if ((m_simStep % 10) == 8) {
    machineTool::NotificationEvent_t notification;
    notification.Identifier = "Custom Event";
    std::stringstream ss;
    ss << "Full MT Msg " << m_simStep;
    notification.Message = {"en", ss.str()};
    notification.Severity = (m_simStep - 8) % 300;
    notification.SourceName = "FullMachineTool";
    OpcUaEvent ev(notification, m_pServer, mt.Notification->Messages.NodeId);
  }
  mt.Monitoring->MachineTool->PowerOnDuration = m_simStep / 3600;
}
