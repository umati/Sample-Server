#include "InstantiatedMachineTool.hpp"
#include <sstream>
#include <unordered_map>

InstantiatedMachineTool::InstantiatedMachineTool(UA_Server *pServer)
    : m_pServer(pServer), NsIndex(m_nsIndex), n(pServer)
{
}

void InstantiatedMachineTool::CreateObject()
{
  std::stringstream ss;
  ss << "http://example.com/" << MachineName << "/";
  m_nsIndex = UA_Server_addNamespace(m_pServer, ss.str().c_str());
  UA_ObjectAttributes objAttr = UA_ObjectAttributes_default;

  objAttr.displayName = UA_LOCALIZEDTEXT_ALLOC("", MachineName.c_str());
  objAttr.eventNotifier = UA_EVENTNOTIFIERTYPE_SUBSCRIBETOEVENTS;

  auto status = UA_Server_addObjectNode(
      m_pServer,
      UA_NODEID_NUMERIC(m_nsIndex, 0),
      UA_NODEID_NUMERIC(nsFromUri(m_pServer, constants::NsMachineryUri), UA_MACHINERY_ID_MACHINES),
      UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
      *open62541Cpp::UA_QualifiedName(m_nsIndex, MachineName).QualifiedName,
      UA_NODEID_NUMERIC(nsFromUri(m_pServer, constants::NsMachineToolUri), UA_MACHINETOOLID_MACHINETOOLTYPE),
      objAttr,
      nullptr,
      m_nodeId.NodeId);

  UA_ObjectAttributes_clear(&objAttr);
  UmatiServerLib::Bind::MembersRefl(mt, m_pServer, m_nodeId, n);
}

void InstantiatedMachineTool::InstantiateIdentification()
{
  {
    std::stringstream ss;
    ss << "http://isw.uni-stuttgart.de/#" << MachineName;
    mt.Identification->ProductInstanceUri = ss.str();
  }
  InstantiateOptional(mt.Identification->Location, m_pServer, n);
  InstantiateOptional(mt.Identification->Model, m_pServer, n);

  mt.Identification->Manufacturer = {"", "ISW Christian von Arnim"};
  mt.Identification->Location = "AMB 0 0/N 48.781340 E 9.165731";
  mt.Identification->Model = {"", MachineName};

  {
    std::hash<std::string> hasher;
    std::stringstream ss;
    ss << "2020-" << hasher(MachineName);
    mt.Identification->SerialNumber = ss.str();
  }
}
