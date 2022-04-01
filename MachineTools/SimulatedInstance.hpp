/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2020-2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 */

#pragma once
#include <random>
#include <open62541/types.h>
#include "../UmatiServerLib/Publication.hpp"

class SimulatedInstance {
 public:
  struct MqttSettings {
    UA_NodeId *connectionIdent;
    std::string prefix;
    std::string publisherId;
    UA_NodeId monitoringWriterGroupIdent;
    UA_NodeId identificationWriterGroupIdent;
    UA_NodeId productionWriterGroupIdent;
    UA_NodeId equipmentWriterGroupIdent;
  };
  MqttSettings m_mqttSettings;
  Publisher m_publisher;
  
  SimulatedInstance();
  SimulatedInstance(MqttSettings mqttSetting);
  
  // Should be called every second
  virtual void Simulate() = 0;
  virtual ~SimulatedInstance() = 0;

 protected:
  // Seeded random generator
  std::mt19937 rnd;
};
