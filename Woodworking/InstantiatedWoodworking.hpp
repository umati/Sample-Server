/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2021 (c) Christoph Ruckstetter, Michael Weinig AG
 */

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
