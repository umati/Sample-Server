#pragma once
#include "InstantiatedMachineTool.hpp"

class CNShowcaseMachineTool : public InstantiatedMachineTool
{
  public:
  CNShowcaseMachineTool(UA_Server *pServer);

  void Simulate() override;
  virtual ~CNShowcaseMachineTool() = default;
  protected:
  void CreateObject() override;

  void InstantiateIdentification();
  void InstantiateMonitoring();
  void InstantiateTools();
  void InstantiateProduction();
  int m_simStep = 0;
};
