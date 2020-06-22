#pragma once

#include "InstantiatedMachineTool.hpp"
#include <memory>
#include "../OpcUaCondition.hpp"
#include "../TypeDefinition/MachineTool/Alert.hpp"

class FullMachineTool : public InstantiatedMachineTool
{
  public:
  FullMachineTool(UA_Server *pServer);

  void Simulate() override;

  protected:
  void CreateObject() override;

  void InstantiateIdentification();
  void InstantiateMonitoring();
  void InstantiateTools();
  void InstantiatePrognosis();
  int m_simStep = 0;
  std::shared_ptr<OpcUaCondition<machineTool::Alert_t>> m_pAlert;
};
