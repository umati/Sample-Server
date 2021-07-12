
#pragma once
#include <open62541/server.h>

#include <Open62541Cpp/UA_NodeId.hpp>
#include <cstdint>

#include "../MachineTools/SimulatedInstance.hpp"
#include "../TypeDefinition/Woodworking/WwMachine.hpp"
#include "../UmatiServerLib/NodesMaster.hpp"

class InstantiatedWoodworking : public SimulatedInstance {
 public:
  InstantiatedWoodworking(UA_Server *pServer);
  const std::uint16_t &NsIndex;

 protected:
  virtual void CreateObject();
  virtual void InstantiateIdentification();
  virtual void InstantiateMachineFlags();
  virtual void InstantiateMachineOverview();

  std::string MachineName;
  std::uint16_t m_nsIndex;
  woodworking::WwMachine_t ww;
  UA_Server *m_pServer;
  open62541Cpp::UA_NodeId m_nodeId = open62541Cpp::UA_NodeId((std::uint16_t)0, 0);
  NodesMaster n;
};
