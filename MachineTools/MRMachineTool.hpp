#pragma once

#include "InstantiatedMachineTool.hpp"
#include <memory>
#include "../UmatiServerLib/OpcUaCondition.hpp"
#include "../TypeDefinition/MachineTool/Alert.hpp"
#include "../UmatiServerLib/StateMachineInstance.hpp"

class MRMachineTool : public InstantiatedMachineTool
{
  public:
  MRMachineTool(UA_Server *pServer);

  void Simulate() override;

  protected:
  void CreateObject() override;

  void InstantiateIdentification();
  void InstantiateMonitoring();
  void InstantiateProduction();
  int m_simStep = 0;
  int isOnFor;
  void SwitchOnStacklightColor(UA_SignalColor color);
  std::shared_ptr<OpcUaCondition<machineTool::Alert_t>> m_pAlert;
  std::shared_ptr<UmatiServerLib::StateMachineInstance<machineTool::ProductionStateMachine_t>> ActiveProgramStateMachine;
  std::shared_ptr<UmatiServerLib::StateMachineInstance<machineTool::ProductionStateMachine_t>> VdJobStateMachine;
  std::shared_ptr<UmatiServerLib::StateMachineInstance<machineTool::ProductionStateMachine_t>> VdJobProgramStateMachine;
};
