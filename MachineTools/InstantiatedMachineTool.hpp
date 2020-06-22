
#pragma once
#include "SimulatedMachineTool.hpp"
#include "../TypeDefinition/MachineTool/MachineTool.hpp"
#include "../NodesMaster.hpp"

#include <open62541/server.h>
#include <Open62541Cpp/UA_NodeId.hpp>
#include <cstdint>

class InstantiatedMachineTool : public SimulatedMachineTool
{
  public:
  InstantiatedMachineTool(UA_Server *pServer);

  const std::uint16_t &NsIndex;
  protected:
  virtual void CreateObject();

  virtual void InstantiateIdentification();

  std::string MachineName;
  std::uint16_t m_nsIndex;
  machineTool::MachineTool_t mt;
  UA_Server *m_pServer;
  open62541Cpp::UA_NodeId m_nodeId = open62541Cpp::UA_NodeId((std::uint16_t) 0, 0);
  NodesMaster n;
};
