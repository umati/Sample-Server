/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2020-2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 */

#pragma once
#include <open62541/server.h>

#include <Open62541Cpp/UA_NodeId.hpp>
#include <cstdint>

#include "../TypeDefinition/MachineTool/MachineTool.hpp"
#include "../UmatiServerLib/NodesMaster.hpp"
#include "SimulatedInstance.hpp"

class InstantiatedMachineTool : public SimulatedInstance {
 public:
  InstantiatedMachineTool(UA_Server *pServer);

  const std::uint16_t &NsIndex;

 protected:
  virtual void CreateObject();

  virtual void InstantiateIdentification();
  virtual void InstantiateMonitoringMT();
  virtual void InstantiateMonitoringStacklight(std::list<UA_SignalColor> stacklightColors);
  virtual void InstantiateMonitoringChannel(int numChannels);
  virtual void SimulateStacklight();

  std::string MachineName;
  std::uint16_t m_nsIndex;
  machineTool::MachineTool_t mt;
  UA_Server *m_pServer;
  open62541Cpp::UA_NodeId m_nodeId = open62541Cpp::UA_NodeId((std::uint16_t)0, 0);
  NodesMaster n;
};
