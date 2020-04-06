#include "BindRefl.hpp"
#include <iostream>
#include <open62541/server_config_default.h>
#include "src_generated/namespace_iswexample_generated.h"
#include "src_generated/namespace_machinetool_generated.h"
#include "src_generated/namespace_machinery_generated.h"
#include "src_generated/namespace_di_generated.h"
#include "src_generated/iswexample.h"
#include "src_generated/machinetool_nodeids.h"
#include "src_generated/di_nodeids.h"
#include "src_generated/machinery.h"
#include <cstdint>
#include <functional>
#include "BindableMember.hpp"
#include "NodeValue.hpp"
#include "NodesMaster.hpp"
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
#include "OpcUaTypes/LocalizedText.hpp"
#include "OpcUaTypes/EUInformation.hpp"
#include "OpcUaTypes/DateTime.hpp"

#include "OpcUaTypes/ConstNodeId.hpp"
#include "OpcUaEvent.hpp"

namespace constants
{
constexpr const char *Ns0Uri = "http://opcfoundation.org/UA/";
constexpr const char *NsDIUri = "http://opcfoundation.org/UA/DI/";
constexpr const char *NsMachineryUri = "http://opcfoundation.org/UA/Machinery/";
constexpr const char *NsMachineToolUri = "http://opcfoundation.org/UA/MachineTool/";
constexpr const char *NsInstanceUri = "http://isw.example.com";
} // namespace constants

struct IVendorNameplate_t
{
  BindableMember<std::string> ProductInstanceUri;
};
REFL_TYPE(IVendorNameplate_t,
          open62541Cpp::attribute::UaObjectType{.NodeId = open62541Cpp::constexp::NodeId(constants::NsDIUri, UA_DIID_IVENDORNAMEPLATETYPE)})
REFL_FIELD(ProductInstanceUri);
REFL_END

struct IMachineVendorNameplate_t : public IVendorNameplate_t
{
};
REFL_TYPE(IMachineVendorNameplate_t,
          Bases<IVendorNameplate_t>(),
          open62541Cpp::attribute::UaObjectType{.NodeId = open62541Cpp::constexp::NodeId(constants::NsMachineryUri, UA_MACHINERY_ID_IMACHINEVENDORNAMEPLATETYPE)})
REFL_END

struct MachineToolIdentification_t : public IMachineVendorNameplate_t
{
  BindableMember<std::uint16_t> YearOfConstruction;
  BindableMember<open62541Cpp::LocalizedText_t> Model;
  BindableMember<std::string> SoftwareRevision;
  BindableMember<open62541Cpp::LocalizedText_t> Manufacturer;
  BindableMember<std::string> SerialNumber;
};

REFL_TYPE(MachineToolIdentification_t,
          Bases<IMachineVendorNameplate_t>(),
          open62541Cpp::attribute::UaObjectType{.NodeId = open62541Cpp::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_MACHINETOOLIDENTIFICATIONTYPE)})
REFL_FIELD(YearOfConstruction, open62541Cpp::attribute::UaBrowseName{.NsURI = constants::NsMachineryUri})
REFL_FIELD(Model, open62541Cpp::attribute::UaBrowseName{.NsURI = constants::NsDIUri})
REFL_FIELD(SoftwareRevision, open62541Cpp::attribute::UaBrowseName{.NsURI = constants::NsDIUri})
REFL_FIELD(Manufacturer, open62541Cpp::attribute::UaBrowseName{.NsURI = constants::NsDIUri})
REFL_FIELD(SerialNumber, open62541Cpp::attribute::UaBrowseName{.NsURI = constants::NsDIUri})
REFL_END

struct SoftwareIdentification_t
{
  BindableMember<std::string> SoftwareRevision;
  BindableMember<std::string> Identifier;
};

REFL_TYPE(SoftwareIdentification_t, open62541Cpp::attribute::UaObjectType())
REFL_FIELD(SoftwareRevision)
REFL_FIELD(Identifier)
REFL_END

template <typename T>
struct AnalogUnitRangeType_t
{
  BindableMember<T> Value;
  BindableMember<UA_Range> EURange;
  BindableMember<open62541Cpp::EUInformation_t> EngineeringUnits;
};

REFL_TEMPLATE((typename T), (AnalogUnitRangeType_t<T>), open62541Cpp::attribute::UaVariableType{.NodeId = open62541Cpp::constexp::NodeId(constants::Ns0Uri, UA_NS0ID_ANALOGUNITRANGETYPE)})
REFL_FIELD(Value, open62541Cpp::attribute::UaVariableTypeValue())
REFL_FIELD(EURange)
REFL_FIELD(EngineeringUnits)
REFL_END

