/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2021 (c) Christoph Ruckstetter, Michael Weinig AG
 */

#pragma once
#include "InstantiatedWoodworking.hpp"

class BasicWoodworking : public InstantiatedWoodworking {
 public:
  BasicWoodworking(UA_Server *pServer);

  void Simulate() override;
  virtual ~BasicWoodworking() = default;

 protected:
  void CreateObject() override;

  void InstantiateIdentification() override;
  void InstantiateMachineFlags() override;
  void InstantiateMachineOverview() override;

  int m_simStep = 0;
};
