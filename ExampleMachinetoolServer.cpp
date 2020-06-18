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

#include "MachineTools/FullMachineTool.hpp"
#include <list>

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
              std::list<std::shared_ptr<SimulatedMachineTool>> &machineTools)
{
  std::unique_lock<std::remove_reference<decltype(accessDataMutex)>::type> ul(accessDataMutex);
  ul.unlock();
  int i = 0;
  while (running)
  {
    ul.lock();
    ++(pMachineTool->Identification->YearOfConstruction.value);
    //pChannel1->ChannelState = static_cast<UA_ChannelState>((((int)pChannel1->ChannelState) + 1) % (UA_ChannelState::UA_CHANNELSTATE_RESET + 1));
    for(auto &mt: machineTools)
    {
      mt->Simulate();
    }

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
  machineTool.Identification->YearOfConstruction = 2020;
  machineTool.Identification->Model = open62541Cpp::LocalizedText_t{.locale = "", .text = "ISW Example"};
  machineTool.Identification->SoftwareRevision = std::string{"master"};
  machineTool.Identification->Manufacturer = open62541Cpp::LocalizedText_t{.locale = "", .text = "ISW Christian von Arnim"};
  machineTool.Identification->SerialNumber = std::string{"3-1415926535-8979323846"};

  machineTool.Identification->ProductInstanceUri.value = "Prototype 01";
  machineTool.Identification->ProductInstanceUri.StatusCode = UA_STATUSCODE_GOODEDITED;
  machineTool.Identification->ProductInstanceUri.SourceTimestamp = UA_DateTime_fromStruct(UA_DateTimeStruct{.sec = 13, .min = 12, .hour = 11, .day = 10, .month = 9, .year = 2008});

  UmatiServerLib::Bind::MembersRefl(machineTool, pServer, open62541Cpp::UA_NodeId(6, UA_ISWEXAMPLE_ID_MACHINES_ISWEXAMPLEMACHINE), n);


  std::atomic_bool running{true};

  std::list<std::shared_ptr<SimulatedMachineTool>> machineTools;
  machineTools.push_back(std::make_shared<FullMachineTool>(pServer));

  UA_Server_run_startup(pServer);
  std::unique_lock<decltype(accessDataMutex)> ul(accessDataMutex);
  std::thread t(simulate, &machineTool, std::ref(running), std::ref(accessDataMutex), pServer, std::ref(n), std::ref(machineTools));
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
