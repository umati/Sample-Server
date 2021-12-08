/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2020-2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 * Copyright 2020 (c) Tonja Heinemann, ISW University of Stuttgart (for umati and VDW e.V.)
 */

#pragma once
#include "InstantiatedMachineTool.hpp"

class BasicMachineTool : public InstantiatedMachineTool {
 public:
  BasicMachineTool(UA_Server *pServer);

  void Simulate() override;
  virtual ~BasicMachineTool() = default;

 protected:
  void CreateObject() override;

  void InstantiateIdentification();
  void InstantiateMonitoring();
  void InstantiateProduction();
  int m_simStep = 0;
};
