#include "src_generated/namespace_machinetool_generated.h"
#include "src_generated/namespace_machinery_generated.h"
#include "src_generated/namespace_di_generated.h"
#include "src_generated/namespace_industrial_automation_generated.h"

#include "OpcUaKeys.hpp"

#include <iostream>
#include <open62541/server_config_default.h>
#include <functional>
#include <thread>
#include <mutex>
#include <atomic>
#include <list>

#include "MachineTools/FullMachineTool.hpp"
#include "MachineTools/BasicMachineTool.hpp"

std::atomic_bool running {true};
void sigHandler(int sig)
{
  std::cout << "Caught signal " << sig << std::endl;
  std::cout << "Stop application..." << std::endl;
  running = false;
}
void simulate(
    std::mutex &accessDataMutex,
    UA_Server *pServer,
    std::list<std::shared_ptr<SimulatedMachineTool>> &machineTools)
{
  std::unique_lock<std::remove_reference<decltype(accessDataMutex)>::type> ul(accessDataMutex);
  ul.unlock();
  int i = 0;
  while (running)
  {
    ul.lock();
    for (auto &mt : machineTools)
    {
      mt->Simulate();
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
  signal(SIGINT, sigHandler);
  signal(SIGABRT, sigHandler);
  signal(SIGTERM, sigHandler);

  std::cout << "ExampleMTServer, exit with Ctrl+C" << std::endl;
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
    std::cout << "Could not load keys 'server_key.der' and 'server_cert.der'  for encryption." << std::endl;
    std::cout << ex.what();
    std::cout << "Generate keys with create_self-signed.py in the open62541/tools directory" << std::endl;
    UA_ServerConfig_setDefault(pConfig);
    std::cout << "No encryption will be available." << std::endl;
  }

  if (argc >= 2)
  {
    pConfig->customHostname = UA_STRING_ALLOC(argv[1]);
  }
  pConfig->nodeLifecycle.generateChildNodeId = generateChildNodeIdInParentNs;

  // Create namespacs
  namespace_di_generated(pServer);
  namespace_industrial_automation_generated(pServer);
  namespace_machinery_generated(pServer);
  namespace_machinetool_generated(pServer);

  std::mutex accessDataMutex;

  std::list<std::shared_ptr<SimulatedMachineTool>> machineTools;
  machineTools.push_back(std::make_shared<FullMachineTool>(pServer));
  machineTools.push_back(std::make_shared<BasicMachineTool>(pServer));

  UA_Server_run_startup(pServer);
  std::unique_lock<decltype(accessDataMutex)> ul(accessDataMutex);
  std::thread t(simulate, std::ref(accessDataMutex), pServer, std::ref(machineTools));
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
  // Ensure that all simulated machines are cleared, so no interaction with the OPC UA Server anymore.
  machineTools.erase(machineTools.begin(), machineTools.end());
  std::cout << "Shut down OPC UA Server" << std::endl;
  UA_Server_run_shutdown(pServer);
  UA_Server_delete(pServer);
  return 0;
}
