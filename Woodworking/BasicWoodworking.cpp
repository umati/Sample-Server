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

void BasicWoodworking::InstantiateIdentification() {
    InstantiatedWoodworking::InstantiateIdentification();
}

void BasicWoodworking::InstantiateMachineFlags() {
    InstantiateOptional(ww.State->Machine->Flags,m_pServer,n);
    InstantiatedWoodworking::InstantiateMachineFlags();
}

void BasicWoodworking::InstantiateMachineOverview() {
    InstantiatedWoodworking::InstantiateMachineOverview();
}

void BasicWoodworking::Simulate() {
    ++m_simStep;
}
