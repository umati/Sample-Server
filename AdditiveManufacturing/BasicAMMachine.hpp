/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2022 (c) Patrick Moeller, 3Yourmind GmbH
 */

#include "../MachineTools/InstantiatedMachineTool.hpp"

class BasicAMMachine : public InstantiatedMachineTool {
 public:
  BasicAMMachine(UA_Server *pServer);

  void Simulate() override;
  virtual ~BasicAMMachine() = default;

 protected:
  void CreateObject() override;

  AdditiveManufacturing::AM_t machine;
  UA_String m_resulturi[1];
  void InstantiateIdentification();
  void InstantiateMonitoring();
  void InstantiateProduction();
  void InstantiateTools();
  int m_simStep = 0;

  void initCorrection(GMS::CorrectionType_t &corr, std::string Identifier, std::string CharacteristicIdentfier, double value);

  void InstantiateResultManagement();
};