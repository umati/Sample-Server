/* MIT License
 *
 * Copyright (c) 2019-2021  Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 * Copyright 2020 (c) Tonja Heinemann, ISW University of Stuttgart (for umati and VDW e.V.)
 * Copyright 2021 (c) Götz Görisch, VDW - Verein Deutscher Werkzeugmaschinenfabriken e.V.
 * Copyright 2021 (c) Christoph Ruckstetter, Michael Weinig AG
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <open62541/plugin/accesscontrol_default.h>
#include <open62541/plugin/pki_default.h>
#include <open62541/server_config_default.h>

#include <algorithm>
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
#include "MachineTools/InstantiatedMachineTool.hpp"
#include "MachineTools/Relab.hpp"
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

#include "deps/open62541/plugins/ua_network_pubsub_mqtt.h"
#include "UmatiServerLib/Publication.hpp"

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

UA_StatusCode addMQTTPubSubConnection(UA_Server *pServer, UA_ServerConfig *pConfig, 
                                  const Configuration::Configuration &configFile, 
                                  UA_NodeId &connectionIdent) {
  UA_StatusCode status = UA_STATUSCODE_GOOD;
  UA_PubSubConnectionConfig cConfig;
  
  if(!configFile.MQTTPubSub.has_value()) {
    return status;
  }

  status = UA_ServerConfig_addPubSubTransportLayer(pConfig, UA_PubSubTransportLayerMQTT());
  if(status != UA_STATUSCODE_GOOD) {
    return status;
  }

  MqttConnectionConfig mqttConnectionConfig{};
  mqttConnectionConfig.setBrokerUrl(configFile.MQTTPubSub.value().BrokerUrl);
  mqttConnectionConfig.setClientId(configFile.MQTTPubSub.value().PublisherId);
  mqttConnectionConfig.setSendBufferSize(1000000);
  if(configFile.MQTTPubSub.value().Username.has_value()) {
    mqttConnectionConfig.setUsername(configFile.MQTTPubSub.value().Username.value()); }
  if(configFile.MQTTPubSub.value().Password.has_value()) {
    mqttConnectionConfig.setPassword(configFile.MQTTPubSub.value().Password.value()); }
  if(configFile.MQTTPubSub.value().MqttCaFile.has_value()) {
    mqttConnectionConfig.setMqttCaFilePath(configFile.MQTTPubSub.value().MqttCaFile.value()); }

  connectionIdent = mqttConnectionConfig.buildAndApplyConnectionOptions(pServer).getConnectionIdent();
  return status;
}

UA_StatusCode setServerConfig(UA_ServerConfig *pConfig, const Configuration::Configuration &configFile, const OpcUaKeys &keys) {
  auto status = UA_ServerConfig_setBasics(pConfig);
  if (status != UA_STATUSCODE_GOOD) {
    return status;
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

  // Do not limit clients
  UA_CertificateVerification_AcceptAll(&pConfig->certificateVerification);
  // Use Default sizes
  std::vector<std::string> serverUrls;
  constexpr int port = 4840;
  {
    std::stringstream ss;
    ss << "opc.tcp://:" << port;
    serverUrls.push_back(ss.str());
  }
  if (configFile.Hostname.has_value()) {
    std::stringstream ss;
    ss << "opc.tcp://" << *configFile.Hostname << ":" << port;
    serverUrls.push_back(ss.str());
  }
  pConfig->serverUrls = (UA_String *)UA_Array_new(serverUrls.size(), &UA_TYPES[UA_TYPES_STRING]);
  pConfig->serverUrlsSize = serverUrls.size();
  std::transform(
    std::begin(serverUrls), std::end(serverUrls), pConfig->serverUrls, [](const std::string &serverUrl) { return UA_String_fromChars(serverUrl.c_str()); });
  status = UA_ServerConfig_addAllSecurityPolicies(pConfig, &keys.PublicCert, &keys.PrivateKey);
  if (status != UA_STATUSCODE_GOOD) {
    return status;
  }
  status = UA_ServerConfig_addSecurityPolicyNone(pConfig, NULL);
  if (status != UA_STATUSCODE_GOOD) {
    return status;
  }
  std::vector<UA_UsernamePasswordLogin> users;
  if (configFile.UserPassAuthentication.has_value()) {
    users.reserve(configFile.UserPassAuthentication->size());
    for (auto up : configFile.UserPassAuthentication.value()) {
      users.push_back(UA_UsernamePasswordLogin{.username = UA_STRING((char *)up.Username.c_str()), .password = UA_STRING((char *)up.Password.c_str())});
    }
  }
  status = UA_AccessControl_default(
    pConfig, true, NULL, &pConfig->securityPolicies[pConfig->securityPoliciesSize - 1].policyUri, users.size(), users.size() == 0 ? nullptr : &users[0]);
  if (status != UA_STATUSCODE_GOOD) {
    return status;
  }
  status = UA_ServerConfig_addAllEndpoints(pConfig);
  if (status != UA_STATUSCODE_GOOD) {
    return status;
  }
  return status;
}

int main(int argc, char *argv[]) {
  signal(SIGINT, sigHandler);
  signal(SIGABRT, sigHandler);
  signal(SIGTERM, sigHandler);
  Configuration::Configuration serverConfig = Configuration::DefaultConfiguration();
  std::string configurationFilename = "configuration.json";
  if (argc >= 2) {
    configurationFilename = argv[1];
  }
  try {
    serverConfig = Configuration::FromJsonFile(configurationFilename);
  } catch (std::exception &e) {
    std::cout << "Could not load configuration, using a possible insecure default one." << std::endl;
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
    try {
      keys.Load();
    } catch (std::exception &ex) {
      std::cout << "Could not load keys for encryption." << std::endl;
      std::cout << ex.what();
      if (keys.PrivateKey.length == 0 && keys.PublicCert.length == 0) {
        std::cout << "Generate and store new keys.";
        keys.GenerateKeys();
        keys.StoreKeys();
      }
    }
    setServerConfig(pConfig, serverConfig, keys);
  } catch (std::exception &ex) {
    std::cout << "Could not load keys for encryption." << std::endl;
    std::cout << ex.what();
    std::cout << "Generate keys with tool/certGen/createCertification.py" << std::endl;
    UA_ServerConfig_setDefault(pConfig);
    std::cout << "No encryption will be available." << std::endl;
  }

  // Create namespaces
  namespace_di_generated(pServer);
  namespace_ia_generated(pServer);
  namespace_machinery_generated(pServer);
  namespace_machinetool_generated(pServer);
  /*namespace_robotics_generated(pServer);*/
  namespace_woodworking_generated(pServer);

  std::mutex accessDataMutex;


  UA_NodeId connectionIdentFMTD{};
  UA_NodeId connectionIdentWood{};
  UA_NodeId connectionIdentBMT{};
  UA_NodeId connectionIdentMRMT{};
  UA_NodeId connectionIdentSCMT{};

  /*
  serverConfig.MQTTPubSub->PublisherId = "FullMachineToolDynamic";
  addMQTTPubSubConnection(pServer, pConfig, serverConfig, connectionIdentFMTD);
  serverConfig.MQTTPubSub->PublisherId = "FullWoodworking";
  addMQTTPubSubConnection(pServer, pConfig, serverConfig, connectionIdentWood);
  serverConfig.MQTTPubSub->PublisherId = "BasicMachineTool";
  addMQTTPubSubConnection(pServer, pConfig, serverConfig, connectionIdentBMT);
  serverConfig.MQTTPubSub->PublisherId = "MoreRealisticMachineTool";
  addMQTTPubSubConnection(pServer, pConfig, serverConfig, connectionIdentMRMT);
  serverConfig.MQTTPubSub->PublisherId = "ShowcaseMachineTool";
  addMQTTPubSubConnection(pServer, pConfig, serverConfig, connectionIdentSCMT);
  */

  std::list<std::shared_ptr<SimulatedInstance>> machineTools;
  /*
  if (serverConfig.MQTTPubSub.has_value()) {  
    machineTools.push_back(std::make_shared<FullMachineToolDynamic>(pServer, true,
      InstantiatedMachineTool::MqttSettings{&connectionIdentFMTD, serverConfig.MQTTPubSub->Prefix, 
                                            "FullMachineToolDynamic", 
                                            UA_NODEID_NULL}));
  } else {
    machineTools.push_back(std::make_shared<FullWoodworking>(pServer));
  }

  if (serverConfig.MQTTPubSub.has_value()) {  
    machineTools.push_back(std::make_shared<FullWoodworking>(pServer,
      InstantiatedMachineTool::MqttSettings{&connectionIdentWood, serverConfig.MQTTPubSub->Prefix, 
                                            "FullWoodworking", 
                                            UA_NODEID_NULL}));
  } else {
    machineTools.push_back(std::make_shared<FullWoodworking>(pServer));
  }

  if (serverConfig.MQTTPubSub.has_value()) {  
    machineTools.push_back(std::make_shared<BasicMachineTool>(pServer,
      InstantiatedMachineTool::MqttSettings{&connectionIdentBMT, serverConfig.MQTTPubSub->Prefix, 
                                            "BasicMachineTool", 
                                            UA_NODEID_NULL}));
  } else {
    machineTools.push_back(std::make_shared<BasicMachineTool>(pServer));
  }

  if (serverConfig.MQTTPubSub.has_value()) {  
    machineTools.push_back(std::make_shared<MRMachineTool>(pServer, 
      InstantiatedMachineTool::MqttSettings{&connectionIdentMRMT, serverConfig.MQTTPubSub->Prefix, 
                                            "MoreRealisticMachineTool", 
                                            UA_NODEID_NULL}));
  } else {
    machineTools.push_back(std::make_shared<MRMachineTool>(pServer));
  }

  */
  if (serverConfig.MQTTPubSub.has_value()) {  
    machineTools.push_back(std::make_shared<Relab>(pServer, 
      InstantiatedMachineTool::MqttSettings{&connectionIdentSCMT, serverConfig.MQTTPubSub->Prefix, 
                                            "Relab", 
                                            UA_NODEID_NULL}));
  } else {
    machineTools.push_back(std::make_shared<Relab>(pServer));
  }

  // machineTools.push_back(std::make_shared<CNShowcaseMachineTool>(pServer));
  /*machineTools.push_back(std::make_shared<BasicRobot>(pServer));*/
  // machineTools.push_back(std::make_shared<BasicWoodworking>(pServer));

  UA_Server_run_startup(pServer);
  std::unique_lock<decltype(accessDataMutex)> ul(accessDataMutex);
  std::thread t(simulate, std::ref(accessDataMutex), pServer, std::ref(machineTools));
  ul.unlock();
  while (running) {
    ul.lock();
    std::uint16_t timeout = UA_Server_run_iterate(pServer, true);
    // timeout = UA_Server_run_iterate(pServer, true);

    // Limit wait time, as UA_Server_run_iterate may return large numbers, when no clients are connected.
    timeout = (std::uint16_t) 50;
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
