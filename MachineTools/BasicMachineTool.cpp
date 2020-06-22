#include "BasicMachineTool.hpp"

BasicMachineTool::BasicMachineTool(UA_Server *pServer)
    : InstantiatedMachineTool(pServer)
{
  MachineName = "BasicMachineTool";
  CreateObject();
}

void BasicMachineTool::CreateObject()
{
  InstantiatedMachineTool::CreateObject();
  InstantiateIdentification();
  InstantiateMonitoring();
  InstantiateTools();
}

void BasicMachineTool::InstantiateMonitoring()
{
  InstantiateOptional(mt.Monitoring->Stacklight, m_pServer, n);
  InstantiateOptional(mt.Monitoring->Stacklight->NodeVersion, m_pServer, n);
  std::list<UA_SignalColor> colors{
      UA_SignalColor::UA_SIGNALCOLOR_RED,
      UA_SignalColor::UA_SIGNALCOLOR_YELLOW,
      UA_SignalColor::UA_SIGNALCOLOR_GREEN};

  for (size_t i = 1; i <= 3; ++i)
  {
    std::stringstream ss;
    ss << "Light " << i;
    auto &light = mt.Monitoring->Stacklight->OrderedObjects.Add(m_pServer, n, {m_nsIndex, ss.str()});
    InstantiateOptional(light.IsPartOfBase, m_pServer, n);
    InstantiateOptional(light.SignalOn, m_pServer, n);

    light.IsPartOfBase = false;
    light.NumberInList = i;
    light.SignalOn = true;
    if (!colors.empty())
    {
      light.SignalColor = colors.front();
      InstantiateOptional(light.SignalColor, m_pServer, n);
      colors.pop_front();
    }
  }

  auto &channel1 = mt.Monitoring->Channels.Add(m_pServer, n, {m_nsIndex, "Channel1"});
  channel1.ChannelState = UA_CHANNELSTATE_INTERRUPTED;
  channel1.FeedOverride->Value = 89.0;
  channel1.FeedOverride->EngineeringUnits->DisplayName = {"", "%"};

  auto &spindle = mt.Monitoring->MonitoredElement.Add<machineTool::SpindleMonitoring_t>(m_pServer, n, {m_nsIndex, "Spindle"});
  InstantiateOptional(spindle.Override, m_pServer, n);
  InstantiateOptional(spindle.IsUsedAsAxis, m_pServer, n);
  spindle.Override->Value = 103.0;
  spindle.Override->EURange->high = 125.0;
  spindle.Override->EURange->low = 0.0;
  spindle.Override->EngineeringUnits->DisplayName = {"", "%"};
  spindle.IsRotating = true;
  spindle.Name = "Spindle";
  spindle.IsUsedAsAxis = false;
}

void BasicMachineTool::InstantiateTools()
{
    InstantiateOptional(mt.Equipment->Tools, m_pServer, n);
    InstantiateOptional(mt.Equipment->Tools->NodeVersion, m_pServer, n);
    n.Remove(mt.Equipment->Tools->NodeVersion.NodeId);
    for(size_t i = 1; i <= 1; ++i)
    {
      std::stringstream ss;
      ss << "Tool" << i;
      auto &tool = mt.Equipment->Tools->Tool.Add<machineTool::Tool_t>(m_pServer, n, {m_nsIndex, ss.str()});
      tool.ControlIdentifier1 = i *10 + 2;
      tool.ControlIdentifierInterpretation = UA_ToolManagement::UA_TOOLMANAGEMENT_NUMBERBASED;
      tool.Locked->Value = false;
      tool.Locked->ReasonForLocking = UA_ToolLocked::UA_TOOLLOCKED_OTHER;
      tool.Name = {"", ss.str()};
      InstantiateOptional(tool.Name, m_pServer, n);
    }
}

void BasicMachineTool::Simulate()
{
  ++m_simStep;
  if((m_simStep % 2) == 1)
  {
    for(auto &light : mt.Monitoring->Stacklight->OrderedObjects.value)
    {
      light->SignalOn = (rnd() % 2) == 0;
    }
  }
}