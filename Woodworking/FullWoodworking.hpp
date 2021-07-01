#pragma once
#include "InstantiatedWoodWorking.hpp"

class FullWoodworking : public InstantiatedWoodworking {
 public:
  FullWoodworking(UA_Server *pServer);

  void Simulate() override;
  virtual ~FullWoodworking() = default;

 protected:
  void CreateObject() override;

  void InstantiateIdentification() override;
  void InstantiateMachineFlags() override;
  void InstantiateMachineOverview() override;
  void InstantiateMachineValues();
  void InstantiateEventsDispatcher();
  void InstantiateManufacturerSpecific();
  void InstantiateSubunits();

  int m_simStep = 0;
};
