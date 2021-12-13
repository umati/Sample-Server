/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2019-2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 */

#include "NodeValue.hpp"

void NodeValue::operator=(NodeValue::GetValue_t F) { fCallback = F; }

UA_DataValue NodeValue::Value() { return fCallback(); }

NodeValue::NodeValue(const open62541Cpp::UA_NodeId &nodeId) : NodeId(nodeId) {}

NodeValue::NodeValue(NodeValue &&other) : NodeId(std::move(other.NodeId)) {}
