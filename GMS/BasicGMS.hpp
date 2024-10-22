/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2022 (c) Sebastian Friedl, ISW University of Stuttgart (for VDMA e.V.)
* Copyright 2022 (c) Alen Galinec
 */

#pragma once
#include "InstantiatedGMS.hpp"
#include "../TypeDefinition/GMS/GMSType.hpp"

class BasicGMS : public InstantiatedGMS {
 public:
  BasicGMS(UA_Server *pServer);

  void Simulate() override;
  virtual ~BasicGMS() = default;

 protected:
  void CreateObject() override;

  GMS::GMS_t mt;
  UA_String m_resulturi[1];
  void InstantiateIdentification();
  void InstantiateMonitoring();
  void InstantiateProduction();
  void InstantiateTools();
  void InstantiateEquipment() override;
  void InstantiateNotification() override;
  int m_simStep = 0;

  void initCorrection(GMS::CorrectionType_t &corr, std::string Identifier, std::string CharacteristicIdentifier, double value);

  void InstantiateResultManagement();
};
