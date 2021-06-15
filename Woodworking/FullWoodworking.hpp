#pragma once
#include "InstantiatedWoodWorking.hpp"

class FullWoodworking : public InstantiatedWoodworking {
 public:
  FullWoodworking(UA_Server *pServer);

  void Simulate() override;
  virtual ~FullWoodworking() = default;

 protected:
  void CreateObject() override;

  void InstantiateIdentification();
  int m_simStep = 0;
};
