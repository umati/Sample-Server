#include "BindRefl.hpp"
#include <iostream>
#include <open62541/server_config_default.h>
#include "src_generated/namespace_iswexample_generated.h"
#include "src_generated/namespace_machinetool_generated.h"
#include "src_generated/namespace_machinery_generated.h"
#include "src_generated/namespace_di_generated.h"
#include "src_generated/namespace_industrial_automation_generated.h"
#include "src_generated/iswexample.h"
#include <cstdint>
#include <functional>
#include "NodeValue.hpp"
#include "NodesMaster.hpp"
#include <thread>
#include <chrono>
#include <mutex>
#include <atomic>
#include "OpcUaEvent.hpp"
#include "Instantiation.hpp"
#include "OpcUaCondition.hpp"
#include "OpcUaKeys.hpp"

#include "TypeDefinition/MachineTool/MachineTool.hpp"
#include "TypeDefinition/MachineTool/Alert.hpp"
#include "TypeDefinition/MachineTool/NotificationEvent.hpp"

std::shared_ptr<OpcUaCondition<machineTool::Alert_t>> pCondition;

namespace constants
{
constexpr const char *NsInstanceUri = "http://isw.example.com";
} // namespace constants

void simulate(machineTool::MachineTool_t *pMachineTool,
              std::atomic_bool &running,
              std::mutex &accessDataMutex,
              UA_Server *pServer,
              NodesMaster &n,
              machineTool::MachineTool_t &machineTool2)
{
  std::unique_lock<std::remove_reference<decltype(accessDataMutex)>::type> ul(accessDataMutex);
  ul.unlock();
  int i = 0;
  while (running)
  {
    ul.lock();
    ++(pMachineTool->Identification->YearOfConstruction.value);
    //pChannel1->ChannelState = static_cast<UA_ChannelState>((((int)pChannel1->ChannelState) + 1) % (UA_ChannelState::UA_CHANNELSTATE_RESET + 1));

    {
      machineTool::NotificationEvent_t notifEvent;

      notifEvent.Time = std::chrono::system_clock::now();
      notifEvent.SourceName = "Reflection Event";
      notifEvent.Severity = 258;
      std::stringstream ss;
      ss << "A Message " << i;
      notifEvent.Message = {
          .locale = "",
          .text = ss.str()};

      notifEvent.Identifier = "A";

      OpcUaEvent ev(notifEvent, pServer, open62541Cpp::UA_NodeId(UA_NODEID_NUMERIC(6, UA_ISWEXAMPLE_ID_MACHINES_ISWEXAMPLEMACHINE_NOTIFICATION_MESSAGES)));
    }

    switch (i % 10)
    {
    case 0:
    {
      machineTool2.Production->ActiveProgram->State->CurrentState->Value = open62541Cpp::LocalizedText_t{"", "Initializing"};
      machineTool2.Production->ActiveProgram->State->CurrentState->Id = UA_NODEID_NUMERIC(nsFromUri(pServer, constants::NsMachineToolUri), UA_MACHINETOOLID_PRODUCTIONSTATEMACHINETYPE_INITIALIZING);
      machineTool2.Production->ActiveProgram->State->CurrentState->Number = 0;
      break;
    }
    case 2:
    {
      machineTool2.Production->ActiveProgram->State->CurrentState->Value = open62541Cpp::LocalizedText_t{"", "Running"};
      machineTool2.Production->ActiveProgram->State->CurrentState->Id = UA_NODEID_NUMERIC(nsFromUri(pServer, constants::NsMachineToolUri), UA_MACHINETOOLID_PRODUCTIONSTATEMACHINETYPE_RUNNING);
      machineTool2.Production->ActiveProgram->State->CurrentState->Number = 1;
      break;
    }
    case 4:
    {
      machineTool2.Production->ActiveProgram->State->CurrentState->Value = open62541Cpp::LocalizedText_t{"", "Interrupted"};
      machineTool2.Production->ActiveProgram->State->CurrentState->Id = UA_NODEID_NUMERIC(nsFromUri(pServer, constants::NsMachineToolUri), UA_MACHINETOOLID_PRODUCTIONSTATEMACHINETYPE_INTERRUPTED);
      machineTool2.Production->ActiveProgram->State->CurrentState->Number = 3;
      break;
    }
    case 6:
    {
      machineTool2.Production->ActiveProgram->State->CurrentState->Value = open62541Cpp::LocalizedText_t{"", "Ended"};
      machineTool2.Production->ActiveProgram->State->CurrentState->Id = UA_NODEID_NUMERIC(nsFromUri(pServer, constants::NsMachineToolUri), UA_MACHINETOOLID_PRODUCTIONSTATEMACHINETYPE_ENDED);
      machineTool2.Production->ActiveProgram->State->CurrentState->Number = 2;
      break;
    }
    case 8:
    {
      machineTool2.Production->ActiveProgram->State->CurrentState->Value = open62541Cpp::LocalizedText_t{"", "Aborted"};
      machineTool2.Production->ActiveProgram->State->CurrentState->Id = UA_NODEID_NUMERIC(nsFromUri(pServer, constants::NsMachineToolUri), UA_MACHINETOOLID_PRODUCTIONSTATEMACHINETYPE_ABORTED);
      machineTool2.Production->ActiveProgram->State->CurrentState->Number = 4;
      break;
    }
    }

    if ((i % 10) == 1)
    {
      pCondition = std::make_shared<OpcUaCondition<machineTool::Alert_t>>(pServer, open62541Cpp::UA_NodeId(UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER)));
      pCondition->Data.ErrorCode = "ERR404";
      {
        std::stringstream ss;
        ss << "Cond Message: " << i;
        pCondition->Data.Message = open62541Cpp::LocalizedText_t{"", ss.str()};
      }
      pCondition->Data.SourceName = "SrcCond";
      pCondition->Data.Severity = 123;
      pCondition->Data.Retain = true;
      pCondition->Data.EnabledState->Value = open62541Cpp::LocalizedText_t{"", "Enabled"};
      pCondition->Data.EnabledState->Id = true;
      pCondition->Data.AckedState->Id = false;
      pCondition->Data.AckedState->Value = open62541Cpp::LocalizedText_t{"", "Unacknowledged"};
      pCondition->Data.ConfirmedState->Id = false;
      pCondition->Data.ConfirmedState->Value = open62541Cpp::LocalizedText_t{"", "Unconfirmed"};

      pCondition->Trigger();

      {
        std::stringstream ss;
        ss << "Job " << i;
        auto &job = machineTool2.Production->ProductionPlan->OrderedObjects.Add(pServer, n, {6, ss.str()});
        job.Identifier = ss.str();
        job.RunsCompleted = 0;
        job.RunsPlanned = 2;
      }
    }
    else if ((i % 10) == 5 && pCondition)
    {
      pCondition->Data.Retain = false;
      pCondition->Data.EnabledState->Id = true;
      pCondition->Data.EnabledState->Value = open62541Cpp::LocalizedText_t{"", "Disabled"};
      pCondition->Data.AckedState->Id = true;
      pCondition->Data.AckedState->Value = open62541Cpp::LocalizedText_t{"", "Acknowledged"};
      pCondition->Data.ConfirmedState->Id = true;
      pCondition->Data.ConfirmedState->Value = open62541Cpp::LocalizedText_t{"", "Confirmed"};
      pCondition->Trigger();
      pCondition = nullptr;

      if (!machineTool2.Monitoring->Channels->empty())
      {
        auto lastIt = --machineTool2.Production->ProductionPlan->OrderedObjects->end();
        machineTool2.Production->ProductionPlan->OrderedObjects.Delete(lastIt, pServer, n);
      }
    }

    //std::cout << i << std::endl;
    //running = i < 10;
    ++i;
    ul.unlock();
    std::this_thread::yield();
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
}

