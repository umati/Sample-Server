/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2022 (c) Patrick Moeller, 3Yourmind GmbH
 */

#include "BasicAMMachine.hpp"

BasicAMMachine::BasicAMMachine(UA_Server *pServer) : InstantiatedAMMachine(pServer) {
  MachineName = "BasicAMMachine";
  CreateObject();
}

void BasicAMMachine::Simulate() {
  ++m_simStep;
  if ((m_simStep % 2) == 1) {
    SimulateStacklight();
  }

  mt.Monitoring->MachineTool->PowerOnDuration = m_simStep / 3600;
}