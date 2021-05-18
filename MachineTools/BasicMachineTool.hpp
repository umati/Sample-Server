#pragma once
#include "InstantiatedMachineTool.hpp"

class BasicMachineTool : public InstantiatedMachineTool {
 public:
  BasicMachineTool(UA_Server *pServer);

  void Simulate() override;
  virtual ~BasicMachineTool() = default;

 protected:
  void CreateObject() override;

  void InstantiateIdentification();
  void InstantiateMonitoring();
  void InstantiateProduction();
  int m_simStep = 0;
};
