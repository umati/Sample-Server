#pragma once
#include "InstantiatedWoodworking.hpp"

class BasicWoodworking : public InstantiatedWoodworking {
 public:
  BasicWoodworking(UA_Server *pServer);

  void Simulate() override;
  virtual ~BasicWoodworking() = default;

 protected:
  void CreateObject() override;

  void InstantiateIdentification() override;
  void InstantiateMachineFlags() override;
  void InstantiateMachineOverview() override;

  int m_simStep = 0;
};
