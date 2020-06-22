#pragma once
#include "SimulatedMachineTool.hpp"
#include <open62541/server.h>
#include "../TypeDefinition/MachineTool/MachineTool.hpp"
#include <Open62541Cpp/UA_NodeId.hpp>
#include <cstdint>
#include "../NodesMaster.hpp"
#include <memory>
#include "../OpcUaCondition.hpp"
#include "../TypeDefinition/MachineTool/Alert.hpp"

class FullMachineTool : public SimulatedMachineTool
{
  public:
  FullMachineTool(UA_Server *pServer);

  void Simulate() override;

  const std::uint16_t &NsIndex;
  protected:
  void CreateObject();
  void InstantiateIdentification();
  void InstantiateMonitoring();
  void InstantiateTools();
  void InstantiatePrognosis();
  std::uint16_t m_nsIndex;
  int m_simStep = 0;
  std::shared_ptr<OpcUaCondition<machineTool::Alert_t>> m_pAlert;
  machineTool::MachineTool_t mt;
  UA_Server *m_pServer;
  open62541Cpp::UA_NodeId m_nodeId = open62541Cpp::UA_NodeId((std::uint16_t) 0, 0);
  NodesMaster n;
};
