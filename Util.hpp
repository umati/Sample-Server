#pragma once
#include <open62541/server.h>
#include <string>

UA_Int16 nsFromUri(UA_Server *pServer, std::string uri);
