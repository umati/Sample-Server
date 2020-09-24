#include "InstantiatedRobot.hpp"

class BasicRobot : public InstantiatedRobot
{
public:
  BasicRobot(UA_Server *pServer);
protected:
  BasicRobot(UA_Server *pServer, bool initialize);
  void Simulate() override;
  int m_simStep = 0;
};
