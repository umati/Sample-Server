/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2020-2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 */

#pragma once
#include "FullMachineTool.hpp"

// Chang address space during runtime
class FullMachineToolDynamic : public FullMachineTool {
 public:
  FullMachineToolDynamic(UA_Server *pServer);

  void Simulate() override;

 protected:
  // Allow skipping initialization in subclasses
  FullMachineToolDynamic(UA_Server *pServer, bool initialize);
};
