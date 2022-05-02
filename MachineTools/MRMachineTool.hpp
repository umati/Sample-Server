/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2020-2022 Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 * Copyright (c) 2020 Tonja Heinemann, ISW University of Stuttgart (for umati and VDW e.V.)
 */

#pragma once

#include <memory>

#include "../TypeDefinition/MachineTool/Alert.hpp"
#include "../UmatiServerLib/OpcUaCondition.hpp"
#include "../UmatiServerLib/StateMachineInstance.hpp"
#include "InstantiatedMachineTool.hpp"

class MRMachineTool : public InstantiatedMachineTool {
 public:
  MRMachineTool(UA_Server *pServer);

  void Simulate() override;

 protected:
  void CreateObject() override;

  void InstantiateIdentification();
  void InstantiateMonitoring();
  void InstantiateTools();
  void InstantiateProduction();
  int m_simStep = 0;
  int isOnFor;
  void SwitchOnStacklightColor(UA_SignalColor color);
  std::shared_ptr<OpcUaCondition<machineTool::Alert_t>> m_pAlert;
  std::shared_ptr<UmatiServerLib::StateMachineInstance<machineTool::ProductionStateMachine_t>> ActiveProgramStateMachine;
  std::shared_ptr<UmatiServerLib::StateMachineInstance<machineTool::ProductionStateMachine_t>> VdJobStateMachine;
  std::shared_ptr<UmatiServerLib::StateMachineInstance<machineTool::ProductionStateMachine_t>> VdJobProgramStateMachine;
};
