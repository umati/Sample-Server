/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2022 (c) mindonwarp
 */

#pragma once
#include <open62541/server.h>

#include <Open62541Cpp/UA_NodeId.hpp>
#include <cstdint>

#include "../TypeDefinition/GMS/GMSType.hpp"
#include "../UmatiServerLib/NodesMaster.hpp"
#include "../MachineTools/SimulatedInstance.hpp"

class InstantiatedGMS : public SimulatedInstance {
 public:
  InstantiatedGMS(UA_Server *pServer);

  const std::uint16_t &NsIndex;

 protected:
  virtual void CreateObject();

  virtual void Simulate() override;

  virtual void InstantiateEquipment();
  virtual void InstantiateIdentification();
  virtual void InstantiateNotification();
  virtual void InstantiateProduction();
  virtual void InstantiateResultManagement();
  virtual void InstantiateMonitoring();

  // TODO: Take care of subcomponents later
  // virtual void InstantiateNotificationCalibration();
  // virtual void InstantiatePrognosesCalibration();

  std::string MachineName;
  std::uint16_t m_nsIndex;
  GMS::GMS_t gms;
  UA_Server *m_pServer;
  open62541Cpp::UA_NodeId m_nodeId = open62541Cpp::UA_NodeId((std::uint16_t)0, 0);
  NodesMaster n;
};
