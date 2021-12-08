/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2020-2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 */

#include "UAClient.hpp"

#include <open62541/client_config_default.h>
#include <open62541/client_highlevel.h>

UAClient::UAClient() {
  m_pClient = UA_Client_new();
  UA_ClientConfig_setDefault(UA_Client_getConfig(m_pClient));
}

UAClient::~UAClient() {}

UA_StatusCode UAClient::Connect(std::string url) { return UA_Client_connect(m_pClient, url.c_str()); }

UA_StatusCode UAClient::ReadValue(open62541Cpp::UA_NodeId nodeId) {
  UA_Variant Variant;
  UA_Variant_init(&Variant);
  auto ret = UA_Client_readValueAttribute(m_pClient, *nodeId.NodeId, &Variant);
  UA_Variant_clear(&Variant);
  return ret;
}