struct ChannelMonitoring_t
{
  BindableMember<UA_ChannelState> ChannelState;
  AnalogUnitRangeType_t<double> FeedOverride;
};

REFL_TYPE(ChannelMonitoring_t, open62541Cpp::attribute::UaObjectType{.NodeId = open62541Cpp::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_CHANNELMONITORINGTYPE)})
REFL_FIELD(ChannelState)
REFL_FIELD(FeedOverride)
REFL_END

struct FiniteStateVariableType_t
{
  BindableMember<open62541Cpp::LocalizedText_t> Value;
  BindableMember<UA_UInt32> Number;
};
REFL_TYPE(FiniteStateVariableType_t, open62541Cpp::attribute::UaVariableType())
REFL_FIELD(Value, open62541Cpp::attribute::UaVariableTypeValue())
REFL_FIELD(Number, open62541Cpp::attribute::UaBrowseName{.NsURI = constants::Ns0Uri})
REFL_END

struct State_t
{
  FiniteStateVariableType_t CurrentState;
};

REFL_TYPE(State_t, open62541Cpp::attribute::UaObjectType{.NodeId = open62541Cpp::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PRODUCTIONJOBSTATEMACHINETYPE)})
REFL_FIELD(CurrentState, open62541Cpp::attribute::UaBrowseName{.NsURI = constants::Ns0Uri})
REFL_END

struct BaseEventType_t
{
  open62541Cpp::DateTime_t Time = std::chrono::system_clock::now();
  std::string SourceName;
  std::uint16_t Severity;
  open62541Cpp::LocalizedText_t Message;
  void bind(UA_Server *pServer, UA_NodeId event, NodesMaster &nodesMaster);
};

REFL_TYPE(BaseEventType_t, open62541Cpp::attribute::UaObjectType{.NodeId = open62541Cpp::constexp::NodeId(constants::Ns0Uri, UA_NS0ID_BASEEVENTTYPE)})
REFL_FIELD(Time, open62541Cpp::attribute::UaBrowseName{.NsURI = constants::Ns0Uri})
REFL_FIELD(SourceName, open62541Cpp::attribute::UaBrowseName{.NsURI = constants::Ns0Uri})
REFL_FIELD(Severity, open62541Cpp::attribute::UaBrowseName{.NsURI = constants::Ns0Uri})
REFL_FIELD(Message, open62541Cpp::attribute::UaBrowseName{.NsURI = constants::Ns0Uri})
REFL_END

void BaseEventType_t::bind(UA_Server *pServer, UA_NodeId event, NodesMaster &nodesMaster)
{
  bindMembersRefl(*this, pServer, event, {}, nodesMaster);
}

struct ProductionJob_t
{
  BindableMember<std::string> Identifier;
  BindableMember<std::uint32_t> RunsCompleted;
  BindableMember<std::uint32_t> RunsPlanned;
  State_t State;
};

REFL_TYPE(ProductionJob_t, open62541Cpp::attribute::UaObjectType{.NodeId = open62541Cpp::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PRODUCTIONJOBTYPE)})
REFL_FIELD(Identifier)
REFL_FIELD(RunsCompleted)
REFL_FIELD(RunsPlanned)
REFL_FIELD(State)
REFL_END

struct ProdictionJobList_t
{
  std::list<ProductionJob_t> Jobs;
};

REFL_TYPE(ProdictionJobList_t, open62541Cpp::attribute::UaObjectType{.NodeId = open62541Cpp::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PRODUCTIONJOBLISTTYPE)})
REFL_FIELD(Jobs, open62541Cpp::attribute::UaReference{
                     {.NodeId = open62541Cpp::constexp::NodeId(constants::Ns0Uri, UA_NS0ID_HASCOMPONENT)}})
REFL_END

struct Monitoring_t
{
  std::list<ChannelMonitoring_t> Channels;
};

REFL_TYPE(Monitoring_t, open62541Cpp::attribute::UaObjectType{.NodeId = open62541Cpp::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_MONITORINGTYPE)})
REFL_FIELD(Channels, open62541Cpp::attribute::UaReference{
                         {.NodeId = open62541Cpp::constexp::NodeId(constants::Ns0Uri, UA_NS0ID_HASCOMPONENT)}})
REFL_END

struct Production_t
{
  ProdictionJobList_t ProductionPlan;
};

REFL_TYPE(Production_t, open62541Cpp::attribute::UaObjectType{.NodeId = open62541Cpp::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_PRODUCTIONTYPE)})
REFL_FIELD(ProductionPlan)
REFL_END

