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

  auto &channel = mt.Monitoring->Channels.Add(m_pServer, n, {m_nsIndex, "InstChannel1"});
  channel.ChannelState = UA_CHANNELSTATE_INTERRUPTED;
  channel.FeedOverride->Value = 89.0;

  //InstantiateOptional(mt.Identification->Location, m_pServer, n);
  InstantiateOptional(mt.Production->ActiveProgram->State, m_pServer, n);
  InstantiateOptional(mt.Identification->YearOfConstruction, m_pServer, n);
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
  InstantiateOptional(mt.Notification->Prognoses, m_pServer, n);
  InstantiateOptional(mt.Notification->Prognoses->NodeVersion, m_pServer, n);
  // Hack: Remove from bindings (Will be written by BindMemberPlaceholder)
  // Can keep binding when writing is supported.
  n.Remove(mt.Notification->Prognoses->NodeVersion.NodeId);
  writeEventNotifier(m_pServer, mt.Notification->Prognoses.NodeId);
  auto &maintenancePrognosis = mt.Notification->Prognoses->Prognosis.Add<machineTool::MaintenancePrognosis_t>(m_pServer, n, {m_nsIndex, "Maintenance"});
  maintenancePrognosis.Activity = {"", "Replace actuator."};
  auto &utilityPrognosis = mt.Notification->Prognoses->Prognosis.Add<machineTool::UtilityChangePrognosis_t>(m_pServer, n, {m_nsIndex, "Utility"});
  utilityPrognosis.UtilityName = {"", "H²"};

  mt.Identification->Manufacturer = {"c++", "ISW Christian von Arnim"};
  mt.Identification->YearOfConstruction = 2020;
  mt.Identification->Location = "AMB 0 0/N 48.781340 E 9.165731";

  InstantiateOptional(mt.Monitoring->Stacklight, m_pServer, n);
  InstantiateOptional(mt.Monitoring->Stacklight->NodeVersion, m_pServer, n);
  auto &light1 = mt.Monitoring->Stacklight->OrderedObjects.Add(m_pServer, n, {m_nsIndex, "Light 1"});
  light1.IsPartOfBase = false;
  light1.NumberInList = 1;
  light1.SignalOn = true;

  InstantiateOptional(mt.Equipment->Tools, m_pServer, n);
  InstantiateOptional(mt.Equipment->Tools->NodeVersion, m_pServer, n);
  n.Remove(mt.Equipment->Tools->NodeVersion.NodeId);
  auto &tool = mt.Equipment->Tools->Tool.Add(m_pServer, n, {m_nsIndex, "Tool1"});
  tool.ControlIdentifier1 = 2;
  tool.ControlIdentifierInterpretation = UA_ToolManagement::UA_TOOLMANAGEMENT_NUMBERBASED;
  tool.Locked->Value = false;
  tool.Locked->ReasonForLocking = UA_ToolLocked::UA_TOOLLOCKED_OTHER;
  tool.Locked->ReasonForLocking.StatusCode = UA_STATUSCODE_BADNOTHINGTODO;
  tool.Name = {"", "Tool1"};
  InstantiateOptional(tool.Name, m_pServer, n);
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
    notification.Severity = (m_simStep-8) % 300;
    notification.SourceName = "FullMachineTool";
    OpcUaEvent ev(notification, m_pServer, mt.Notification->Messages.NodeId);
  }
}
