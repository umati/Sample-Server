#include "BasicWoodworking.hpp"

BasicWoodworking::BasicWoodworking(UA_Server *pServer) : InstantiatedWoodworking(pServer) {
  MachineName = "BasicWoodworking";
  CreateObject();
}

void BasicWoodworking::CreateObject() {
  InstantiatedWoodworking::CreateObject();
  InstantiateIdentification();
  InstantiateMachineFlags();
  InstantiateMachineOverview();
}

void BasicWoodworking::InstantiateIdentification() { InstantiatedWoodworking::InstantiateIdentification(); }

void BasicWoodworking::InstantiateMachineFlags() { InstantiatedWoodworking::InstantiateMachineFlags(); }

void BasicWoodworking::InstantiateMachineOverview() { InstantiatedWoodworking::InstantiateMachineOverview(); }

void BasicWoodworking::Simulate() { ++m_simStep; }
