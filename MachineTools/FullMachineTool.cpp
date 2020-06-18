#include "FullMachineTool.hpp"
#include <open62541/types_generated.h>
#include "../BindRefl.hpp"
#include "../OpcUaCondition.hpp"
#include "../TypeDefinition/MachineTool/Alert.hpp"

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
  auto st = UA_Server_writeEventNotifier(m_pServer, *mt.Production->ProductionPlan.NodeId.NodeId, UA_EVENTNOTIFIERTYPE_SUBSCRIBETOEVENTS);
  if (st != UA_STATUSCODE_GOOD)
  {
    std::cout << "WriteEventNotifier failed: " << UA_StatusCode_name(st) << std::endl;
  }
  InstantiateOptional(mt.Production->ProductionPlan->NodeVersion, m_pServer, n);
  // Hack: Remove from bindings (Will be written by BindMemberPlaceholder)
  // Can keep binding when writing is supported.
  n.Remove(mt.Production->ProductionPlan->NodeVersion.NodeId);
  auto &job = mt.Production->ProductionPlan->OrderedObjects.Add(m_pServer, n, {m_nsIndex, "MyJob 1"});
  job.Identifier = std::string("ID 1");
  job.RunsCompleted = 0;
  job.RunsPlanned = 1;

  InstantiateOptional(mt.Notification->Prognoses, m_pServer, n);
  InstantiateOptional(mt.Notification->Prognoses->NodeVersion, m_pServer, n);
  // Hack: Remove from bindings (Will be written by BindMemberPlaceholder)
  // Can keep binding when writing is supported.
  n.Remove(mt.Notification->Prognoses->NodeVersion.NodeId);
  auto &maintenancePrognosis = mt.Notification->Prognoses->Prognosis.Add<machineTool::MaintenancePrognosis_t>(m_pServer, n, {m_nsIndex, "Maintenance"});
  maintenancePrognosis.Activity = open62541Cpp::LocalizedText_t{"", "Replace actuator."};
  auto &utilityPrognosis = mt.Notification->Prognoses->Prognosis.Add<machineTool::UtilityChangePrognosis_t>(m_pServer, n, {m_nsIndex, "Utility"});
  utilityPrognosis.UtilityName = open62541Cpp::LocalizedText_t{"", "H²"};

  mt.Identification->Manufacturer = open62541Cpp::LocalizedText_t{.locale = "c++", .text = "ISW Christian von Arnim"};
  mt.Identification->YearOfConstruction = 2020;
  mt.Identification->Location = std::string("AMB 0 0/N 48.781340 E 9.165731");
}

void FullMachineTool::Simulate()
{
  ++m_simStep;

  switch (m_simStep % 10)
  {
  case 0:
  {
    mt.Production->ActiveProgram->State->CurrentState->Value = open62541Cpp::LocalizedText_t{"", "Initializing"};
    mt.Production->ActiveProgram->State->CurrentState->Id = UA_NODEID_NUMERIC(nsFromUri(m_pServer, constants::NsMachineToolUri), UA_MACHINETOOLID_PRODUCTIONSTATEMACHINETYPE_INITIALIZING);
    mt.Production->ActiveProgram->State->CurrentState->Number = 0;
    break;
  }
  case 2:
  {
    mt.Production->ActiveProgram->State->CurrentState->Value = open62541Cpp::LocalizedText_t{"", "Running"};
    mt.Production->ActiveProgram->State->CurrentState->Id = UA_NODEID_NUMERIC(nsFromUri(m_pServer, constants::NsMachineToolUri), UA_MACHINETOOLID_PRODUCTIONSTATEMACHINETYPE_RUNNING);
    mt.Production->ActiveProgram->State->CurrentState->Number = 1;
    break;
  }
  case 4:
  {
    mt.Production->ActiveProgram->State->CurrentState->Value = open62541Cpp::LocalizedText_t{"", "Interrupted"};
    mt.Production->ActiveProgram->State->CurrentState->Id = UA_NODEID_NUMERIC(nsFromUri(m_pServer, constants::NsMachineToolUri), UA_MACHINETOOLID_PRODUCTIONSTATEMACHINETYPE_INTERRUPTED);
    mt.Production->ActiveProgram->State->CurrentState->Number = 3;
    break;
  }
  case 6:
  {
    mt.Production->ActiveProgram->State->CurrentState->Value = open62541Cpp::LocalizedText_t{"", "Ended"};
    mt.Production->ActiveProgram->State->CurrentState->Id = UA_NODEID_NUMERIC(nsFromUri(m_pServer, constants::NsMachineToolUri), UA_MACHINETOOLID_PRODUCTIONSTATEMACHINETYPE_ENDED);
    mt.Production->ActiveProgram->State->CurrentState->Number = 2;
    break;
  }
  case 8:
  {
    mt.Production->ActiveProgram->State->CurrentState->Value = open62541Cpp::LocalizedText_t{"", "Aborted"};
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
      m_pAlert->Data.Message = open62541Cpp::LocalizedText_t{"", ss.str()};
    }
    m_pAlert->Data.SourceName = "SrcCond";
    m_pAlert->Data.Severity = 123;
    m_pAlert->Data.Retain = true;
    m_pAlert->Data.EnabledState->Value = open62541Cpp::LocalizedText_t{"", "Enabled"};
    m_pAlert->Data.EnabledState->Id = true;
    m_pAlert->Data.AckedState->Id = false;
    m_pAlert->Data.AckedState->Value = open62541Cpp::LocalizedText_t{"", "Unacknowledged"};
    m_pAlert->Data.ConfirmedState->Id = false;
    m_pAlert->Data.ConfirmedState->Value = open62541Cpp::LocalizedText_t{"", "Unconfirmed"};

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
    m_pAlert->Data.EnabledState->Value = open62541Cpp::LocalizedText_t{"", "Disabled"};
    m_pAlert->Data.AckedState->Id = true;
    m_pAlert->Data.AckedState->Value = open62541Cpp::LocalizedText_t{"", "Acknowledged"};
    m_pAlert->Data.ConfirmedState->Id = true;
    m_pAlert->Data.ConfirmedState->Value = open62541Cpp::LocalizedText_t{"", "Confirmed"};
    m_pAlert->Trigger();
    m_pAlert = nullptr;

    if (!mt.Monitoring->Channels->empty())
    {
      auto lastIt = --mt.Production->ProductionPlan->OrderedObjects->end();
      mt.Production->ProductionPlan->OrderedObjects.Delete(lastIt, m_pServer, n);
    }
  }
}
