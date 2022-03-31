/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2020-2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 * Copyright 2020 (c) Tonja Heinemann, ISW University of Stuttgart (for umati and VDW e.V.)
 */

#include "FullMachineToolDynamic.hpp"

FullMachineToolDynamic::FullMachineToolDynamic(UA_Server *pServer) : FullMachineToolDynamic(pServer, true, {}) {}

FullMachineToolDynamic::FullMachineToolDynamic(UA_Server *pServer, bool initialize, MqttSettings mqttSettings) : FullMachineTool(pServer, false, mqttSettings) {
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
    mt.Production->ActiveProgram->JobIdentifier.value = mt.Production->ProductionPlan->OrderedObjects.value.back()->Identifier.value;
    mt.Production->ActiveProgram->JobNodeId.value = *mt.Production->ProductionPlan->OrderedObjects.value.back().NodeId.NodeId;

    if (m_mqttSettings.connectionIdent != nullptr) {
      auto nId = m_publisher.m_namesToNodeIds["Production_WriterGroup"];
      TopicCreator tc{m_mqttSettings.prefix, m_mqttSettings.publisherId, "Production_WriterGroup", "_.ProductionPlan." + ss.str()};
      m_publisher.PublishFields(refl::reflect(job), job, tc, m_pServer, &nId, UA_FALSE, UA_TRUE);
    }

  } else if ((m_simStep % 10) == 8) {
    if (!mt.Production->ProductionPlan->OrderedObjects->empty()) {
      auto lastIt = --mt.Production->ProductionPlan->OrderedObjects->end();
      auto job = mt.Production->ProductionPlan->OrderedObjects->back();
      mt.Production->ProductionPlan->OrderedObjects.Delete(lastIt, m_pServer, n);
      auto name = "_.ProductionPlan." + job.value.Identifier.value + "_Writer";
      auto name2 = "_.ProductionPlan." + job.value.Identifier.value + ".State_Writer";

      mt.Production->ActiveProgram->JobIdentifier.value = mt.Production->ProductionPlan->OrderedObjects.value.front()->Identifier.value;
      mt.Production->ActiveProgram->JobNodeId.value = *mt.Production->ProductionPlan->OrderedObjects.value.front().NodeId.NodeId;

      if (m_mqttSettings.connectionIdent != nullptr) {
        auto nId = m_publisher.m_namesToNodeIds[name];
        removeDataSetWriter(m_pServer, &nId);
        auto nId2 = m_publisher.m_namesToNodeIds[name2];
        removeDataSetWriter(m_pServer, &nId2);
      }
    }
  }
}
