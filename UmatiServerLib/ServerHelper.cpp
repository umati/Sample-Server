/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2020-2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 */

#include "ServerHelper.hpp"

#include <iostream>
#include <sstream>
#include <stdexcept>

void writeEventNotifier(UA_Server *pServer, open62541Cpp::UA_NodeId nodeId, UA_Byte eventNotifier) {
  auto status = UA_Server_writeEventNotifier(pServer, *nodeId.NodeId, eventNotifier);
  if (status != UA_STATUSCODE_GOOD) {
    std::stringstream ss;
    ss << "WriteEventNotifier failed: " << UA_StatusCode_name(status);
    throw std::runtime_error(ss.str());
  }
}
