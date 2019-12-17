#include <iostream>
#include <open62541/server_config_default.h>
#include "src_generated/namespace_iswexample_generated.h"
#include "src_generated/namespace_umati_generated.h"
#include "src_generated/iswexample.h"
#include "src_generated/umati_nodeids.h"
#include <cstdint>
#include <functional>
#include "NodeValue.hpp"
#include "NodesMaster.hpp"
#include "BindValue.hpp"
#include <thread>
#include <chrono>
#include <mutex>
#include <atomic>
#include <open62541Cpp/UA_String.hpp>

struct MachineIdentification_t{
  std::uint32_t  BuildYear;
  std::string CatalogueName;
};

struct NotificationEvent_t {
  std::string Identifier;
  std::string Message;
  std::string SourceName;
  std::uint16_t Severity;
};

UA_NodeId setupNotificationEvent(UA_Server *pServer, NotificationEvent_t ev){
  UA_NodeId eventNodeId;
  UA_StatusCode retval = UA_Server_createEvent(
      pServer,
      UA_NODEID_NUMERIC(2, UA_UMATIID_NOTIFICATIONEVENTTYPE),
      &eventNodeId);

  if(retval != UA_STATUSCODE_GOOD){
    std::cerr << "Could not create event: " << retval << std::endl;
    return UA_NODEID_NULL;
  }

  // Setting the Time is required or else the event will not show up in UAExpert!
  UA_DateTime eventTime = UA_DateTime_now();
  UA_Server_writeObjectProperty_scalar(
      pServer,
      eventNodeId,
      UA_QUALIFIEDNAME(0, "Time"),
      &eventTime,
      &UA_TYPES[UA_TYPES_DATETIME]
      );
  {
    open62541Cpp::UA_String strIdentifier(ev.Identifier);
    UA_Variant *val = UA_Variant_new();
    UA_Variant_clear(val);
    UA_Variant_setScalarCopy(val, strIdentifier.String, &UA_TYPES[UA_TYPES_STRING]);
    UA_Server_writeObjectProperty(
        pServer,
        eventNodeId,
      UA_QUALIFIEDNAME(2, "Identifier"),
      *val
    );
  }

  {
    open62541Cpp::UA_String strIdentifier(ev.SourceName);
    UA_Variant *val = UA_Variant_new();
    UA_Variant_clear(val);
    UA_Variant_setScalarCopy(val, strIdentifier.String, &UA_TYPES[UA_TYPES_STRING]);
    UA_Server_writeObjectProperty(
        pServer,
        eventNodeId,
        UA_QUALIFIEDNAME(0, "SourceName"),
        *val
    );
  }

  {
    open62541Cpp::UA_String strMessage(ev.Message);
    UA_Variant *val = UA_Variant_new();
    UA_LocalizedText ltxtMessage = UA_LOCALIZEDTEXT("en-en", "");
    ltxtMessage.text = *strMessage.String;
    UA_Variant_clear(val);
    UA_Variant_setScalarCopy(val, &ltxtMessage, &UA_TYPES[UA_TYPES_LOCALIZEDTEXT]);
    UA_Server_writeObjectProperty(
        pServer,
        eventNodeId,
        UA_QUALIFIEDNAME(0, "Message"),
        *val
    );
  }

  {
    UA_Variant *val = UA_Variant_new();
    UA_Variant_clear(val);
    UA_Variant_setScalarCopy(val, &ev.Severity, &UA_TYPES[UA_TYPES_UINT16]);
    UA_Server_writeObjectProperty(
        pServer,
        eventNodeId,
        UA_QUALIFIEDNAME(0, "Severity"),
        *val
    );
  }

  return eventNodeId;
}

void simulate(MachineIdentification_t *pInfo, std::atomic_bool &running, std::mutex &accessDataMutex, UA_Server *pServer){
  std::unique_lock<std::remove_reference<decltype(accessDataMutex)>::type> ul(accessDataMutex);
  ul.unlock();
  while(running){
    ul.lock();
    ++(pInfo->BuildYear);
    NotificationEvent_t ev {.Identifier = "MyId", .Message="MessageTxt", .SourceName="MySource", .Severity = 500};
    auto evNodeId = setupNotificationEvent(pServer, ev);
    auto retval = UA_Server_triggerEvent(pServer, evNodeId, UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER), NULL, UA_TRUE);
    ul.unlock();
    std::this_thread::yield();
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }

}

int main(int argc, char *argv[]) {

  UA_Server *pServer = UA_Server_new();
  UA_ServerConfig_setDefault(UA_Server_getConfig(pServer));

  std::cout << "ExampleUmatiServer" << std::endl;
  namespace_umati_generated(pServer);
  namespace_iswexample_generated(pServer);

  std::mutex accessDataMutex;
  NodesMaster n(pServer);
  MachineIdentification_t identificationMachine = {.BuildYear = 2019, .CatalogueName = "ISW Example"};
  bindValue(
      n(3, UA_ISWEXAMPLE_ID_MACHINETOOLS_ISWEXAMPLE_IDENTIFICATION_MACHINE_BUILDYEAR),
      identificationMachine.BuildYear
      );
  bindValue(
      n(3, UA_ISWEXAMPLE_ID_MACHINETOOLS_ISWEXAMPLE_IDENTIFICATION_MACHINE_CATALOGUENAME),
      identificationMachine.CatalogueName
      );

  n.SetCallbacks();

  std::atomic_bool running {true};

  //UA_Server_run(pServer, &running);
  UA_Server_run_startup(pServer);
  std::unique_lock<decltype(accessDataMutex)> ul(accessDataMutex);
  std::thread t(simulate, &identificationMachine, std::ref(running), std::ref(accessDataMutex), pServer);
  ul.unlock();
  while(running)
  {
    ul.lock();
    std::uint16_t timeout = UA_Server_run_iterate(pServer, false);
    ul.unlock();
    std::this_thread::yield();
    std::this_thread::sleep_for(std::chrono::milliseconds(timeout));
  }

  UA_Server_run_shutdown(pServer);

  return 0;
}