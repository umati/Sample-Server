#include "InstantiatedMachineTool.hpp"
#include <sstream>
#include <unordered_map>

InstantiatedMachineTool::InstantiatedMachineTool(UA_Server *pServer)
    : m_pServer(pServer), NsIndex(m_nsIndex), n(pServer)
{
}

void InstantiatedMachineTool::CreateObject()
{
  std::stringstream ss;
  ss << "http://example.com/" << MachineName << "/";
  m_nsIndex = UA_Server_addNamespace(m_pServer, ss.str().c_str());
  UA_ObjectAttributes objAttr = UA_ObjectAttributes_default;

  objAttr.displayName = UA_LOCALIZEDTEXT_ALLOC("", MachineName.c_str());
  objAttr.eventNotifier = UA_EVENTNOTIFIERTYPE_SUBSCRIBETOEVENTS;

  auto status = UA_Server_addObjectNode(
      m_pServer,
      UA_NODEID_NUMERIC(m_nsIndex, 0),
      UA_NODEID_NUMERIC(nsFromUri(m_pServer, constants::NsMachineryUri), UA_MACHINERYID_MACHINES),
      UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
      *open62541Cpp::UA_QualifiedName(m_nsIndex, MachineName).QualifiedName,
      UA_NODEID_NUMERIC(nsFromUri(m_pServer, constants::NsMachineToolUri), UA_MACHINETOOLID_MACHINETOOLTYPE),
      objAttr,
      nullptr,
      m_nodeId.NodeId);

  UA_ObjectAttributes_clear(&objAttr);
  UmatiServerLib::Bind::MembersRefl(mt, m_pServer, m_nodeId, n);
}

void InstantiatedMachineTool::InstantiateIdentification()
{
  {
    std::stringstream ss;
    ss << "http://www.isw.uni-stuttgart.de/#" << MachineName;
    mt.Identification->ProductInstanceUri = ss.str();
  }

  mt.Identification->Manufacturer = {"", "ISW Christian von Arnim"};

  {
    std::hash<std::string> hasher;
    std::stringstream ss;
    ss << "2021-" << hasher(MachineName);
    mt.Identification->SerialNumber = ss.str();
  }
}

void InstantiatedMachineTool::InstantiateMonitoringStacklight(std::list<UA_SignalColor> stacklightColors)
{
  InstantiateOptional(mt.Monitoring->Stacklight, m_pServer, n);
  InstantiateOptional(mt.Monitoring->Stacklight->NodeVersion, m_pServer, n);
  mt.Monitoring->Stacklight->StacklightMode = UA_StacklightOperationMode::UA_STACKLIGHTOPERATIONMODE_SEGMENTED;

  // Store size, as the list will become shorter
  std::size_t s = stacklightColors.size();
  for (std::size_t i = 0; i < s; ++i)
  {
    std::stringstream ss;
    ss << "Light " << i;
    auto &light = mt.Monitoring->Stacklight->OrderedObjects.Add(m_pServer, n, {m_nsIndex, ss.str()});
    InstantiateOptional(light.IsPartOfBase, m_pServer, n);
    InstantiateOptional(light.SignalOn, m_pServer, n);
    InstantiateOptional(light.StacklightMode, m_pServer, n);

    light.StacklightMode = UA_SignalModeLight::UA_SIGNALMODELIGHT_CONTINUOUS;
    light.IsPartOfBase = false;
    light.NumberInList = i;
    light.SignalOn = true;
    if (!stacklightColors.empty())
    {
      light.SignalColor = stacklightColors.front();
      InstantiateOptional(light.SignalColor, m_pServer, n);
      stacklightColors.pop_front();
    }
  }
}

void InstantiatedMachineTool::InstantiateMonitoringChannel(int numChannels)
{
  for (std::size_t i = 1; i <= numChannels; ++i)
  {
    std::stringstream ss;
    ss << "Channel " << i;
    auto &channel = mt.Monitoring->Channels.Add(m_pServer, n, {m_nsIndex, ss.str()});
    channel.ChannelState = UA_CHANNELSTATE_INTERRUPTED;
    channel.FeedOverride->EngineeringUnits->DisplayName = {"", "%"};
    channel.FeedOverride->EURange->low = 0;
    channel.FeedOverride->EURange->high = (rnd() % 5) * 10 + 100;
    channel.FeedOverride->Value = std::fmod(rnd() / 100.0, channel.FeedOverride->EURange->high);
    channel.Name = ss.str();
    channel.ChannelMode = UA_ChannelMode::UA_CHANNELMODE_AUTOMATIC;
  }
}

void InstantiatedMachineTool::SimulateStacklight()
{
  for (auto &light : mt.Monitoring->Stacklight->OrderedObjects.value)
  {
    light->SignalOn = (rnd() % 2) == 0;
  }
}

void InstantiatedMachineTool::InstantiateMonitoringMT()
{
  mt.Monitoring->MachineTool->OperationMode = UA_MachineOperationMode::UA_MACHINEOPERATIONMODE_AUTOMATIC;
  InstantiateOptional(mt.Monitoring->MachineTool->PowerOnDuration, m_pServer, n);
  mt.Monitoring->MachineTool->PowerOnDuration = 10;
}
