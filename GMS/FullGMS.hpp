/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2022 (c) mindonwarp
 */

#pragma once

#include "../TypeDefinition/GMS/GMSType.hpp"

#include "InstantiatedGMS.hpp"

class FullGMS : public InstantiatedGMS {
 public:
  FullGMS(UA_Server *pServer);

  virtual ~FullGMS() = default;

 protected:
  void CreateObject() override;

  void InstantiateIdentification() override;
  void InstantiateMonitoring() override;
  void InstantiateProduction() override;
  void InstantiateEquipment() override;
};
