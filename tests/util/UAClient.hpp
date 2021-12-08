/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2020-2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 */

#pragma once
#include <open62541/client.h>

#include <Open62541Cpp/UA_NodeId.hpp>
#include <string>

class UAClient {
 public:
  UAClient();
  ~UAClient();

  UA_StatusCode Connect(std::string url);
  UA_StatusCode ReadValue(open62541Cpp::UA_NodeId nodeId);

 protected:
  UA_Client *m_pClient = nullptr;
};
