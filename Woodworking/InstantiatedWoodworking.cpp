#include "InstantiatedWoodworking.hpp"

#include <sstream>
#include <unordered_map>

InstantiatedWoodworking::InstantiatedWoodworking(UA_Server *pServer) : m_pServer(pServer), NsIndex(m_nsIndex), n(pServer) {}

void InstantiatedWoodworking::CreateObject() {
  std::stringstream ss;
  ss << "http://example.com/" << MachineName << "/";
  m_nsIndex = UA_Server_addNamespace(m_pServer, ss.str().c_str());
  UA_ObjectAttributes objAttr = UA_ObjectAttributes_default;

  objAttr.displayName = UA_LOCALIZEDTEXT_ALLOC("", MachineName.c_str());
  objAttr.eventNotifier = UA_EVENTNOTIFIERTYPE_SUBSCRIBETOEVENTS;

  auto status = UA_Server_addObjectNode(
    m_pServer,
    UA_NODEID_NUMERIC(m_nsIndex, 0),
    UA_NODEID_NUMERIC(nsFromUri(m_pServer, constants::NsMachineryUri), UA_MACHINERYID_MACHINES),
    UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
    *open62541Cpp::UA_QualifiedName(m_nsIndex, MachineName).QualifiedName,
    UA_NODEID_NUMERIC(nsFromUri(m_pServer, constants::NsWoodworkingUri), UA_WOODWORKINGID_WWMACHINETYPE),
    objAttr,
    nullptr,
    m_nodeId.NodeId);

  UA_ObjectAttributes_clear(&objAttr);
  UmatiServerLib::Bind::MembersRefl(ww, m_pServer, m_nodeId, n);
}

void InstantiatedWoodworking::InstantiateIdentification() {
  std::stringstream ss;
  ss << "https://www.weinig.com/#" << MachineName;
  ww.Identification->ProductInstanceUri = ss.str();
  ww.Identification->YearOfConstruction = 2021;
  ww.Identification->DeviceClass = "Moulder";
  ww.Identification->Model = {"","Powermat 4000"};
  ww.Identification->Manufacturer = {"", "Weinig"};
  ww.Identification->SerialNumber = "123456";
}

void InstantiatedWoodworking::InstantiateMachineFlags() {
    ww.State->Machine->Flags->MachineOn = true;
    ww.State->Machine->Flags->MachineInitialized = true;
    ww.State->Machine->Flags->PowerPresent = true;
    ww.State->Machine->Flags->Emergency = false;
    ww.State->Machine->Flags->Calibrated = true;
    ww.State->Machine->Flags->Error = false;
    ww.State->Machine->Flags->Alarm = false;
    ww.State->Machine->Flags->Warning = false;
    ww.State->Machine->Flags->RecipeInRun = true;
}

void InstantiatedWoodworking::InstantiateMachineOverview() {
    ww.State->Machine->Overview->CurrentMode = UA_WwUnitModeEnumeration::UA_WWUNITMODEENUMERATION_MANUAL;
    ww.State->Machine->Overview->CurrentState = UA_WwUnitStateEnumeration::UA_WWUNITSTATEENUMERATION_READY;
}