UA_StatusCode generateChildNodeIdInParentNs(
    UA_Server *server,
    const UA_NodeId *sessionId, void *sessionContext,
    const UA_NodeId *sourceNodeId,
    const UA_NodeId *targetParentNodeId,
    const UA_NodeId *referenceTypeId,
    UA_NodeId *targetNodeId)
{
  if (UA_NodeId_equal(targetNodeId, &UA_NODEID_NULL) && !UA_NodeId_equal(targetParentNodeId, &UA_NODEID_NULL))
  {
    targetNodeId->namespaceIndex = targetParentNodeId->namespaceIndex;
  }
  return UA_STATUSCODE_GOOD;
}

int main(int argc, char *argv[])
{
  std::cout << "ExampleMTServer" << std::endl;
  OpcUaKeys keys("server_key.der", "server_cert.der");
  UA_Server *pServer = UA_Server_new();
  auto pConfig = UA_Server_getConfig(pServer);
  try
  {
    keys.Load();
    // Skip all certificate checks
    size_t issuerListSize = 0;
    UA_ByteString *issuerList = NULL;
    size_t trustListSize = 0;
    UA_ByteString *trustList = NULL;
    size_t revocationListSize = 0;
    UA_ByteString *revocationList = NULL;
    UA_ServerConfig_setDefaultWithSecurityPolicies(
        pConfig, 4840,
        &keys.PublicCert, &keys.PrivateKey,
        trustList, trustListSize,
        issuerList, issuerListSize,
        revocationList, revocationListSize);
  }
  catch (std::exception &ex)
  {
    std::cout << "Could not load keys." << std::endl;
    std::cout << ex.what();
    std::cout << "Generate keys with create_self-signed.py in open62541/tools directory" << std::endl;
    UA_ServerConfig_setDefault(pConfig);
    std::cout << "No encryption will be available." << std::endl;
  }
  if(argc >= 2)
  {
    pConfig->customHostname = UA_STRING_ALLOC(argv[1]);
  }
  pConfig->nodeLifecycle.generateChildNodeId = generateChildNodeIdInParentNs;
  namespace_di_generated(pServer);
  namespace_industrial_automation_generated(pServer);
  namespace_machinery_generated(pServer);
  namespace_machinetool_generated(pServer);
  namespace_iswexample_generated(pServer);

  std::mutex accessDataMutex;
  NodesMaster n(pServer);
  machineTool::MachineTool_t machineTool;

  UA_ObjectAttributes objAttr = UA_ObjectAttributes_default;

  objAttr.displayName = UA_LOCALIZEDTEXT_ALLOC("", "Instance2");
  objAttr.eventNotifier = UA_EVENTNOTIFIERTYPE_SUBSCRIBETOEVENTS;
  open62541Cpp::UA_NodeId inst2((UA_UInt16)0, 0);

  {
    auto status = UA_Server_addObjectNode(
        pServer,
        UA_NODEID_NUMERIC(nsFromUri(pServer, constants::NsInstanceUri), 0),
        UA_NODEID_NUMERIC(nsFromUri(pServer, constants::NsMachineryUri), UA_MACHINERY_ID_MACHINES),
        UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
        *open62541Cpp::UA_QualifiedName(nsFromUri(pServer, constants::NsInstanceUri), "Instance2").QualifiedName,
        UA_NODEID_NUMERIC(nsFromUri(pServer, constants::NsMachineToolUri), UA_MACHINETOOLID_MACHINETOOLTYPE),
        objAttr,
        nullptr,
        inst2.NodeId);
  }

  UA_ObjectAttributes_clear(&objAttr);
  machineTool::MachineTool_t machineTool2;
  machineTool2.Identification->Manufacturer = open62541Cpp::LocalizedText_t{.locale = "c++", .text = "ISW Christian von Arnim"};
  machineTool2.Identification->YearOfConstruction = 2020;
  machineTool2.Identification->Location = std::string("AMB 0 0/N 48.781340 E 9.165731");

  machineTool.Identification->YearOfConstruction = 2020;
  machineTool.Identification->Model = open62541Cpp::LocalizedText_t{.locale = "", .text = "ISW Example"};
  machineTool.Identification->SoftwareRevision = std::string{"master"};
  machineTool.Identification->Manufacturer = open62541Cpp::LocalizedText_t{.locale = "", .text = "ISW Christian von Arnim"};
  machineTool.Identification->SerialNumber = std::string{"3-1415926535-8979323846"};

  machineTool.Identification->ProductInstanceUri.value = "Prototype 01";
  machineTool.Identification->ProductInstanceUri.StatusCode = UA_STATUSCODE_GOODEDITED;
  machineTool.Identification->ProductInstanceUri.SourceTimestamp = UA_DateTime_fromStruct(UA_DateTimeStruct{.sec = 13, .min = 12, .hour = 11, .day = 10, .month = 9, .year = 2008});

  bindMembersRefl(machineTool, pServer, open62541Cpp::UA_NodeId(6, UA_ISWEXAMPLE_ID_MACHINES_ISWEXAMPLEMACHINE), n);
  bindMembersRefl(machineTool2, pServer, inst2, n);

  auto &channel = machineTool2.Monitoring->Channels.Add(pServer, n, {6, "InstChannel1"});
  channel.ChannelState = UA_CHANNELSTATE_INTERRUPTED;
  channel.FeedOverride->Value = 89.0;

  //InstantiateOptional(machineTool2.Identification->Location, pServer, n);
  InstantiateOptional(machineTool2.Production->ActiveProgram->State, pServer, n);
  InstantiateOptional(machineTool2.Identification->YearOfConstruction, pServer, n);
  InstantiateOptional(machineTool2.Production->ProductionPlan, pServer, n);
  auto st = UA_Server_writeEventNotifier(pServer, *machineTool2.Production->ProductionPlan.NodeId.NodeId, UA_EVENTNOTIFIERTYPE_SUBSCRIBETOEVENTS);
  if (st != UA_STATUSCODE_GOOD)
  {
    std::cout << "WriteEventNotifier failed: " << UA_StatusCode_name(st) << std::endl;
  }
  InstantiateOptional(machineTool2.Production->ProductionPlan->NodeVersion, pServer, n);
  // Hack: Remove from bindings (Will be written by BindMemberPlaceholder)
  // Can keep binding when writing is supported.
  n.Remove(machineTool2.Production->ProductionPlan->NodeVersion.NodeId);
  auto &job = machineTool2.Production->ProductionPlan->OrderedObjects.Add(pServer, n, {6, "MyJob 1"});
  job.Identifier = std::string("ID 1");
  job.RunsCompleted = 0;
  job.RunsPlanned = 1;
  std::atomic_bool running{true};

  InstantiateOptional(machineTool2.Notification->Prognoses, pServer, n);
  InstantiateOptional(machineTool2.Notification->Prognoses->NodeVersion, pServer, n);
  // Hack: Remove from bindings (Will be written by BindMemberPlaceholder)
  // Can keep binding when writing is supported.
  n.Remove(machineTool2.Notification->Prognoses->NodeVersion.NodeId);
  auto &maintenancePrognosis = machineTool2.Notification->Prognoses->Prognosis.Add<machineTool::MaintenancePrognosis_t>(pServer, n, {6, "Maintenance"});
  maintenancePrognosis.Activity = open62541Cpp::LocalizedText_t{"", "Replace actuator."};
  auto &utilityPrognosis = machineTool2.Notification->Prognoses->Prognosis.Add<machineTool::UtilityChangePrognosis_t>(pServer, n, {6, "Utility"});
  utilityPrognosis.UtilityName = open62541Cpp::LocalizedText_t{"", "H²"};

  UA_Server_run_startup(pServer);
  std::unique_lock<decltype(accessDataMutex)> ul(accessDataMutex);
  std::thread t(simulate, &machineTool, std::ref(running), std::ref(accessDataMutex), pServer, std::ref(n), std::ref(machineTool2));
  ul.unlock();
  while (running)
  {
    ul.lock();
    std::uint16_t timeout = UA_Server_run_iterate(pServer, false);
    ul.unlock();
    std::this_thread::yield();
    std::this_thread::sleep_for(std::chrono::milliseconds(timeout));
  }

  std::cout << "Waiting for exiting simulate-thread." << std::endl;
  t.join();
  std::cout << "Shut down OPC UA Server" << std::endl;
  UA_Server_run_shutdown(pServer);
  UA_Server_delete(pServer);
  return 0;
}
