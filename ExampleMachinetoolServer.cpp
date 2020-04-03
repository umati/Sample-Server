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
#include "Variable.hpp"
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
#include "OpcUaTypes/LocalizedText.hpp"
#include "OpcUaTypes/EUInformation.hpp"
#include "OpcUaTypes/DateTime.hpp"

#include <refl.hpp>
#include "OpcUaTypes/ConstNodeId.hpp"
#include "OpcUaTypes/Attributes.hpp"
#include "OpcUaEvent.hpp"
#include "Util.hpp"

namespace constants
{
constexpr const char *Ns0Uri = "http://opcfoundation.org/UA/";
constexpr const char *NsDIUri = "http://opcfoundation.org/UA/DI/";
constexpr const char *NsMachineryUri = "http://opcfoundation.org/UA/Machinery/";
constexpr const char *NsMachineToolUri = "http://opcfoundation.org/UA/MachineTool/";
constexpr const char *NsInstanceUri = "http://isw.example.com";
} // namespace constants

template <typename T>
void bindMemberRefl(T &member, UA_Server *pServer, UA_NodeId nodeId, open62541Cpp::UA_RelativPathBase base, NodesMaster &nodesMaster);

template <typename T>
void bindPlaceholder(std::list<T> &member, UA_Server *pServer, UA_NodeId nodeId, open62541Cpp::UA_RelativPathBase base, NodesMaster &nodesMaster, open62541Cpp::UA_NodeId refTypeNodeId);

/**
 * @brief Binding the members by it's reflection description
 *
 * @tparam T Any type with defined reflection
 * @param instance Instance to be bind
 * @param pServer Pointer to OPC UA Server
 * @param nodeId Start reference for binding
 * @param base Base, added to nodeId for all nodes
 * @param nodesMaster NodesMaster instace for resolving the bindings.
 */
template <typename T>
void bindMembersRefl(T &instance, UA_Server *pServer, UA_NodeId nodeId, open62541Cpp::UA_RelativPathBase base, NodesMaster &nodesMaster)
{
  if constexpr (refl::descriptor::has_attribute<Bases>(refl::reflect<T>()))
  {
    constexpr auto bases = refl::descriptor::get_attribute<Bases>(refl::reflect<T>());
    if constexpr (bases.descriptors.size)
    {
      refl::util::for_each(bases.descriptors, [&](auto t) {
        bindMembersRefl(static_cast<typename decltype(t)::type &>(instance), pServer, nodeId, base, nodesMaster);
      });
    }
  }

  for_each(refl::reflect(instance).members, [&](auto member) {
    auto childRelativPathElements = base();

    if constexpr (
        is_same_template<typename decltype(member)::value_type, std::list>::value)
    {
      std::cout << member.name << " is a placeholder." << std::endl;
      std::cout << "Placeholders are not implemented." << std::endl;
      ///\todo Read placeholder instances from Server.

      if constexpr (
          !refl::descriptor::has_attribute<open62541Cpp::attribute::UaReference>(member))
      {
        std::cout << "Placeholder " << member.name << " has no UaReference." << std::endl;
        throw std::runtime_error("Required attribute UaReference not found.");
      }
      const auto &reference = refl::descriptor::get_attribute<open62541Cpp::attribute::UaReference>(member);
      auto refTypeNodeId = reference.NodeId.UANodeId(pServer);

      bindPlaceholder(member(instance), pServer, nodeId, base, nodesMaster, refTypeNodeId);

      return;
    }

    // Check if this is not the value of a variable type
    // If this is the value of a variable type bind it to the base without appending a browse name (skip adding a browse name)
    if constexpr (
        !refl::descriptor::has_attribute<open62541Cpp::attribute::UaVariableTypeValue>(member))
    {
      std::uint16_t nsIndex = 2;
      std::string name(member.name);
      if constexpr (refl::descriptor::has_attribute<open62541Cpp::attribute::UaBrowseName>(member))
      {
        const auto &attrBrowseName = refl::descriptor::get_attribute<open62541Cpp::attribute::UaBrowseName>(member);
        if (attrBrowseName.NsURI != nullptr)
        {
          nsIndex = nsFromUri(pServer, attrBrowseName.NsURI);
        }

        if (attrBrowseName.Name != nullptr)
        {
          name = attrBrowseName.Name;
        }
      }
      else if constexpr (refl::descriptor::has_attribute<open62541Cpp::attribute::UaObjectType>(refl::reflect<T>()))
      {
        // Take index from type as default BrowseName Index
        auto objTypeAttr = refl::descriptor::get_attribute<open62541Cpp::attribute::UaObjectType>(refl::reflect<T>());
        if (objTypeAttr.NodeId.NsUri != nullptr)
        {
          nsIndex = nsFromUri(pServer, objTypeAttr.NodeId.NsUri);
        }
        else
        {
          std::cout << "No NodeId at object type provided!" << std::endl;
        }
      }
      else if constexpr (refl::descriptor::has_attribute<open62541Cpp::attribute::UaVariableType>(refl::reflect<T>()))
      {
        // Take index from type as default BrowseName Index
        auto varTypeAttr = refl::descriptor::get_attribute<open62541Cpp::attribute::UaVariableType>(refl::reflect<T>());
        if (varTypeAttr.NodeId.NsUri != nullptr)
        {
          nsIndex = nsFromUri(pServer, varTypeAttr.NodeId.NsUri);
        }
        else
        {
          std::cout << "No NodeId at variable type provided!" << std::endl;
        }
      }

      childRelativPathElements.push_back(open62541Cpp::UA_RelativPathElement(nsIndex, name));
    }

    bindMemberRefl(member(instance), pServer, nodeId, childRelativPathElements, nodesMaster);
  });
}

