#include "FullMachineTool.hpp"
#include "../UmatiServerLib/BindRefl.hpp"
#include "../UmatiServerLib/OpcUaCondition.hpp"
#include "../UmatiServerLib/ServerHelper.hpp"
#include "../TypeDefinition/MachineTool/Alert.hpp"
#include "../TypeDefinition/MachineTool/NotificationEvent.hpp"
#include <open62541/types_generated.h>

FullMachineTool::FullMachineTool(UA_Server *pServer)
:FullMachineTool(pServer, true)
{
}

FullMachineTool::FullMachineTool(UA_Server *pServer, bool initialize)
    : InstantiatedMachineTool(pServer)
{
  if(initialize)
  {
    MachineName = "FullMachineTool";
    CreateObject();
  }
}

void FullMachineTool::CreateObject()
{
  InstantiatedMachineTool::CreateObject();

  InstantiateIdentification();
  InstantiateMonitoring();
  InstantiateTools();
  InstantiatePrognosis();

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
  job.Identifier = std::string("ID 1");
  job.RunsCompleted = 0;
  job.RunsPlanned = 1;

  InstantiateOptional(mt.Notification->Messages, m_pServer, n);
  writeEventNotifier(m_pServer, mt.Notification->Messages.NodeId);
}

void FullMachineTool::InstantiateIdentification()
{
  InstantiatedMachineTool::InstantiateIdentification();

  InstantiateOptional(mt.Identification->YearOfConstruction, m_pServer, n);
  InstantiateOptional(mt.Identification->MonthOfConstruction, m_pServer, n);
  InstantiateOptional(mt.Identification->DeviceClass, m_pServer, n);
  InstantiateOptional(mt.Identification->ComponentName, m_pServer, n);
  InstantiateOptional(mt.Identification->SoftwareIdentification, m_pServer, n);


  mt.Identification->YearOfConstruction = 2020;
  mt.Identification->MonthOfConstruction = 6;
  mt.Identification->Location = "AMB 0 1/N 48.781340 E 9.165731";
  mt.Identification->SerialNumber = "3-1415926535-8979323846";
  mt.Identification->DeviceClass = "MachineTool";
  mt.Identification->ComponentName = {"", MachineName};

  auto &swOS = mt.Identification->SoftwareIdentification->SoftwareItem.Add(m_pServer, n, {m_nsIndex, "OS"});
  swOS.Identifier = "Alpine Container";
  swOS.SoftwareRevision = "latest";
}
void FullMachineTool::InstantiateTools()
{
  InstantiateOptional(mt.Equipment->Tools, m_pServer, n);
  InstantiateOptional(mt.Equipment->Tools->NodeVersion, m_pServer, n);
  n.Remove(mt.Equipment->Tools->NodeVersion.NodeId);
  auto &tool = mt.Equipment->Tools->Tool.Add<machineTool::Tool_t>(m_pServer, n, {m_nsIndex, "Tool1"});
  tool.ControlIdentifier1 = 2;
  tool.ControlIdentifierInterpretation = UA_ToolManagement::UA_TOOLMANAGEMENT_NUMBERBASED;
  tool.Locked->Value = false;
  tool.Locked->ReasonForLocking = UA_ToolLocked::UA_TOOLLOCKED_OTHER;
  tool.Locked->ReasonForLocking.StatusCode = UA_STATUSCODE_BADNOTHINGTODO;
  tool.Name = {"", "Tool1"};
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
  for (std::size_t i = 0; i < 3; ++i)
  {
    std::stringstream ss;
    ss << "SubTool " << i;
    auto &subTool = multiTool.Tool.Add(m_pServer, n, {m_nsIndex, ss.str()});
    subTool.ControlIdentifier1 = i * 1021 % 881;
    subTool.ControlIdentifierInterpretation = UA_ToolManagement::UA_TOOLMANAGEMENT_NUMBERBASED;
    subTool.Locked->Value = false;
    subTool.Locked->ReasonForLocking = UA_ToolLocked::UA_TOOLLOCKED_OTHER;
    subTool.Locked->ReasonForLocking.StatusCode = UA_STATUSCODE_BADNOTHINGTODO;
    subTool.Identifier = ss.str();
    InstantiateOptional(subTool.Identifier, m_pServer, n);
  }
}

