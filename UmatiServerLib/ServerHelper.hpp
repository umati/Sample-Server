#pragma once
#include <open62541/server.h>

#include <Open62541Cpp/UA_NodeId.hpp>

void writeEventNotifier(UA_Server *pServer, open62541Cpp::UA_NodeId nodeId, UA_Byte eventNotifier = UA_EVENTNOTIFIERTYPE_SUBSCRIBETOEVENTS);
