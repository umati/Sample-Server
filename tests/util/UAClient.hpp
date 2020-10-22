#pragma once
#include <open62541/client.h>
#include <Open62541Cpp/UA_NodeId.hpp>
#include <string>

class UAClient {
  public:
  UAClient();
  ~UAClient();

  UA_StatusCode Connect(std::string url);
  UA_StatusCode ReadValue(open62541Cpp::UA_NodeId nodeId);

  protected:
  UA_Client *m_pClient = nullptr;

};
