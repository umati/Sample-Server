/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2022 (c) Patrick Moeller, 3Yourmind GmbH
 * Copyright 2022 (c) Dennis Hezel, 3Yourmind GmbH
 */

#pragma once
#include "../AdditiveManufacturing/InstantiatedAMMachine.hpp"

class ShowcaseAMMachine : public InstantiatedAMMachine {
 public:
  ShowcaseAMMachine(UA_Server *pServer);

  void Simulate() override;
  virtual ~ShowcaseAMMachine() = default;

 protected:
  void CreateObject() override;

  void InstantiateEquipment();
  void InstantiateTools();
  int m_simStep = 0;
};
