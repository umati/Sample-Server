#include "MT.hpp"
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

std::shared_ptr<OpcUaCondition<Alert_t>> pCondition;

void simulate(MT::MachineTool_t *pMachineTool,
              std::atomic_bool &running,
              std::mutex &accessDataMutex,
              UA_Server *pServer)
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
      NotificationEvent_t notifEvent;

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
    if((i%10) == 1)
    {
      pCondition = std::make_shared<OpcUaCondition<Alert_t>>(pServer, open62541Cpp::UA_NodeId(UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER)));
      pCondition->Data.ErrorCode = "ERR404";
      std::stringstream ss;
      ss << "Cond Message: " << i;
      pCondition->Data.Message = open62541Cpp::LocalizedText_t{"", ss.str()};
      pCondition->Data.SourceName = "SrcCond";
      pCondition->Data.Severity = 123;
      pCondition->Data.Retain = true;
      pCondition->Data.EnabledState->Value = {"", "Active"};
      pCondition->Data.EnabledState->Id = true;

      pCondition->Trigger();
    }
    else if((i%10) == 5)
    {
      if(pCondition)
      {
        pCondition->Data.Retain = false;
        pCondition->Data.EnabledState->Id = true;
        pCondition->Data.EnabledState->Value = {"", "Inactive"};
        pCondition->Trigger();
        pCondition = nullptr;
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

int main(int argc, char *argv[])
{
  UA_Server *pServer = UA_Server_new();
  UA_ServerConfig_setDefault(UA_Server_getConfig(pServer));

  std::cout << "ExampleUmatiServer" << std::endl;
  /*UA_Server_addNamespace(pServer, "http://opcfoundation.org/UA/DI/");
  UA_Server_addNamespace(pServer, "http://opcfoundation.org/UA/Machinery/");
  UA_Server_addNamespace(pServer, "http://opcfoundation.org/UA/IA");
  UA_Server_addNamespace(pServer, "http://opcfoundation.org/UA/MachineTool/");*/

  namespace_di_generated(pServer);
  namespace_industrial_automation_generated(pServer);
  namespace_machinery_generated(pServer);
  namespace_machinetool_generated(pServer);
  namespace_iswexample_generated(pServer);

  UA_ObjectAttributes objAttr = UA_ObjectAttributes_default;

  objAttr.displayName = UA_LOCALIZEDTEXT_ALLOC("", "Instance2");
  objAttr.eventNotifier = UA_EVENTNOTIFIERTYPE_SUBSCRIBETOEVENTS;
  open62541Cpp::UA_NodeId inst2((UA_UInt16)0, 0);

  {
    auto status = UA_Server_addObjectNode(
        pServer,
        UA_NODEID_NULL,
        UA_NODEID_NUMERIC(nsFromUri(pServer, constants::NsMachineryUri), UA_MACHINERY_ID_MACHINES),
        UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
        *open62541Cpp::UA_QualifiedName(nsFromUri(pServer, constants::NsInstanceUri), "Instance2").QualifiedName,
        UA_NODEID_NUMERIC(nsFromUri(pServer, constants::NsMachineToolUri), UA_MACHINETOOLID_MACHINETOOLTYPE),
        objAttr,
        nullptr,
        inst2.NodeId);
  }
  UA_ObjectAttributes_clear(&objAttr);
  MT::MachineTool_t machineTool2;
  machineTool2.Identification->Manufacturer = open62541Cpp::LocalizedText_t{.locale = "c++", .text = "ISW Christian von Arnim"};
  machineTool2.Identification->YearOfConstruction = 2020;

  std::mutex accessDataMutex;
  NodesMaster n(pServer);
  MT::MachineTool_t machineTool;

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

  // Assign placeholders after binding!
  {
    int i = 1;
    for (auto &job : machineTool.Production->ProductionPlan->Jobs)
    {
      std::stringstream ss;
      ss << "ID_" << i++ << std::endl;
      job.Identifier = ss.str();
      job.RunsCompleted = 8;
      job.RunsPlanned = 10;
      job.State->CurrentState->Value = open62541Cpp::LocalizedText_t{.locale = "en-en", .text = "Testing State"};
      job.State->CurrentState->Number = 1234;
    }

    for (auto &channel : machineTool.Monitoring->Channels)
    {
      channel.ChannelState = UA_ChannelState::UA_CHANNELSTATE_INTERRUPTED;
      channel.FeedOverride->Value = 6;
      *channel.FeedOverride->EURange = {
          .low = 123.45,
          .high = 234.56};
      *channel.FeedOverride->EngineeringUnits = {
          .NamespaceUri = "eu://meter",
          .UnitId = -1,
          .DisplayName{.locale = "", .text = "Meter"},
          .Description = {.locale = "en", .text = "100cm"},
      };
    }
  }

  n.SetCallbacks();
  InstatiateOptional(machineTool2.Identification->YearOfConstruction, pServer, n);
  InstatiateOptional(machineTool2.Production->ProductionPlan, pServer, n);
  n.SetCallbacks();

  std::atomic_bool running{true};

  UA_Server_run_startup(pServer);
  std::unique_lock<decltype(accessDataMutex)> ul(accessDataMutex);
  std::thread t(simulate, &machineTool, std::ref(running), std::ref(accessDataMutex), pServer);
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