/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2020-2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 */

#include "ConstNodeId.hpp"

#include <stdexcept>

#include "../UmatiServerLib/Util.hpp"

namespace UmatiServerLib::constexp {

open62541Cpp::UA_NodeId NodeId::UANodeId(UA_Server *pServer) const {
  if (NsUri == nullptr) {
    return open62541Cpp::UA_NodeId();
  }
  UA_UInt16 nsIndex = nsFromUri(pServer, NsUri);
  switch (IdentifierType) {
    case ::UA_NodeIdType::UA_NODEIDTYPE_NUMERIC: {
      return open62541Cpp::UA_NodeId(nsIndex, Identifier.numeric);
      break;
    }
    case ::UA_NodeIdType::UA_NODEIDTYPE_STRING: {
      return open62541Cpp::UA_NodeId(nsIndex, Identifier.string);
      break;
    }
    case ::UA_NodeIdType::UA_NODEIDTYPE_GUID: {
      throw std::runtime_error("GUID NodeId not implemented.");
      break;
    }
    case ::UA_NodeIdType::UA_NODEIDTYPE_BYTESTRING: {
      throw std::runtime_error("Bytestring NodeId not implemented.");
      break;
    }
    default:
      throw std::runtime_error("Unknown NodeId type.");
  }
}

}  // namespace UmatiServerLib::constexp