void FullMachineTool::InstantiateMonitoring()
{
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

  auto &edm = mt.Monitoring->MonitoredElement.Add<machineTool::EDMGeneratorMonitoring_t>(m_pServer, n, {m_nsIndex, "EDM"});
  edm.IsOn = true;
  edm.Name = "EDM";
  edm.EDMGeneratorState = UA_EDMGeneratorState::UA_EDMGENERATORSTATE_ACTIVE_HIGH_VOLTAGE;

  auto &lsr = mt.Monitoring->MonitoredElement.Add<machineTool::LaserMonitoring_t>(m_pServer, n, {m_nsIndex, "Laser"});
  lsr.IsOn = false;
  lsr.Name = "Laser";
  lsr.LaserState = UA_LaserState::UA_LASERSTATE_READY;
}

void FullMachineTool::InstantiatePrognosis()
{
  InstantiateOptional(mt.Notification->Prognoses, m_pServer, n);
  InstantiateOptional(mt.Notification->Prognoses->NodeVersion, m_pServer, n);
  // Hack: Remove from bindings (Will be written by BindMemberPlaceholder)
  // Can keep binding when writing is supported.
  n.Remove(mt.Notification->Prognoses->NodeVersion.NodeId);
  writeEventNotifier(m_pServer, mt.Notification->Prognoses.NodeId);
  auto &maintenancePrognosis = mt.Notification->Prognoses->Prognosis.Add<machineTool::MaintenancePrognosis_t>(m_pServer, n, {m_nsIndex, "Maintenance"});
  maintenancePrognosis.Activity = {"", "Replace actuator."};
  auto &manualPrognosis = mt.Notification->Prognoses->Prognosis.Add<machineTool::ManualActivityPrognosis_t>(m_pServer, n, {m_nsIndex, "Manual"});
  manualPrognosis.Activity = {"", "Open Window"};
  auto &partLoadPrognosis = mt.Notification->Prognoses->Prognosis.Add<machineTool::PartLoadPrognosis_t>(m_pServer, n, {m_nsIndex, "PartLoad"});
  partLoadPrognosis.Location = {"", "Left"};
  partLoadPrognosis.PartName = "Circle";
  auto &partUnLoadPrognosis = mt.Notification->Prognoses->Prognosis.Add<machineTool::PartUnLoadPrognosis_t>(m_pServer, n, {m_nsIndex, "PartUnLoad"});
  partUnLoadPrognosis.Location = {"", "Right"};
  partUnLoadPrognosis.PartName = "Smiley";
  auto &processChangeoverPrognosis = mt.Notification->Prognoses->Prognosis.Add<machineTool::ProcessChangeoverPrognosis_t>(m_pServer, n, {m_nsIndex, "ProcessChangeover"});
  processChangeoverPrognosis.Location = {"", "Mid"};
  processChangeoverPrognosis.Activity = {"", "Flip"};
  auto &productionJobEndPrognosis = mt.Notification->Prognoses->Prognosis.Add<machineTool::ProductionJobEndPrognosis_t>(m_pServer, n, {m_nsIndex, "ProductionJobEnd"});
  productionJobEndPrognosis.SourceIdentifier = "100x Smiley Job";
  auto &toolChangePrognosis = mt.Notification->Prognoses->Prognosis.Add<machineTool::ToolChangePrognosis_t>(m_pServer, n, {m_nsIndex, "ToolChange"});
  toolChangePrognosis.Location = {"", "Magazine 1"};
  auto &toolLoadPrognosis = mt.Notification->Prognoses->Prognosis.Add<machineTool::ToolLoadPrognosis_t>(m_pServer, n, {m_nsIndex, "ToolLoad"});
  toolLoadPrognosis.Location = {"", "Magazine 2"};
  auto &toolUnloadPrognosis = mt.Notification->Prognoses->Prognosis.Add<machineTool::ToolUnloadPrognosis_t>(m_pServer, n, {m_nsIndex, "ToolUnLoad"});
  toolUnloadPrognosis.Location = {"", "Magazine 3"};
  auto &utilityPrognosis = mt.Notification->Prognoses->Prognosis.Add<machineTool::UtilityChangePrognosis_t>(m_pServer, n, {m_nsIndex, "UtilityChange"});
  utilityPrognosis.UtilityName = {"", "H²"};
}

