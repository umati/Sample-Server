#pragma once
#include "InstantiatedMachineTool.hpp"

class ShowcaseMachineTool : public InstantiatedMachineTool {
 public:
  ShowcaseMachineTool(UA_Server *pServer);

  void Simulate() override;
  virtual ~ShowcaseMachineTool() = default;

 protected:
  void CreateObject() override;

  void InstantiateIdentification();
  void InstantiateMonitoring();
  void InstantiateTools();
  void InstantiateProduction();
  int m_simStep = 0;
};
