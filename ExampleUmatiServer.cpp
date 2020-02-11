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

#include <refl.hpp>

struct IdentificationMachine_t {
  std::uint32_t BuildYear;
  std::string CatalogueName;
  std::string CustomName;
  std::string Manufacturer;
  std::string SerialNumber;

  void bind(UA_Server *pServer, UA_NodeId machine, NodesMaster &nodesMaster);
};

REFL_TYPE(IdentificationMachine_t)
  REFL_FIELD(BuildYear)
  REFL_FIELD(CatalogueName)
  REFL_FIELD(CustomName)
  REFL_FIELD(Manufacturer)
  REFL_FIELD(SerialNumber)
REFL_END

void IdentificationMachine_t::bind(UA_Server *pServer, UA_NodeId machine, NodesMaster &nodesMaster) {
  open62541Cpp::UA_RelativPathBase Base
      ({open62541Cpp::UA_RelativPathElement(2, "Identification"), open62541Cpp::UA_RelativPathElement(2, "Machine")});

  for_each(refl::reflect(*this).members, [&](auto member) {
    bindValueByPath(pServer,
                    open62541Cpp::UA_BrowsePath(
                        machine,
                        Base(open62541Cpp::UA_RelativPathElement(2, std::string(member.name)))
                    ),
                    nodesMaster,
                    member(*this));

  });
}

struct IdentificationSoftware_t {
  std::string ComponentVersion;
  std::string Identifier;
  void bind(UA_Server *pServer, UA_NodeId machine, NodesMaster &nodesMaster);
};

REFL_TYPE(IdentificationSoftware_t)
  REFL_FIELD(ComponentVersion)
  REFL_FIELD(Identifier)
REFL_END

void IdentificationSoftware_t::bind(UA_Server *pServer, UA_NodeId machine, NodesMaster &nodesMaster) {
  open62541Cpp::UA_RelativPathBase
      Base
      ({open62541Cpp::UA_RelativPathElement(2, "Identification"), open62541Cpp::UA_RelativPathElement(2, "Software")});

  for_each(refl::reflect(*this).members, [&](auto member) {
    bindValueByPath(pServer,
                    open62541Cpp::UA_BrowsePath(
                        machine,
                        Base(open62541Cpp::UA_RelativPathElement(2, std::string(member.name)))
                    ),
                    nodesMaster,
                    member(*this));

  });
}

template<typename T>
struct OverrideItemType_t {
  T Value;
  UA_Range EURange;
  UA_EUInformation EngineeringUnits;
};

struct ChannelMonitoringType_t {
  UA_ChannelState ChannelState;
  OverrideItemType_t<double> FeedOverride;
  void bind(UA_Server *pServer, UA_NodeId channel, NodesMaster &nodesMaster) {
    open62541Cpp::UA_RelativPathBase ChannelBase;
    bindValueByPath(pServer,
                    open62541Cpp::UA_BrowsePath(channel,
                                                ChannelBase(open62541Cpp::UA_RelativPathElement(2, "ChannelState"))),
                    nodesMaster,
                    this->ChannelState);

    bindValueByPath(pServer,
                    open62541Cpp::UA_BrowsePath(
                        channel,
                        ChannelBase(open62541Cpp::UA_RelativPathElement(2, "FeedOverride"))
                    ),
                    nodesMaster,
                    this->FeedOverride.Value);

    open62541Cpp::UA_RelativPathBase FeedOverride(ChannelBase(open62541Cpp::UA_RelativPathElement(2, "FeedOverride")));
    bindValueByPath(pServer,
                    open62541Cpp::UA_BrowsePath(
                        channel,
                        FeedOverride(open62541Cpp::UA_RelativPathElement(0, "EURange"))
                    ),
                    nodesMaster,
                    this->FeedOverride.EURange);
  }
};

bool first = true;

void simulate(IdentificationMachine_t *pInfo,
              ChannelMonitoringType_t *pChannel1,
              std::atomic_bool &running,
              std::mutex &accessDataMutex,
              UA_Server *pServer) {
  std::unique_lock<std::remove_reference<decltype(accessDataMutex)>::type> ul(accessDataMutex);
  ul.unlock();
  int i = 0;
  while (running) {
    ++i;
    ul.lock();
    ++(pInfo->BuildYear);
    pChannel1->ChannelState = static_cast<UA_ChannelState>((((int) pChannel1->ChannelState) + 1)
        % (UA_ChannelState::UA_CHANNELSTATE_RESET + 1));

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

    //std::cout << i << std::endl;
    //running = i < 10;
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

  ChannelMonitoringType_t channel1 = {
      .ChannelState = UA_ChannelState::UA_CHANNELSTATE_INTERRUPTED,
      .FeedOverride = {
          .EURange = {
              .low = 123.45,
              .high = 234.56
          }
      }
  };

  identificationMachine.bind(pServer, UA_NODEID_NUMERIC(3, UA_ISWEXAMPLE_ID_MACHINETOOLS_ISWEXAMPLE), n);
  identificationSoftware.bind(pServer, UA_NODEID_NUMERIC(3, UA_ISWEXAMPLE_ID_MACHINETOOLS_ISWEXAMPLE), n);
  channel1.bind(pServer, UA_NODEID_NUMERIC(3, UA_ISWEXAMPLE_ID_MACHINETOOLS_ISWEXAMPLE_MONITORING_CHANNEL1), n);

  n.SetCallbacks();

  std::atomic_bool running{true};

  //UA_Server_run(pServer, &running);
  UA_Server_run_startup(pServer);
  std::unique_lock<decltype(accessDataMutex)> ul(accessDataMutex);
  std::thread t(simulate, &identificationMachine, &channel1, std::ref(running), std::ref(accessDataMutex), pServer);
  ul.unlock();
  while (running) {
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

  return 0;
}