template <typename AttributeType, typename T>
constexpr bool hasAttributeIfReflectable(const T &member) noexcept
{
  if constexpr (!refl::trait::is_reflectable<T>::value)
  {
    return false;
  }
  else
  {
    return refl::descriptor::has_attribute<AttributeType>(
        refl::reflect(member));
  }
}

template <typename T>
void bindMemberRefl(T &member, UA_Server *pServer, UA_NodeId nodeId, open62541Cpp::UA_RelativPathBase base, NodesMaster &nodesMaster)
{
  constexpr bool isReflectable = refl::trait::is_reflectable<T>::value;
  if constexpr (
      hasAttributeIfReflectable<open62541Cpp::attribute::UaObjectType>(member) ||
      hasAttributeIfReflectable<open62541Cpp::attribute::UaVariableType>(member))
  {
    bindMembersRefl(member, pServer, nodeId, base, nodesMaster);
  }
  else if constexpr (is_same_template<T, Variable>::value)
  {
    bindVariableByPath(
        pServer,
        open62541Cpp::UA_BrowsePath(
            nodeId,
            base()),
        nodesMaster,
        member);
  }
  else
  {
    bindValueByPath(
        pServer,
        open62541Cpp::UA_BrowsePath(
            nodeId,
            base()),
        nodesMaster,
        member);
  }
}

