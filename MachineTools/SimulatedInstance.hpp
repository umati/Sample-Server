/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2020-2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 */

#pragma once
#include <random>

class SimulatedInstance {
 public:
  SimulatedInstance();
  // Should be called every second
  virtual void Simulate() = 0;
  virtual ~SimulatedInstance() = 0;

 protected:
  // Seeded random generator
  std::mt19937 rnd;
};
