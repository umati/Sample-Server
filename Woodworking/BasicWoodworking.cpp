/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2021 (c) Christoph Ruckstetter, Michael Weinig AG
 */

#include "BasicWoodworking.hpp"

BasicWoodworking::BasicWoodworking(UA_Server *pServer) : InstantiatedWoodworking(pServer) {
  MachineName = "BasicWoodworking";
  CreateObject();
}

void BasicWoodworking::CreateObject() {
  InstantiatedWoodworking::CreateObject();
  InstantiateIdentification();
  InstantiateMachineFlags();
  InstantiateMachineOverview();
}

void BasicWoodworking::InstantiateIdentification() { InstantiatedWoodworking::InstantiateIdentification(); }

void BasicWoodworking::InstantiateMachineFlags() { InstantiatedWoodworking::InstantiateMachineFlags(); }

void BasicWoodworking::InstantiateMachineOverview() { InstantiatedWoodworking::InstantiateMachineOverview(); }

void BasicWoodworking::Simulate() { ++m_simStep; }
