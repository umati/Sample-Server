#include "InstantiatedRobot.hpp"
#include "../TypeDefinition/Robotics/MotionDevice.hpp"
#include "../TypeDefinition/Machinery/Constants.hpp"
#include <Open62541Cpp/UA_QualifiedName.hpp>
#include "../UmatiServerLib/BindRefl.hpp"

InstantiatedRobot::InstantiatedRobot(UA_Server *pServer)
    : NsIndex(m_nsIndex), n(pServer), m_pServer(pServer)
{
}

InstantiatedRobot::~InstantiatedRobot()
{
}

void InstantiatedRobot::CreateObject()
{
  std::stringstream ss;
  ss << "http://example.com/" << Name << "/";
  m_nsIndex = UA_Server_addNamespace(m_pServer, ss.str().c_str());
  UA_ObjectAttributes objAttr = UA_ObjectAttributes_default;

  objAttr.displayName = UA_LOCALIZEDTEXT_ALLOC("", Name.c_str());
  objAttr.eventNotifier = UA_EVENTNOTIFIERTYPE_SUBSCRIBETOEVENTS;

  auto status = UA_Server_addObjectNode(
      m_pServer,
      UA_NODEID_NUMERIC(m_nsIndex, 0),
      UA_NODEID_NUMERIC(nsFromUri(m_pServer, constants::NsDIUri), UA_DIID_DEVICESET),
      UA_NODEID_NUMERIC(0, UA_NS0ID_ORGANIZES),
      *open62541Cpp::UA_QualifiedName(m_nsIndex, Name).QualifiedName,
      UA_NODEID_NUMERIC(nsFromUri(m_pServer, constants::NsRoboticsUri), UA_ROBOTICSID_MOTIONDEVICETYPE),
      objAttr,
      nullptr,
      m_nodeId.NodeId);

  if (status != UA_STATUSCODE_GOOD)
  {
    throw std::runtime_error("Add object failed.");
  }
  UA_ObjectAttributes_clear(&objAttr);
  UmatiServerLib::Bind::MembersRefl(rb, m_pServer, m_nodeId, n);
}
static UA_ExpandedNodeId ExNodeId(const UA_NodeId &nodeId)
{
  UA_ExpandedNodeId exN;
  exN.namespaceUri = UA_STRING_NULL;
  exN.serverIndex = 0;
  exN.nodeId = nodeId;
  return exN;
}

void InstantiatedRobot::AddIdentificationAddin()
{
  UA_NodeId identification = UA_NODEID_NUMERIC(m_nsIndex, 0);
  UA_ObjectAttributes objAttr = UA_ObjectAttributes_default;
  objAttr.displayName = UA_LOCALIZEDTEXT_ALLOC("", "Identification");
  auto status = UA_Server_addNode_begin(
      m_pServer,
      UA_NodeClass::UA_NODECLASS_OBJECT,
      identification,
      *m_nodeId.NodeId,
      UA_NODEID_NUMERIC(0, UA_NS0ID_HASADDIN),
      *open62541Cpp::UA_QualifiedName(nsFromUri(m_pServer, constants::NsDIUri), Name).QualifiedName,
      UA_NODEID_NUMERIC(nsFromUri(m_pServer, constants::NsMachineryUri), UA_MACHINERY_ID_MACHINEIDENTIFICATIONTYPE),
      &objAttr,
      &UA_TYPES[UA_TYPES_OBJECTATTRIBUTES],
      NULL,
      &identification);
  if (status != UA_STATUSCODE_GOOD)
  {
    throw std::runtime_error("Add UA_Server_addNode_begin in add Identification failed.");
  }

  if (rb.AssetId.IsBind())
  {
    status |= UA_Server_addReference(m_pServer, identification, UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY), ExNodeId(*rb.AssetId.NodeId.NodeId), true);
  }
  if (rb.ComponentName.IsBind())
  {
    status |= UA_Server_addReference(m_pServer, identification, UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY), ExNodeId(*rb.ComponentName.NodeId.NodeId), true);
  }
  if (rb.DeviceClass.IsBind())
  {
    status |= UA_Server_addReference(m_pServer, identification, UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY), ExNodeId(*rb.DeviceClass.NodeId.NodeId), true);
  }
  if (rb.HardwareRevision.IsBind())
  {
    status |= UA_Server_addReference(m_pServer, identification, UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY), ExNodeId(*rb.HardwareRevision.NodeId.NodeId), true);
  }
  if (rb.Manufacturer.IsBind())
  {
    status |= UA_Server_addReference(m_pServer, identification, UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY), ExNodeId(*rb.Manufacturer.NodeId.NodeId), true);
  }
  if (rb.ManufacturerUri.IsBind())
  {
    status |= UA_Server_addReference(m_pServer, identification, UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY), ExNodeId(*rb.ManufacturerUri.NodeId.NodeId), true);
  }
  if (rb.Model.IsBind())
  {
    status |= UA_Server_addReference(m_pServer, identification, UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY), ExNodeId(*rb.Model.NodeId.NodeId), true);
  }
  if (rb.ProductCode.IsBind())
  {
    status |= UA_Server_addReference(m_pServer, identification, UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY), ExNodeId(*rb.ProductCode.NodeId.NodeId), true);
  }
  if (rb.ProductInstanceUri.IsBind())
  {
    status |= UA_Server_addReference(m_pServer, identification, UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY), ExNodeId(*rb.ProductInstanceUri.NodeId.NodeId), true);
  }
  if (rb.SerialNumber.IsBind())
  {
    status |= UA_Server_addReference(m_pServer, identification, UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY), ExNodeId(*rb.SerialNumber.NodeId.NodeId), true);
  }
  if (rb.SoftwareRevision.IsBind())
  {
    status |= UA_Server_addReference(m_pServer, identification, UA_NODEID_NUMERIC(0, UA_NS0ID_HASPROPERTY), ExNodeId(*rb.SoftwareRevision.NodeId.NodeId), true);
  }

  if (status != UA_STATUSCODE_GOOD)
  {
    throw std::runtime_error("Add UA_Server_addReference in add Identification failed.");
  }

  status = UA_Server_addNode_finish(m_pServer, identification);

  if (status != UA_STATUSCODE_GOOD)
  {
    throw std::runtime_error("UA_Server_addNode_finish in add Identification failed.");
  }
}
