/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2020-2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 * Copyright 2022 (c) Patrick Möller, 3Yourmind GmbH
 */

#include "BasicAMMachine.hpp"

BasicAMMachine::BasicAMMachine(UA_Server *pServer) : BasicAMMachine(pServer, true) {}

BasicAMMachine::BasicAMMachine(UA_Server *pServer, bool initialize) : InstantiatedAMMachine(pServer) {
  if (initialize) {
    Name = "BasicAMMachine";
    CreateObject();
    InstantiateIdentification(); //FIXME: Replace with Robotics' AddIdentificationAddin?
  }
}

void BasicAMMachine::Simulate() { ++m_simStep; }
