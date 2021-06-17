#include "FullWoodworking.hpp"
#include <open62541/types_generated.h>
#include "../UmatiServerLib/BindRefl.hpp"
#include "../UmatiServerLib/OpcUaCondition.hpp"
#include "../UmatiServerLib/ServerHelper.hpp"

FullWoodworking::FullWoodworking(UA_Server *pServer) : InstantiatedWoodworking(pServer) {
  MachineName = "FullWoodworking";
  CreateObject();
}

void FullWoodworking::CreateObject() {
  InstantiatedWoodworking::CreateObject();
  InstantiateIdentification();
  InstantiateMachineState();
}

void FullWoodworking::InstantiateIdentification() {
    InstantiatedWoodworking::InstantiateIdentification();

    InstantiateOptional(ww.Identification->LocationPlant, m_pServer, n);
    InstantiateOptional(ww.Identification->LocationGPS, m_pServer, n);
    InstantiateOptional(ww.Identification->CustomerCompanyName, m_pServer, n);
    InstantiateOptional(ww.Identification->ManufacturerUri, m_pServer, n);
    InstantiateOptional(ww.Identification->ProductCode, m_pServer, n);
    InstantiateOptional(ww.Identification->HardwareRevision, m_pServer, n);
    InstantiateOptional(ww.Identification->SoftwareRevision, m_pServer, n);
    InstantiateOptional(ww.Identification->MonthOfConstruction, m_pServer, n);
    InstantiateOptional(ww.Identification->InitialOperationDate, m_pServer, n);
    InstantiateOptional(ww.Identification->AssetId, m_pServer, n);
    InstantiateOptional(ww.Identification->ComponentName, m_pServer, n);
    InstantiateOptional(ww.Identification->Location, m_pServer, n);

    ww.Identification->LocationPlant = "TBB";
    ww.Identification->LocationGPS = "158.3259 123.1525";
    ww.Identification->CustomerCompanyName = {"","Weinig Expo Center"};
    ww.Identification->ManufacturerUri = "https://www.weinig.com";
    ww.Identification->ProductCode = "123456";
    ww.Identification->HardwareRevision = "1.0";
    ww.Identification->SoftwareRevision = "1.1";
    ww.Identification->MonthOfConstruction = 1;
    ww.Identification->InitialOperationDate = std::chrono::system_clock::now();
    ww.Identification->AssetId = "123456";
    ww.Identification->ComponentName = {"","Machine"};
    ww.Identification->Location = "TBB";
}

void FullWoodworking::InstantiateMachineState() {
  ww.State->Machine->Overview->CurrentState = UA_WwUnitStateEnumeration::UA_WWUNITSTATEENUMERATION_READY;
  ww.State->Machine->Overview->CurrentMode = UA_WwUnitModeEnumeration::UA_WWUNITMODEENUMERATION_SEMIAUTOMATIC;
}

void FullWoodworking::Simulate() {
    ++m_simStep;
}
