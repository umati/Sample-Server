#include <iostream>
#include <open62541/server_config_default.h>
#include "src_generated/namespace_iswexample_generated.h"
#include "src_generated/namespace_umati_generated.h"
#include "src_generated/iswexample.h"
#include <cstdint>
#include <functional>
#include "NodeValue.hpp"
#include "NodesMaster.hpp"
#include "BindValue.hpp"
#include <thread>
#include <chrono>
#include <mutex>
#include <atomic>

struct MachineIdentification_t{
  std::uint32_t  BuildYear;
  std::string CatalogueName;
};


void simulate(MachineIdentification_t *pInfo, std::atomic_bool &running, std::mutex &accessDataMutex){
  std::unique_lock<std::remove_reference<decltype(accessDataMutex)>::type> ul(accessDataMutex);
  ul.unlock();
  while(running){
    ul.lock();
    ++(pInfo->BuildYear);
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
  std::thread t(simulate, &identificationMachine, std::ref(running), std::ref(accessDataMutex));

  //UA_Server_run(pServer, &running);
  UA_Server_run_startup(pServer);
  std::unique_lock<decltype(accessDataMutex)> ul(accessDataMutex);
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