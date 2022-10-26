/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2022 (c) Patrick Moeller, 3Yourmind GmbH
 */

#pragma once
#include "../MachineTools/InstantiatedMachineTool.hpp"

class ShowcaseAMMachine : public InstantiatedMachineTool {
 public:
  ShowcaseAMMachine(UA_Server *pServer);

  void Simulate() override;
  virtual ~ShowcaseAMMachine() = default;

 protected:
  void CreateObject() override;

  AdditiveManufacturing::AM_t machine;
  void InstantiateIdentification();
  void InstantiateMonitoring();
  void InstantiateTools();
  void InstantiateProduction();
  int m_simStep = 0;
};
