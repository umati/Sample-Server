#include <iostream>
#include <open62541/server_config_default.h>
#include "src_generated/namespace_iswexample_generated.h"
#include "src_generated/namespace_umati_generated.h"
#include "src_generated/iswexample.h"
#include <open62541/server.h>
#include <cstdint>

void onReadBuildYear(UA_Server *pServer, const UA_NodeId *sessionId,
	void *sessionContext, const UA_NodeId *nodeid,
	void *nodeContext, const UA_NumericRange *range,
	const UA_DataValue *oldValue)
{
	UA_Variant value;
	std::uint32_t i = 2019;
	UA_Variant_setScalar(&value, &i , &UA_TYPES[UA_TYPES_UINT32]);
	UA_Server_writeValue(pServer, *nodeid, value);
}

int main(int argc, char* argv[]){
	UA_Server *pServer = UA_Server_new();
    UA_ServerConfig_setDefault(UA_Server_getConfig(pServer));

	std::cout << "TestNodesetCompiler" << std::endl;
	namespace_umati_generated(pServer);
	namespace_iswexample_generated(pServer);

	UA_ValueCallback valCallback;
	valCallback.onWrite = nullptr;
	valCallback.onRead = onReadBuildYear;

	UA_Server_setVariableNode_valueCallback(
		pServer,
		UA_NODEID_NUMERIC(3, UA_ISWEXAMPLE_ID_MACHINETOOLS_ISWEXAMPLE_IDENTIFICATION_MACHINE_BUILDYEAR),
		valCallback
		);
	bool running = true;
	UA_StatusCode retval = UA_Server_run(pServer, &running);

	return 0;
}