#include "BasicRobot.hpp"

BasicRobot::BasicRobot(UA_Server *pServer) : BasicRobot(pServer, true) {}

BasicRobot::BasicRobot(UA_Server *pServer, bool initialize) : InstantiatedRobot(pServer) {
  if (initialize) {
    Name = "BasicRobot";
    CreateObject();
    AddIdentificationAddin();
  }
}

void BasicRobot::Simulate() { ++m_simStep; }