template <typename T>
void bindPlaceholder(std::list<T> &member, UA_Server *pServer, UA_NodeId baseNodeId, open62541Cpp::UA_RelativPathBase base, NodesMaster &nodesMaster, open62541Cpp::UA_NodeId refTypeNodeId)
{
  auto nodeId = bindValueByPathInternal::resolveBrowsePath(
      pServer, open62541Cpp::UA_BrowsePath(
                   baseNodeId,
                   base()));
  UA_BrowseDescription brDesc;
  UA_BrowseDescription_init(&brDesc);
  UA_NodeId_copy(nodeId.NodeId, &brDesc.nodeId);
  UA_NodeId_copy(refTypeNodeId.NodeId, &brDesc.referenceTypeId);

  open62541Cpp::UA_NodeId typeNodeId;

  if constexpr (!refl::trait::is_reflectable<T>::value)
  {
    static_assert(always_false<T>::value, "T not reflectable");
  }

  if constexpr (refl::descriptor::has_attribute<open62541Cpp::attribute::UaObjectType>(refl::reflect<T>()))
  {
    auto objTypeAttr = refl::descriptor::get_attribute<open62541Cpp::attribute::UaObjectType>(refl::reflect<T>());
    typeNodeId = objTypeAttr.NodeId.UANodeId(pServer);
    brDesc.nodeClassMask = UA_NODECLASS_OBJECT;
  }
  else
  {
    // TODO handle variable types
    static_assert(always_false<T>::value, "T has attribute UaObjectType or (UaVariableType -- not implemented).");
  }

  brDesc.browseDirection = UA_BrowseDirection::UA_BROWSEDIRECTION_FORWARD;
  brDesc.includeSubtypes = UA_TRUE;
  brDesc.resultMask = UA_BROWSERESULTMASK_ALL;

  auto browseResult = UA_Server_browse(pServer, UA_UINT32_MAX, &brDesc);
  if (browseResult.statusCode != UA_STATUSCODE_GOOD)
  {
    std::cout << "Resutl not good: " << UA_StatusCode_name(browseResult.statusCode) << std::endl;
    std::cout << "Could not find placeholders." << std::endl;
  }

  for (std::size_t i = 0; i < browseResult.referencesSize; ++i)
  {
    open62541Cpp::UA_String txt(&browseResult.references[i].displayName.text);

    std::cout << "Placeholder possible value: " << txt << std::endl;
    if (UA_NodeId_equal(&browseResult.references[i].typeDefinition.nodeId, typeNodeId.NodeId))
    {
      std::cout << "Placeholder type match" << std::endl;
      member.push_back(T());
      T &instance = *(member.rbegin());
      bindMembersRefl(instance, pServer, browseResult.references[i].nodeId.nodeId, {}, nodesMaster);
    }
  }

  UA_BrowseResult_deleteMembers(&browseResult);
  UA_BrowseDescription_deleteMembers(&brDesc);
}

struct IVendorNameplate_t
{
  Variable<std::string> ProductInstanceUri;
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
  Variable<std::uint16_t> YearOfConstruction;
  Variable<open62541Cpp::LocalizedText_t> Model;
  Variable<std::string> SoftwareRevision;
  Variable<open62541Cpp::LocalizedText_t> Manufacturer;
  Variable<std::string> SerialNumber;
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
  Variable<std::string> SoftwareRevision;
  Variable<std::string> Identifier;
};

REFL_TYPE(SoftwareIdentification_t, open62541Cpp::attribute::UaObjectType())
REFL_FIELD(SoftwareRevision)
REFL_FIELD(Identifier)
REFL_END

template <typename T>
struct AnalogUnitRangeType_t
{
  Variable<T> Value;
  Variable<UA_Range> EURange;
  Variable<open62541Cpp::EUInformation_t> EngineeringUnits;
};

REFL_TEMPLATE((typename T), (AnalogUnitRangeType_t<T>), open62541Cpp::attribute::UaVariableType{.NodeId = open62541Cpp::constexp::NodeId(constants::Ns0Uri, UA_NS0ID_ANALOGUNITRANGETYPE)})
REFL_FIELD(Value, open62541Cpp::attribute::UaVariableTypeValue())
REFL_FIELD(EURange)
REFL_FIELD(EngineeringUnits)
REFL_END

struct ChannelMonitoring_t
{
  Variable<UA_ChannelState> ChannelState;
  AnalogUnitRangeType_t<double> FeedOverride;
};

REFL_TYPE(ChannelMonitoring_t, open62541Cpp::attribute::UaObjectType{.NodeId = open62541Cpp::constexp::NodeId(constants::NsMachineToolUri, UA_MACHINETOOLID_CHANNELMONITORINGTYPE)})
REFL_FIELD(ChannelState)
REFL_FIELD(FeedOverride)
REFL_END

struct FiniteStateVariableType_t
{
  Variable<open62541Cpp::LocalizedText_t> Value;
  Variable<UA_UInt32> Number;
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
  Variable<std::string> Identifier;
  Variable<std::uint32_t> RunsCompleted;
  Variable<std::uint32_t> RunsPlanned;
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