#include "CNShowcaseMachineTool.hpp"

CNShowcaseMachineTool::CNShowcaseMachineTool(UA_Server *pServer)
    : InstantiatedMachineTool(pServer)
{
  MachineName = "CNShowcaseMachineTool";
  CreateObject();
}

void CNShowcaseMachineTool::CreateObject()
{
  InstantiatedMachineTool::CreateObject();
  InstantiateIdentification();
  InstantiateMonitoring();
  InstantiateTools();
  InstantiateProduction();
}

void CNShowcaseMachineTool::InstantiateIdentification()
{
    InstantiatedMachineTool::InstantiateIdentification();
    InstantiateOptional(mt.Identification->YearOfConstruction, m_pServer, n);
    InstantiateOptional(mt.Identification->ProductCode, m_pServer, n);
    InstantiateOptional(mt.Identification->SoftwareRevision, m_pServer, n);
    InstantiateOptional(mt.Identification->DeviceClass, m_pServer, n);
    InstantiateOptional(mt.Identification->Location, m_pServer, n);
    InstantiateOptional(mt.Identification->Model, m_pServer, n);  

    mt.Identification->Manufacturer = {"", "umati China Showcase"};
    mt.Identification->ProductCode = "2653837gg1548";
    mt.Identification->YearOfConstruction = 2021;
    mt.Identification->SoftwareRevision= "v1.02.3";
    mt.Identification->DeviceClass = "Machining centre";
    mt.Identification->ProductInstanceUri = "http://amtc.tech";
    mt.Identification->Location = "METAV 2 8-7/VIRTUAL 0 0/N 31.282196 E 121.212046";
    mt.Identification->Model = {"", MachineName}; 
}

void CNShowcaseMachineTool::InstantiateMonitoring()
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
  spindle.Override->Value = 98.0;
  spindle.Override->EURange->high = 125.0;
  spindle.Override->EURange->low = 0.0;
  spindle.Override->EngineeringUnits->DisplayName = {"", "%"};
  spindle.IsRotating = true;
  spindle.Name = "Spindle 1";
  spindle.IsUsedAsAxis = false;
}

void CNShowcaseMachineTool::InstantiateTools()
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
    tool.Name = ss.str();
    InstantiateOptional(tool.Name, m_pServer, n);
  }
}

void CNShowcaseMachineTool::InstantiateProduction()
{
  InstantiateOptional(mt.Production->ActiveProgram->State, m_pServer, n);
  mt.Production->ActiveProgram->NumberInList = 0;
  mt.Production->ActiveProgram->Name = "Basic Program";
  mt.Production->ActiveProgram->State->CurrentState->Value = {"en","Running"};
  mt.Production->ActiveProgram->State->CurrentState->Number = 1;
  mt.Production->ActiveProgram->State->CurrentState->Id = UA_NODEID_NUMERIC(nsFromUri(m_pServer, constants::NsMachineToolUri), UA_MACHINETOOLID_PRODUCTIONSTATEMACHINETYPE_RUNNING);
}


void CNShowcaseMachineTool::Simulate()
{
  ++m_simStep;
  if ((m_simStep % 2) == 1)
  {
    SimulateStacklight();
  }

  mt.Monitoring->MachineTool->PowerOnDuration = m_simStep / 3600;

}
