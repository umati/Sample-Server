#include <open62541/server_config_default.h>

#include <atomic>
#include <csignal>
#include <functional>
#include <iostream>
#include <list>
#include <mutex>
#include <thread>

#include "Configuration/Configuration_json.hpp"
#include "MachineTools/BasicMachineTool.hpp"
#include "MachineTools/CNShowcaseMachineTool.hpp"
#include "MachineTools/FullMachineTool.hpp"
#include "MachineTools/FullMachineToolDynamic.hpp"
#include "MachineTools/MRMachineTool.hpp"
#include "MachineTools/ShowcaseMachineTool.hpp"
/*#include "Robotics/BasicRobot.hpp"*/
#include "UmatiServerLib/OpcUaKeys.hpp"
#include "Woodworking/BasicWoodworking.hpp"
#include "Woodworking/FullWoodworking.hpp"
#include "src_generated/namespace_di_generated.h"
#include "src_generated/namespace_ia_generated.h"
#include "src_generated/namespace_machinery_generated.h"
#include "src_generated/namespace_machinetool_generated.h"
/*#include "src_generated/namespace_robotics_generated.h"*/
#include "src_generated/namespace_woodworking_generated.h"

std::atomic_bool running{true};
void sigHandler(int sig) {
  std::cout << "Caught signal " << sig << std::endl;
  std::cout << "Stop application..." << std::endl;
  running = false;
}
void simulate(std::mutex &accessDataMutex, UA_Server *pServer, std::list<std::shared_ptr<SimulatedInstance>> &machineTools) {
  std::unique_lock<std::remove_reference<decltype(accessDataMutex)>::type> ul(accessDataMutex);
  ul.unlock();
  int i = 0;
  while (running) {
    ul.lock();
    for (auto &mt : machineTools) {
      mt->Simulate();
    }

    ++i;
    ul.unlock();
    std::this_thread::yield();
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
}

UA_StatusCode generateChildNodeIdInParentNs(
  UA_Server *server,
  const UA_NodeId *sessionId,
  void *sessionContext,
  const UA_NodeId *sourceNodeId,
  const UA_NodeId *targetParentNodeId,
  const UA_NodeId *referenceTypeId,
  UA_NodeId *targetNodeId) {
  if (UA_NodeId_equal(targetNodeId, &UA_NODEID_NULL) && !UA_NodeId_equal(targetParentNodeId, &UA_NODEID_NULL)) {
    targetNodeId->namespaceIndex = targetParentNodeId->namespaceIndex;
  }
  return UA_STATUSCODE_GOOD;
}

int main(int argc, char *argv[]) {
  signal(SIGINT, sigHandler);
  signal(SIGABRT, sigHandler);
  signal(SIGTERM, sigHandler);
  Configuration::Configuration serverConfig;
  std::string configurationFilename = "configuration.json";
  if (argc >= 2) {
    configurationFilename = argv[1];
  }
  try {
    serverConfig = Configuration::FromJsonFile(configurationFilename);
  } catch (std::exception &e) {
    std::cout << "Could not load configuration, using an insecure default one." << std::endl;
    std::cout << e.what() << std::endl;
  }
  std::cout << "SampleServer, exit with Ctrl+C" << std::endl;
  UA_Server *pServer = UA_Server_new();
  auto pConfig = UA_Server_getConfig(pServer);

  try {
    if (!serverConfig.Encryption.has_value()) {
      throw std::runtime_error("Encryption not configured.");
    }
    OpcUaKeys keys(
      serverConfig.Encryption->ServerKey,
      serverConfig.Encryption->ServerCert,
      serverConfig.Encryption->TrustedClients,
      serverConfig.Encryption->IssuerCerts,
      serverConfig.Encryption->Revocation);
    keys.Load();
    // Skip all certificate checks
    size_t issuerListSize = 0;
    UA_ByteString *issuerList = NULL;
    size_t trustListSize = 0;
    UA_ByteString *trustList = NULL;
    size_t revocationListSize = 0;
    UA_ByteString *revocationList = NULL;
    UA_ServerConfig_setDefaultWithSecurityPolicies(
      pConfig,
      4840,
      &keys.PublicCert,
      &keys.PrivateKey,
      &keys.Trusted[0],
      keys.Trusted.size(),
      &keys.Issuer[0],
      keys.Issuer.size(),
      &keys.Revoked[0],
      keys.Revoked.size());
  } catch (std::exception &ex) {
    std::cout << "Could not load keys for encryption." << std::endl;
    std::cout << ex.what();
    std::cout << "Generate keys with tool/certGen/createCertification.py" << std::endl;
    UA_ServerConfig_setDefault(pConfig);
    std::cout << "No encryption will be available." << std::endl;
  }

  if (serverConfig.Hostname.has_value()) {
    UA_String_clear(&pConfig->customHostname);
    pConfig->customHostname = UA_STRING_ALLOC(serverConfig.Hostname->c_str());
  }
  pConfig->nodeLifecycle.generateChildNodeId = generateChildNodeIdInParentNs;
  // Companion Specificaitons will trigger many warnings, and values in instances are set later
  pConfig->allowEmptyVariables = UA_RuleHandling::UA_RULEHANDLING_ACCEPT;
  pConfig->modellingRulesOnInstances = UA_FALSE;
  UA_String_clear(&pConfig->applicationDescription.applicationUri);
  pConfig->applicationDescription.applicationUri = UA_STRING_ALLOC("urn:UmatiSampleServer");
  UA_LocalizedText_clear(&pConfig->applicationDescription.applicationName);
  pConfig->applicationDescription.applicationName = UA_LOCALIZEDTEXT_ALLOC("en", "Umati Sample Server");
  UA_String_clear(&pConfig->applicationDescription.productUri);
  pConfig->applicationDescription.productUri = UA_STRING_ALLOC("https://github.com/umati/Sample-Server/");

  // Create namespaces
  namespace_di_generated(pServer);
  namespace_ia_generated(pServer);
  namespace_machinery_generated(pServer);
  namespace_machinetool_generated(pServer);
  /*namespace_robotics_generated(pServer);*/
  namespace_woodworking_generated(pServer);

  std::mutex accessDataMutex;

  std::list<std::shared_ptr<SimulatedInstance>> machineTools;
  machineTools.push_back(std::make_shared<FullMachineTool>(pServer));
  // machineTools.push_back(std::make_shared<FullMachineToolDynamic>(pServer));
  machineTools.push_back(std::make_shared<BasicMachineTool>(pServer));
  // machineTools.push_back(std::make_shared<MRMachineTool>(pServer));
  // machineTools.push_back(std::make_shared<ShowcaseMachineTool>(pServer));
  // machineTools.push_back(std::make_shared<CNShowcaseMachineTool>(pServer));
  /*machineTools.push_back(std::make_shared<BasicRobot>(pServer));*/
  machineTools.push_back(std::make_shared<BasicWoodworking>(pServer));
  machineTools.push_back(std::make_shared<FullWoodworking>(pServer));

  UA_Server_run_startup(pServer);
  std::unique_lock<decltype(accessDataMutex)> ul(accessDataMutex);
  std::thread t(simulate, std::ref(accessDataMutex), pServer, std::ref(machineTools));
  ul.unlock();
  while (running) {
    ul.lock();
    std::uint16_t timeout = UA_Server_run_iterate(pServer, false);
    ul.unlock();
    // std::this_thread::yield();
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
