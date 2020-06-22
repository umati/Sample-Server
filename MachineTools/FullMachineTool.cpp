#include "FullMachineTool.hpp"
#include <open62541/types_generated.h>
#include "../BindRefl.hpp"
#include "../OpcUaCondition.hpp"
#include "../TypeDefinition/MachineTool/Alert.hpp"
#include "../TypeDefinition/MachineTool/NotificationEvent.hpp"
#include "../ServerHelper.hpp"

FullMachineTool::FullMachineTool(UA_Server *pServer)
    : m_pServer(pServer), NsIndex(m_nsIndex), n(pServer)
{
  CreateObject();
}

void FullMachineTool::CreateObject()
{
  m_nsIndex = UA_Server_addNamespace(m_pServer, "http://example.com/FullMachineTool/");
  UA_ObjectAttributes objAttr = UA_ObjectAttributes_default;

  objAttr.displayName = UA_LOCALIZEDTEXT_ALLOC("", "FullMachineTool");
  objAttr.eventNotifier = UA_EVENTNOTIFIERTYPE_SUBSCRIBETOEVENTS;
  open62541Cpp::UA_NodeId inst2((UA_UInt16)0, 0);

  auto status = UA_Server_addObjectNode(
      m_pServer,
      UA_NODEID_NUMERIC(m_nsIndex, 0),
      UA_NODEID_NUMERIC(nsFromUri(m_pServer, constants::NsMachineryUri), UA_MACHINERY_ID_MACHINES),
      UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
      *open62541Cpp::UA_QualifiedName(m_nsIndex, "FullMachineTool").QualifiedName,
      UA_NODEID_NUMERIC(nsFromUri(m_pServer, constants::NsMachineToolUri), UA_MACHINETOOLID_MACHINETOOLTYPE),
      objAttr,
      nullptr,
      m_nodeId.NodeId);

  UA_ObjectAttributes_clear(&objAttr);
  UmatiServerLib::Bind::MembersRefl(mt, m_pServer, m_nodeId, n);

  InstantiateIdentification();
  InstantiateMonitoring();
  InstantiateTools();
  InstantiatePrognosis();

  InstantiateOptional(mt.Production->ActiveProgram->State, m_pServer, n);
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
  InstantiateOptional(mt.Identification->Location, m_pServer, n);
  InstantiateOptional(mt.Identification->YearOfConstruction, m_pServer, n);
  InstantiateOptional(mt.Identification->MonthOfConstruction, m_pServer, n);
  InstantiateOptional(mt.Identification->DeviceClass, m_pServer, n);
  InstantiateOptional(mt.Identification->ComponentName, m_pServer, n);
  InstantiateOptional(mt.Identification->Model, m_pServer, n);

  mt.Identification->Manufacturer = {"c++", "ISW Christian von Arnim"};
  mt.Identification->YearOfConstruction = 2020;
  mt.Identification->Location = "AMB 0 0/N 48.781340 E 9.165731";
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
  for(std::size_t i = 0; i < 3; ++i)
  {
    std::stringstream ss;
    ss << "SubTool " << i;
    auto &subTool = multiTool.Tool.Add(m_pServer, n, {m_nsIndex, ss.str()});
    subTool.ControlIdentifier1 = i*1021 % 881;
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
  InstantiateOptional(mt.Monitoring->Stacklight, m_pServer, n);
  InstantiateOptional(mt.Monitoring->Stacklight->NodeVersion, m_pServer, n);
  auto &light1 = mt.Monitoring->Stacklight->OrderedObjects.Add(m_pServer, n, {m_nsIndex, "Light 1"});
  light1.IsPartOfBase = false;
  light1.NumberInList = 1;
  light1.SignalOn = true;

  auto &channel = mt.Monitoring->Channels.Add(m_pServer, n, {m_nsIndex, "InstChannel1"});
  channel.ChannelState = UA_CHANNELSTATE_INTERRUPTED;
  channel.FeedOverride->Value = 89.0;

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
    m_pAlert = std::make_shared<OpcUaCondition<machineTool::Alert_t>>(m_pServer, open62541Cpp::UA_NodeId(UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER)));
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

    {
      std::stringstream ss;
      ss << "Job " << m_simStep;
      auto &job = mt.Production->ProductionPlan->OrderedObjects.Add(m_pServer, n, {m_nsIndex, ss.str()});
      job.Identifier = ss.str();
      job.RunsCompleted = 0;
      job.RunsPlanned = 2;
    }
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

    if (!mt.Monitoring->Channels->empty())
    {
      auto lastIt = --mt.Production->ProductionPlan->OrderedObjects->end();
      mt.Production->ProductionPlan->OrderedObjects.Delete(lastIt, m_pServer, n);
    }
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
