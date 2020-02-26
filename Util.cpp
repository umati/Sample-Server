
#include "Util.hpp"
#include <Open62541Cpp/UA_String.hpp>
#include <iostream>

UA_Int16 nsFromUri(UA_Server *pServer, std::string uri)
{
  std::size_t nsIndex = 0;
  open62541Cpp::UA_String uastr(uri);
  auto ret = UA_Server_getNamespaceByName(pServer, *uastr.String, &nsIndex);
  if (ret != UA_STATUSCODE_GOOD)
  {
    std::cout << "Could not resolve namespace URI: " << uri << std::endl;
    throw std::runtime_error("NsUri resolution failed.");
  }

  return nsIndex;
}