struct MachineTool_t
{
  MachineToolIdentification_t Identification;
  Monitoring_t Monitoring;
  Production_t Production;
};

REFL_TYPE(MachineTool_t, open62541Cpp::attribute::UaObjectType{.NodeId = open62541Cpp::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_MACHINETOOLTYPE)})
REFL_FIELD(Identification, open62541Cpp::attribute::UaBrowseName{.NsURI = constants::NsDIUri})
REFL_FIELD(Monitoring)
REFL_FIELD(Production)
REFL_END

bool first = true;

void simulate(MachineTool_t *pMachineTool,
              std::atomic_bool &running,
              std::mutex &accessDataMutex,
              UA_Server *pServer)
{
  std::unique_lock<std::remove_reference<decltype(accessDataMutex)>::type> ul(accessDataMutex);
  ul.unlock();
  int i = 0;
  while (running)
  {
    ++i;
    ul.lock();
    ++(pMachineTool->Identification.YearOfConstruction.value);
    //pChannel1->ChannelState = static_cast<UA_ChannelState>((((int)pChannel1->ChannelState) + 1) % (UA_ChannelState::UA_CHANNELSTATE_RESET + 1));

    {
      BaseEventType_t baseEvent{
          .Time = std::chrono::system_clock::now(),
          .SourceName = "Reflection Event",
          .Severity = 258,
          .Message = {
              .locale = "",
              .text = "MyMessage"},
      };
      OpcUaEvent ev(baseEvent, pServer);
    }
    NotificationEvent_t ev{.Identifier = "MyId", .Message = "MessageTxt", .SourceName = "MySource", .Severity = 500};
    /*
    auto evNodeId = setupNotificationEvent(pServer, ev);
    auto retval = UA_Server_triggerEvent(pServer, evNodeId, UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER), NULL, UA_TRUE);
    */
    if (first)
    {
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

int main(int argc, char *argv[])
{
  UA_Server *pServer = UA_Server_new();
  UA_ServerConfig_setDefault(UA_Server_getConfig(pServer));

  std::cout << "ExampleUmatiServer" << std::endl;
  namespace_di_generated(pServer);
  namespace_machinery_generated(pServer);
  namespace_machinetool_generated(pServer);
  namespace_iswexample_generated(pServer);

  std::mutex accessDataMutex;
  NodesMaster n(pServer);
  MachineTool_t machineTool;

  machineTool.Identification.YearOfConstruction = 2020;
  machineTool.Identification.Model = open62541Cpp::LocalizedText_t{.locale = "", .text = "ISW Example"};
  machineTool.Identification.SoftwareRevision = std::string{"master"};
  machineTool.Identification.Manufacturer = open62541Cpp::LocalizedText_t{.locale = "", .text = "ISW Christian von Arnim"};
  machineTool.Identification.SerialNumber = std::string{"3-1415926535-8979323846"};

  machineTool.Identification.ProductInstanceUri.value = "Prototype 01";
  machineTool.Identification.ProductInstanceUri.StatusCode = UA_STATUSCODE_GOODEDITED;
  machineTool.Identification.ProductInstanceUri.SourceTimestamp = UA_DateTime_fromStruct(UA_DateTimeStruct{.sec = 13, .min = 12, .hour = 11, .day = 10, .month = 9, .year = 2008});

  bindMembersRefl(machineTool, pServer, UA_NODEID_NUMERIC(5, UA_ISWEXAMPLE_ID_MACHINES_ISWEXAMPLEMACHINE), {}, n);

  // Assign placeholders after binding!
  {
    int i = 1;
    for (auto &job : machineTool.Production.ProductionPlan.Jobs)
    {
      std::stringstream ss;
      ss << "ID_" << i++ << std::endl;
      job.Identifier = ss.str();
      job.RunsCompleted = 8;
      job.RunsPlanned = 10;
      job.State.CurrentState.Value = open62541Cpp::LocalizedText_t{.locale = "en-en", .text = "Testing State"};
      job.State.CurrentState.Number = 1234;
    }

    for (auto &channel : machineTool.Monitoring.Channels)
    {
      channel.ChannelState = UA_ChannelState::UA_CHANNELSTATE_INTERRUPTED;
      channel.FeedOverride.Value = 6;
      *channel.FeedOverride.EURange = {
          .low = 123.45,
          .high = 234.56};
      *channel.FeedOverride.EngineeringUnits = {
          .NamespaceUri = "eu://meter",
          .UnitId = -1,
          .DisplayName{.locale = "", .text = "Meter"},
          .Description = {.locale = "en", .text = "100cm"},
      };
    }
  }

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

  return 0;
}