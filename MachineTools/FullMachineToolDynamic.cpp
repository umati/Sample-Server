#include "FullMachineToolDynamic.hpp"

FullMachineToolDynamic::FullMachineToolDynamic(UA_Server *pServer) : FullMachineToolDynamic(pServer, true) {}

FullMachineToolDynamic::FullMachineToolDynamic(UA_Server *pServer, bool initialize) : FullMachineTool(pServer, false) {
  if (initialize) {
    MachineName = "FullMachineToolDynamic";
    CreateObject();
  }
}

void FullMachineToolDynamic::Simulate() {
  FullMachineTool::Simulate();
  if ((m_simStep % 10) == 1) {
    std::stringstream ss;
    ss << "Job " << m_simStep;
    auto &job = mt.Production->ProductionPlan->OrderedObjects.Add(m_pServer, n, {m_nsIndex, ss.str()});
    job.Identifier = ss.str();
    job.RunsCompleted = 0;
    job.RunsPlanned->Value = 2;
    mt.Production->ActiveProgram->JobIdentifier = mt.Production->ProductionPlan->OrderedObjects.value.back()->Identifier.value;
    mt.Production->ActiveProgram->JobNodeId = *mt.Production->ProductionPlan->OrderedObjects.value.back().NodeId.NodeId;
  } else if ((m_simStep % 10) == 8) {
    if (!mt.Production->ProductionPlan->OrderedObjects->empty()) {
      auto lastIt = --mt.Production->ProductionPlan->OrderedObjects->end();
      mt.Production->ProductionPlan->OrderedObjects.Delete(lastIt, m_pServer, n);
      mt.Production->ActiveProgram->JobIdentifier = mt.Production->ProductionPlan->OrderedObjects.value.front()->Identifier.value;
      mt.Production->ActiveProgram->JobNodeId = *mt.Production->ProductionPlan->OrderedObjects.value.front().NodeId.NodeId;
    }
  }
}
