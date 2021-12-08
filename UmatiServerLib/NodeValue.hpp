/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2019-2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 */

#pragma once

#include <open62541/types.h>

#include <Open62541Cpp/UA_NodeId.hpp>
#include <functional>

/**
 * @brief Class for accessting the value of a single node.
 *
 * Allow setting an callback function, which is called, when the value is needed.
 * @todo Use new External DataValue concept of open62541
 */
class NodeValue {
  typedef std::function<UA_DataValue()> GetValue_t;
  GetValue_t fCallback = nullptr;
  open62541Cpp::UA_NodeId NodeId;
  NodeValue(const NodeValue&) = delete;

 public:
  open62541Cpp::UA_NodeId GetNodeId() { return NodeId; }
  NodeValue(NodeValue&& other);
  NodeValue(const open62541Cpp::UA_NodeId& nodeId);
  // Use variadict expression https://stackoverflow.com/questions/9242234/c11-variadic-stdfunction-parameter
  void operator=(GetValue_t F);

  UA_DataValue Value();
};
