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
  InstantiateProduction();
}

void BasicMachineTool::InstantiateIdentification()
{
    InstantiatedMachineTool::InstantiateIdentification();
}

void BasicMachineTool::InstantiateMonitoring()
{
  InstantiateMonitoringMT();
  InstantiateMonitoringStacklight(
      {UA_SignalColor::UA_SIGNALCOLOR_RED,
       UA_SignalColor::UA_SIGNALCOLOR_YELLOW,
       UA_SignalColor::UA_SIGNALCOLOR_GREEN});

  InstantiateMonitoringChannel(1);
}

void BasicMachineTool::InstantiateProduction()
{
  InstantiateOptional(mt.Production->ActiveProgram->State, m_pServer, n);
  mt.Production->ActiveProgram->NumberInList = 0;
  mt.Production->ActiveProgram->Name = "Basic Program";
  mt.Production->ActiveProgram->State->CurrentState->Value = {"en","Running"};
  mt.Production->ActiveProgram->State->CurrentState->Number = 1;
  mt.Production->ActiveProgram->State->CurrentState->Id = UA_NODEID_NUMERIC(nsFromUri(m_pServer, constants::NsMachineToolUri), UA_MACHINETOOLID_PRODUCTIONSTATEMACHINETYPE_RUNNING);
}


void BasicMachineTool::Simulate()
{
  ++m_simStep;
  if ((m_simStep % 2) == 1)
  {
    SimulateStacklight();
  }
  
    mt.Monitoring->MachineTool->PowerOnDuration = m_simStep / 3600;
}
