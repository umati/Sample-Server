/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2020-2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 */

#include "InstantiatedRobot.hpp"

class BasicRobot : public InstantiatedRobot {
 public:
  BasicRobot(UA_Server *pServer);

 protected:
  BasicRobot(UA_Server *pServer, bool initialize);
  void Simulate() override;
  int m_simStep = 0;
};
