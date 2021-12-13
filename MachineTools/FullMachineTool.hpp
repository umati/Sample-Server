/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2020-2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 */

#pragma once

#include <memory>

#include "../TypeDefinition/MachineTool/Alert.hpp"
#include "../UmatiServerLib/OpcUaCondition.hpp"
#include "../UmatiServerLib/StateMachineInstance.hpp"
#include "InstantiatedMachineTool.hpp"

class FullMachineTool : public InstantiatedMachineTool {
 public:
  FullMachineTool(UA_Server *pServer);

  void Simulate() override;

 protected:
  // Allow skipping initialization in subclasses
  FullMachineTool(UA_Server *pServer, bool initialize);
  void CreateObject() override;

  void InstantiateIdentification();
  void InstantiateMonitoring();
  void InstantiateTools();
  void InstantiatePrognosis();
  void InstantiateProduction();
  int m_simStep = 0;
  std::shared_ptr<OpcUaCondition<machineTool::Alert_t>> m_pAlert;
  UmatiServerLib::StateMachineInstance<machineTool::ProductionStateMachine_t> JobStateMachine;
};