void FullMachineTool::Simulate()
{
  ++m_simStep;
  if ((m_simStep % 2) == 1)
  {
    SimulateStacklight();
  }

  switch (m_simStep % 10)
  {
  case 0:
  {
    mt.Production->ActiveProgram->State->CurrentState->Value = {"", "Initializing"};
    mt.Production->ActiveProgram->State->CurrentState->Id = UA_NODEID_NUMERIC(nsFromUri(m_pServer, constants::NsMachineToolUri), UA_MACHINETOOLID_PRODUCTIONSTATEMACHINETYPE_INITIALIZING);
    mt.Production->ActiveProgram->State->CurrentState->Number = 0;
    break;
  }
  case 2:
  {
    mt.Production->ActiveProgram->State->CurrentState->Value = {"", "Running"};
    mt.Production->ActiveProgram->State->CurrentState->Id = UA_NODEID_NUMERIC(nsFromUri(m_pServer, constants::NsMachineToolUri), UA_MACHINETOOLID_PRODUCTIONSTATEMACHINETYPE_RUNNING);
    mt.Production->ActiveProgram->State->CurrentState->Number = 1;
    break;
  }
  case 4:
  {
    mt.Production->ActiveProgram->State->CurrentState->Value = {"", "Interrupted"};
    mt.Production->ActiveProgram->State->CurrentState->Id = UA_NODEID_NUMERIC(nsFromUri(m_pServer, constants::NsMachineToolUri), UA_MACHINETOOLID_PRODUCTIONSTATEMACHINETYPE_INTERRUPTED);
    mt.Production->ActiveProgram->State->CurrentState->Number = 3;
    break;
  }
  case 6:
  {
    mt.Production->ActiveProgram->State->CurrentState->Value = {"", "Ended"};
    mt.Production->ActiveProgram->State->CurrentState->Id = UA_NODEID_NUMERIC(nsFromUri(m_pServer, constants::NsMachineToolUri), UA_MACHINETOOLID_PRODUCTIONSTATEMACHINETYPE_ENDED);
    mt.Production->ActiveProgram->State->CurrentState->Number = 2;
    break;
  }
  case 8:
  {
    mt.Production->ActiveProgram->State->CurrentState->Value = {"", "Aborted"};
    mt.Production->ActiveProgram->State->CurrentState->Id = UA_NODEID_NUMERIC(nsFromUri(m_pServer, constants::NsMachineToolUri), UA_MACHINETOOLID_PRODUCTIONSTATEMACHINETYPE_ABORTED);
    mt.Production->ActiveProgram->State->CurrentState->Number = 4;
    break;
  }
  }

  if ((m_simStep % 10) == 1)
  {
    m_pAlert = std::make_shared<OpcUaCondition<machineTool::Alert_t>>(m_pServer, mt.Notification->Messages.NodeId);
    m_pAlert->Data.ErrorCode = "ERR404";
    {
      std::stringstream ss;
      ss << "Cond Message: " << m_simStep;
      m_pAlert->Data.Message = {"", ss.str()};
    }
    m_pAlert->Data.SourceName = "SrcCond";
    m_pAlert->Data.Severity = 123;
    m_pAlert->Data.Retain = true;
    m_pAlert->Data.EnabledState->Value = {"", "Enabled"};
    m_pAlert->Data.EnabledState->Id = true;
    m_pAlert->Data.AckedState->Id = false;
    m_pAlert->Data.AckedState->Value = {"", "Unacknowledged"};
    m_pAlert->Data.ConfirmedState->Id = false;
    m_pAlert->Data.ConfirmedState->Value = {"", "Unconfirmed"};

    m_pAlert->Trigger();
  }
  else if ((m_simStep % 10) == 8 && m_pAlert)
  {
    m_pAlert->Data.Retain = false;
    m_pAlert->Data.EnabledState->Id = true;
    m_pAlert->Data.EnabledState->Value = {"", "Disabled"};
    m_pAlert->Data.AckedState->Id = true;
    m_pAlert->Data.AckedState->Value = {"", "Acknowledged"};
    m_pAlert->Data.ConfirmedState->Id = true;
    m_pAlert->Data.ConfirmedState->Value = {"", "Confirmed"};
    m_pAlert->Trigger();
    m_pAlert = nullptr;
  }

  if ((m_simStep % 10) == 8)
  {
    machineTool::NotificationEvent_t notification;
    notification.Identifier = "Custom Event";
    std::stringstream ss;
    ss << "Full MT Msg " << m_simStep;
    notification.Message = {"", ss.str()};
    notification.Severity = (m_simStep - 8) % 300;
    notification.SourceName = "FullMachineTool";
    OpcUaEvent ev(notification, m_pServer, mt.Notification->Messages.NodeId);
  }
}
