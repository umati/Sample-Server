#include "BasicMachineTool.hpp"

BasicMachineTool::BasicMachineTool(UA_Server *pServer)
    : InstantiatedMachineTool(pServer)
{
  MachineName = "BasicMachineTool";
  CreateObject();
}

void BasicMachineTool::CreateObject()
{
  mt.Production->ActiveProgram->NumberInList = 0;
  mt.Production->ActiveProgram->Name = "Basic Program";

  InstantiatedMachineTool::CreateObject();
  InstantiateIdentification();
  InstantiateMonitoring();
  InstantiateTools();
}

void BasicMachineTool::InstantiateMonitoring()
{
  InstantiateMonitoringMT();
  InstantiateMonitoringStacklight(
      {UA_SignalColor::UA_SIGNALCOLOR_RED,
       UA_SignalColor::UA_SIGNALCOLOR_YELLOW,
       UA_SignalColor::UA_SIGNALCOLOR_GREEN});

  InstantiateMonitoringChannel(1);

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
  for (size_t i = 1; i <= 1; ++i)
  {
    std::stringstream ss;
    ss << "Tool" << i;
    auto &tool = mt.Equipment->Tools->Tool.Add<machineTool::Tool_t>(m_pServer, n, {m_nsIndex, ss.str()});
    tool.ControlIdentifier1 = i * 10 + 2;
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
  if ((m_simStep % 2) == 1)
  {
    SimulateStacklight();
  }
}