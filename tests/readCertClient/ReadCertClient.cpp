
#include <open62541/client_config_default.h>
#include <open62541/client_highlevel.h>
#include <open62541/plugin/log_stdout.h>

#include <iostream>
#include <memory>
#include <string>

std::ostream& operator<<(std::ostream& ostr, const UA_String& s) {
  auto str = std::string((char*)s.data, s.length);
  ostr << str;
  return ostr;
}

std::ostream& operator<<(std::ostream& ostr, UA_MessageSecurityMode secMode) {
  switch (secMode) {
    case UA_MESSAGESECURITYMODE_INVALID: {
      ostr << "UA_MESSAGESECURITYMODE_INVALID";
      break;
    }
    case UA_MESSAGESECURITYMODE_NONE: {
      ostr << "UA_MESSAGESECURITYMODE_NONE";
      break;
    }
    case UA_MESSAGESECURITYMODE_SIGN: {
      ostr << "UA_MESSAGESECURITYMODE_SIGN";
      break;
    }
    case UA_MESSAGESECURITYMODE_SIGNANDENCRYPT: {
      ostr << "UA_MESSAGESECURITYMODE_SIGNANDENCRYPT";
      break;
    }
    default: {
      ostr << "UA_MESSAGESECURITYMODE_UNKNOWN " << (int)secMode;
      break;
    }
  }

  return ostr;
}

void printEndpoint(const UA_EndpointDescription& endpointDesc) {
  std::cout << "EndpointURL: " << endpointDesc.endpointUrl << std::endl;
  std::cout << " SecurityPolicyUri:" << endpointDesc.securityPolicyUri << std::endl;
  std::cout << " securityMode:" << endpointDesc.securityMode << std::endl;
  std::cout << " ServerCert:" << endpointDesc.serverCertificate << std::endl;
}

int main(int argc, char* argv[]) {
  std::cout << "Begin ReadCertClient" << std::endl;
  std::string serverUri = "opc.tcp://localhost:4840";
  if (argc >= 2) {
    serverUri = argv[1];
  }

  std::shared_ptr<UA_Client> pClientShared(UA_Client_new(), UA_Client_delete);
  auto pClient = pClientShared.get();
  UA_ClientConfig_setDefault(UA_Client_getConfig(pClient));
  size_t numEndpoints = 0;
  UA_EndpointDescription* pEndpointDescriptions;
  UA_StatusCode retval = UA_Client_getEndpoints(pClient, serverUri.c_str(), &numEndpoints, &pEndpointDescriptions);
  if (retval != UA_STATUSCODE_GOOD) {
    UA_LOG_INFO(UA_Log_Stdout, UA_LOGCATEGORY_USERLAND, "UA_Client_getEndpoints failed with status code %s", UA_StatusCode_name(retval));
    return EXIT_FAILURE;
  }

  for (size_t i = 0; i < numEndpoints; ++i) {
    printEndpoint(pEndpointDescriptions[i]);
  }
  UA_Array_delete(pEndpointDescriptions, numEndpoints, &UA_TYPES[UA_TYPES_ENDPOINTDESCRIPTION]);

  std::cout << "End ReadCertClient" << std::endl;
  return EXIT_SUCCESS;
}
