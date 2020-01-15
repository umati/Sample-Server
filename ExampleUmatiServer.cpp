#include <iostream>
#include <open62541/server_config_default.h>
#include "src_generated/namespace_iswexample_generated.h"
#include "src_generated/namespace_umati_generated.h"
#include "src_generated/iswexample.h"
#include "src_generated/umati_nodeids.h"
#include <cstdint>
#include <functional>
#include "NodeValue.hpp"
#include "NodesMaster.hpp"
#include "BindValue.hpp"
#include "SetupEvents.hpp"
#include <Open62541Cpp/UA_RelativPathElement.hpp>
#include <Open62541Cpp/UA_BrowsePath.hpp>
#include <thread>
#include <chrono>
#include <mutex>
#include <atomic>
#include <Open62541Cpp/UA_String.hpp>
#include <list>
#include <Open62541Cpp/UA_RelativPathBase.hpp>
#include "BindValueHelper.hpp"

struct IdentificationMachine_t {
  std::uint32_t BuildYear;
  std::string CatalogueName;
  std::string CustomName;
  std::string Manufacturer;
  std::string SerialNumber;

  void bind(UA_Server *pServer, UA_NodeId machine, NodesMaster &nodesMaster) {
    open62541Cpp::UA_RelativPathBase IdentificationMachine({open62541Cpp::UA_RelativPathElement(2, "Identification"), open62541Cpp::UA_RelativPathElement(2, "Machine")});

    bindValueByPath(pServer,
                    open62541Cpp::UA_BrowsePath(machine, IdentificationMachine(open62541Cpp::UA_RelativPathElement(2, "BuildYear"))),
                    nodesMaster,
                    this->BuildYear);
    bindValueByPath(pServer,
                    open62541Cpp::UA_BrowsePath(machine, IdentificationMachine(open62541Cpp::UA_RelativPathElement(2, "CatalogueName"))),
                    nodesMaster,
                    this->CatalogueName);
    bindValueByPath(pServer,
                    open62541Cpp::UA_BrowsePath(machine, IdentificationMachine(open62541Cpp::UA_RelativPathElement(2, "CustomName"))),
                    nodesMaster,
                    this->CustomName);
    bindValueByPath(pServer,
                    open62541Cpp::UA_BrowsePath(machine, IdentificationMachine(open62541Cpp::UA_RelativPathElement(2, "Manufacturer"))),
                    nodesMaster,
                    this->Manufacturer);
    bindValueByPath(pServer,
                    open62541Cpp::UA_BrowsePath(machine, IdentificationMachine(open62541Cpp::UA_RelativPathElement(2, "SerialNumber"))),
                    nodesMaster,
                    this->SerialNumber);
  }
};

struct IdentificationSoftware_t {
  std::string ComponentVersion;
  std::string Identifier;
  void bind(UA_Server *pServer, UA_NodeId machine, NodesMaster &nodesMaster) {
    open62541Cpp::UA_RelativPathBase
        IdentificationSoftware({open62541Cpp::UA_RelativPathElement(2, "Identification"), open62541Cpp::UA_RelativPathElement(2, "Software")});
    bindValueByPath(pServer,
                    open62541Cpp::UA_BrowsePath(machine, IdentificationSoftware(open62541Cpp::UA_RelativPathElement(2, "ComponentVersion"))),
                    nodesMaster,
                    this->ComponentVersion);
    bindValueByPath(pServer,
                    open62541Cpp::UA_BrowsePath(machine, IdentificationSoftware(open62541Cpp::UA_RelativPathElement(2, "Identifier"))),
                    nodesMaster,
                    this->Identifier);
  }
};

bool first = true;

void simulate(IdentificationMachine_t *pInfo,
              std::atomic_bool &running,
              std::mutex &accessDataMutex,
              UA_Server *pServer) {
  std::unique_lock<std::remove_reference<decltype(accessDataMutex)>::type> ul(accessDataMutex);
  ul.unlock();
  while (running) {
    ul.lock();
    ++(pInfo->BuildYear);
    NotificationEvent_t ev{.Identifier = "MyId", .Message="MessageTxt", .SourceName="MySource", .Severity = 500};
    auto evNodeId = setupNotificationEvent(pServer, ev);
    auto retval = UA_Server_triggerEvent(pServer, evNodeId, UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER), NULL, UA_TRUE);
    if (first) {
      AlertCondition_t aev;
      aev.Identifier = "CondId";
      aev.Message = "CondMessage";
      aev.SourceName = "SrcCond";
      aev.Severity = 123;
      aev.Retain = true;

      auto condNodeId = setupAlertConditionType(pServer, aev);
      UA_Server_triggerConditionEvent(pServer, condNodeId, UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER), NULL);
    }
    first = false;
    ul.unlock();
    std::this_thread::yield();
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }

}

int main(int argc, char *argv[]) {

  UA_Server *pServer = UA_Server_new();
  UA_ServerConfig_setDefault(UA_Server_getConfig(pServer));

  std::cout << "ExampleUmatiServer" << std::endl;
  namespace_umati_generated(pServer);
  namespace_iswexample_generated(pServer);

  std::mutex accessDataMutex;
  NodesMaster n(pServer);
  IdentificationMachine_t identificationMachine =
      {
          .BuildYear = 2020,
          .CatalogueName = "ISW Example",
          .CustomName="My Custom Server",
          .Manufacturer="ISW Christian von Arnim",
          .SerialNumber="3-1415926535-8979323846"
      };

  IdentificationSoftware_t identificationSoftware = {
      .ComponentVersion = "master@2020-01-13",
      .Identifier = "OPC UA Server Open62541"
  };

  identificationMachine.bind(pServer, UA_NODEID_NUMERIC(3, UA_ISWEXAMPLE_ID_MACHINETOOLS_ISWEXAMPLE), n);
  identificationSoftware.bind(pServer, UA_NODEID_NUMERIC(3, UA_ISWEXAMPLE_ID_MACHINETOOLS_ISWEXAMPLE), n);

  n.SetCallbacks();

  std::atomic_bool running{true};

  //UA_Server_run(pServer, &running);
  UA_Server_run_startup(pServer);
  std::unique_lock<decltype(accessDataMutex)> ul(accessDataMutex);
  std::thread t(simulate, &identificationMachine, std::ref(running), std::ref(accessDataMutex), pServer);
  ul.unlock();
  while (running) {
    ul.lock();
    std::uint16_t timeout = UA_Server_run_iterate(pServer, false);
    ul.unlock();
    std::this_thread::yield();
    std::this_thread::sleep_for(std::chrono::milliseconds(timeout));
  }

  UA_Server_run_shutdown(pServer);

  return 0;
}