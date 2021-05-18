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
