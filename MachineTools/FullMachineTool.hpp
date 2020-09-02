#pragma once

#include "InstantiatedMachineTool.hpp"
#include <memory>
#include "../UmatiServerLib/OpcUaCondition.hpp"
#include "../TypeDefinition/MachineTool/Alert.hpp"
#include "../UmatiServerLib/StateMachineInstance.hpp"

class FullMachineTool : public InstantiatedMachineTool
{
  public:
  FullMachineTool(UA_Server *pServer);

  void Simulate() override;

  protected:
  // Allow skipping initialization in subclasses
  FullMachineTool(UA_Server *pServer, bool initialize);
  void CreateObject() override;

  void InstantiateIdentification();
  void InstantiateMonitoring();
  void InstantiateTools();
  void InstantiatePrognosis();
  void InstantiateProduction();
  int m_simStep = 0;
  std::shared_ptr<OpcUaCondition<machineTool::Alert_t>> m_pAlert;
  UmatiServerLib::StateMachineInstance<machineTool::ProductionStateMachine_t> JobStateMachine;
};
