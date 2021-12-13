/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2020-2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 */

#include <open62541/server.h>

#include <Open62541Cpp/UA_NodeId.hpp>

#include "../MachineTools/SimulatedInstance.hpp"
#include "../TypeDefinition/Robotics/MotionDevice.hpp"
#include "../UmatiServerLib/NodesMaster.hpp"

class InstantiatedRobot : public SimulatedInstance {
 private:
 public:
  InstantiatedRobot(UA_Server *pServer);
  ~InstantiatedRobot();
  const std::uint16_t &NsIndex;

 protected:
  virtual void CreateObject();
  virtual void AddIdentificationAddin();
  std::string Name;
  std::uint16_t m_nsIndex = 0;
  robotics::MotionDevice_t rb;
  UA_Server *m_pServer;
  open62541Cpp::UA_NodeId m_nodeId = open62541Cpp::UA_NodeId((std::uint16_t)0, 0);
  NodesMaster n;
};
