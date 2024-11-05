/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2020-2022 Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 * Copyright (c) 2020 Tonja Heinemann, ISW University of Stuttgart (for umati and VDW e.V.)
 */

#include "MRMachineTool.hpp"

#include <open62541/types_generated.h>

#include "../TypeDefinition/MachineTool/Alert.hpp"
#include "../TypeDefinition/MachineTool/ProductionJobTransitionEvent.hpp"
#include "../UmatiServerLib/BindRefl.hpp"
#include "../UmatiServerLib/OpcUaCondition.hpp"
#include "../UmatiServerLib/ServerHelper.hpp"

MRMachineTool::MRMachineTool(UA_Server *pServer) : InstantiatedMachineTool(pServer) {
  MachineName = "MRMachineTool";
  CreateObject();
  isOnFor = m_simStep;
}

void MRMachineTool::CreateObject() {
  InstantiatedMachineTool::CreateObject();

  InstantiateIdentification();
  InstantiateMonitoring();
  InstantiateProduction();
  InstantiateTools();

  InstantiateOptional(mt.Notification->Messages, m_pServer, n);
  writeEventNotifier(m_pServer, mt.Notification->Messages.NodeId);
}

void MRMachineTool::InstantiateIdentification() {
  InstantiatedMachineTool::InstantiateIdentification();
  InstantiateOptional(mt.Identification->YearOfConstruction, m_pServer, n);
  InstantiateOptional(mt.Identification->SoftwareRevision, m_pServer, n);
  InstantiateOptional(mt.Identification->ProductCode, m_pServer, n);
  InstantiateOptional(mt.Identification->Location, m_pServer, n);
  InstantiateOptional(mt.Identification->Model, m_pServer, n);

  mt.Identification->Location = "JIMTOF E7 E7128/VIRTUAL 1 1/N 48.781340 E 9.165731";
  mt.Identification->SerialNumber = std::string{"070-101-098-14"};
  mt.Identification->Manufacturer = {"", "ISW UA4MT Team"};
  mt.Identification->Model = {"", "T3IUTH"};
  mt.Identification->ProductInstanceUri = "http://isw.uni-stuttgart.de/#T3IUTH/070-101-098-14";
  mt.Identification->ProductCode = "2684-rga-t842574-b7";
  mt.Identification->YearOfConstruction = 2020;
  mt.Identification->SoftwareRevision = "4.30.2";
}

void MRMachineTool::InstantiateMonitoring() {
  InstantiateMonitoringMT();
  mt.Monitoring->MachineTool->OperationMode = UA_MachineOperationMode::UA_MACHINEOPERATIONMODE_SETUP;

  InstantiateOptional(mt.Monitoring->MachineTool->FeedOverride, m_pServer, n);
  InstantiateOptional(mt.Monitoring->MachineTool->IsWarmUp, m_pServer, n);
  mt.Monitoring->MachineTool->FeedOverride->Value = 0.0;
  mt.Monitoring->MachineTool->FeedOverride->EURange->high = 100.0;
  mt.Monitoring->MachineTool->FeedOverride->EURange->low = 0.0;
  mt.Monitoring->MachineTool->FeedOverride->EngineeringUnits->DisplayName = {"", "%"};
  mt.Monitoring->MachineTool->IsWarmUp = true;

  InstantiateMonitoringStacklight({
    UA_SignalColor::UA_SIGNALCOLOR_GREEN,
    UA_SignalColor::UA_SIGNALCOLOR_YELLOW,
    UA_SignalColor::UA_SIGNALCOLOR_RED,
  });
  n.Remove(mt.Monitoring->Stacklight->NodeVersion.NodeId);
  SwitchOnStacklightColor(UA_SignalColor::UA_SIGNALCOLOR_YELLOW);
}

void MRMachineTool::InstantiateProduction() {
  InstantiateOptional(mt.Production->ActiveProgram->State, m_pServer, n);
  InstantiateOptional(mt.Production->ActiveProgram->JobNodeId, m_pServer, n);
  InstantiateOptional(mt.Production->ActiveProgram->JobIdentifier, m_pServer, n);
  InstantiateOptional(mt.Production->ProductionPlan, m_pServer, n);

  mt.Production->ActiveProgram->Name = "heart.nc";
  mt.Production->ActiveProgram->NumberInList = 0;
  ActiveProgramStateMachine =
    std::make_shared<UmatiServerLib::StateMachineInstance<machineTool::ProductionStateMachine_t>>(mt.Production->ActiveProgram->State.value, m_pServer);
  ActiveProgramStateMachine->SetState(0);

  auto &job = mt.Production->ProductionPlan->OrderedObjects.Add(m_pServer, n, {m_nsIndex, "VDJob"});
  job.Identifier = std::string("VDJob");
  job.RunsCompleted = 0;
  job.RunsPlanned->Value = 7;
  job.RunsPlanned->IsValid = true;
  VdJobStateMachine = std::make_shared<UmatiServerLib::StateMachineInstance<machineTool::ProductionStateMachine_t>>(job.State.value, m_pServer);
  VdJobStateMachine->SetState(0);
  job.NumberInList = 0;
  auto &program = mt.Production->ProductionPlan->OrderedObjects.value.front()->ProductionPrograms->OrderedObjects.Add<machineTool::ProductionProgram_t>(
    m_pServer, n, {m_nsIndex, "heart.nc"});

  InstantiateOptional(program.State, m_pServer, n);
  program.Name = "heart.nc";
  VdJobProgramStateMachine = std::make_shared<UmatiServerLib::StateMachineInstance<machineTool::ProductionStateMachine_t>>(program.State.value, m_pServer);
  VdJobProgramStateMachine->SetState(0);

  mt.Production->ActiveProgram->JobNodeId = *mt.Production->ProductionPlan->OrderedObjects.value.front().NodeId.NodeId;
  mt.Production->ActiveProgram->JobIdentifier = job.Identifier;

  writeEventNotifier(m_pServer, mt.Production->ActiveProgram->State.NodeId);
  writeEventNotifier(m_pServer, job.State.NodeId);
  writeEventNotifier(m_pServer, program.State.NodeId);
}

void MRMachineTool::InstantiateTools() {
  InstantiateOptional(mt.Equipment->Tools, m_pServer, n);
  InstantiateOptional(mt.Equipment->Tools->NodeVersion, m_pServer, n);
  n.Remove(mt.Equipment->Tools->NodeVersion.NodeId);
  auto &tool = mt.Equipment->Tools->Tool.Add<machineTool::Tool_t>(m_pServer, n, {m_nsIndex, "Tool1"});
  tool.ControlIdentifier1 = 2;
  tool.ControlIdentifierInterpretation = UA_ToolManagement::UA_TOOLMANAGEMENT_NUMBERBASED;
  tool.Locked->Value = false;
  tool.Locked->ReasonForLocking = UA_ToolLocked::UA_TOOLLOCKED_OTHER;
  tool.Locked->ReasonForLocking.StatusCode = UA_STATUSCODE_BADNOTHINGTODO;
  tool.Name = "Tool 0815";
  InstantiateOptional(tool.Name, m_pServer, n);
  InstantiateOptional(tool.ToolLife, m_pServer, n);
  auto &toolLifeRotations = tool.ToolLife->ToolLifeEntry.Add<machineTool::ToolLife_t<std::int32_t>>(m_pServer, n, {m_nsIndex, "Rotations"});
  toolLifeRotations.Indication = UA_ToolLifeIndication::UA_TOOLLIFEINDICATION_OTHER;
  toolLifeRotations.Value = 512;
  toolLifeRotations.IsCountingUp = true;
  toolLifeRotations.LimitValue = 1 << 20;
  InstantiateOptional(toolLifeRotations.LimitValue, m_pServer, n);
}

void MRMachineTool::Simulate() {
  ++m_simStep;
  ++isOnFor;

  mt.Monitoring->MachineTool->PowerOnDuration = isOnFor / 3600;

  auto &vdjob = mt.Production->ProductionPlan->OrderedObjects.value.front();
  auto &vdprog = vdjob->ProductionPrograms->OrderedObjects.value.front();
  auto &aprog = mt.Production->ActiveProgram;

  // Job in initializing
  if (m_simStep == 30 && vdjob->State->CurrentState->Number.value == 0) {
    mt.Monitoring->MachineTool->FeedOverride->Value = 0.0;
    mt.Monitoring->MachineTool->IsWarmUp = false;
    SwitchOnStacklightColor(UA_SignalColor::UA_SIGNALCOLOR_GREEN);
    mt.Monitoring->MachineTool->OperationMode = UA_MachineOperationMode::UA_MACHINEOPERATIONMODE_AUTOMATIC;

    VdJobStateMachine->SetState(1);

    m_simStep = 0;

    machineTool::ProductionJobTransitionEvent_t jtevent;
    std::stringstream ss;
    ss << "Transition to Running triggered";
    jtevent.Message = {"en", ss.str()};

    jtevent.Severity = 20;

    jtevent.SourceName = "MRMachineTool";
    jtevent.Identifier = vdjob->Identifier.value;
    jtevent.RunsPlanned->Value = vdjob->RunsPlanned->Value.value;
    jtevent.RunsPlanned->IsValid = vdjob->RunsPlanned->IsValid.value;
    jtevent.RunsCompleted = vdjob->RunsCompleted.value;
    jtevent.Transition->Value = {"en", "InitializingToRunning"};
    jtevent.Transition->Id =
      UA_NODEID_NUMERIC(nsFromUri(m_pServer, constants::NsMachineToolUri), UA_MACHINETOOLID_PRODUCTIONSTATEMACHINETYPE_INITIALIZINGTORUNNING);
    jtevent.FromState->Value = {"en", "Initializing"};
    jtevent.FromState->Id = UA_NODEID_NUMERIC(nsFromUri(m_pServer, constants::NsMachineToolUri), UA_MACHINETOOLID_PRODUCTIONSTATEMACHINETYPE_INITIALIZING);
    jtevent.ToState->Value = {"en", "Running"};
    jtevent.ToState->Id = UA_NODEID_NUMERIC(nsFromUri(m_pServer, constants::NsMachineToolUri), UA_MACHINETOOLID_PRODUCTIONSTATEMACHINETYPE_RUNNING);

    OpcUaEvent ev(jtevent, m_pServer, open62541Cpp::UA_NodeId(UA_NODEID_NUMERIC(0, UA_NS0ID_SERVER)));
  }

  // Job in running
  else if (vdjob->State->CurrentState->Number.value == 1) {
    // Program is not running
    if (m_simStep == 2 && aprog->State->CurrentState->Number.value != 1) {
      ActiveProgramStateMachine->SetState(0);
      VdJobProgramStateMachine->SetState(0);
    } else if (m_simStep == 3 && aprog->State->CurrentState->Number.value != 1) {
      ActiveProgramStateMachine->SetState(1);
      VdJobProgramStateMachine->SetState(1);

      mt.Monitoring->MachineTool->FeedOverride->Value = 100.0;

      /**
      machineTool::ProductionJobTransitionEvent_t ptevent;
      std::stringstream ss;
      ss << "Transition to Running triggered";
      ptevent.Message = {"en", ss.str()};
      ptevent.Severity = 20;
      ptevent.SourceName = "MRMachineTool";
      ptevent.Identifier = vdjob->Identifier;
      ptevent.RunsPlanned->Value = vdjob->RunsPlanned->Value;
      ptevent.RunsPlanned->IsValid = vdjob->RunsPlanned->IsValid;
      ptevent.RunsCompleted = vdjob->RunsCompleted;
      ptevent.FromState->Value = {"en", "Initializing"};
      ptevent.ToState->Value = {"en", "Running"};
      ptevent.Transition->Value = {"en", "InitializingToRunning"};
      OpcUaEvent ev(ptevent, m_pServer, aprog->State.NodeId);
      **/
    }

    // Program is running
    else if (m_simStep == 17 && aprog->State->CurrentState->Number.value == 1) {
      ActiveProgramStateMachine->SetState(2);
      VdJobProgramStateMachine->SetState(2);

      mt.Monitoring->MachineTool->FeedOverride->Value = 0.0;
    }

    // one more run completed
    else if (m_simStep == 19 && vdjob->RunsCompleted.value < 7) {
      vdjob->RunsCompleted.value += 1;
      VdJobStateMachine->SetState(1);
      m_simStep = 0;
    }

    // all runs ended/overproduction
    else if (vdjob->RunsCompleted.value >= 7) {
      mt.Monitoring->MachineTool->FeedOverride->Value = 0.0;
      SwitchOnStacklightColor(UA_SignalColor::UA_SIGNALCOLOR_YELLOW);

      vdjob->RunsCompleted = 0;
      ActiveProgramStateMachine->SetState(2);
      VdJobProgramStateMachine->SetState(2);
      VdJobStateMachine->SetState(2);
    }
  }

  // Job ended and 5 steps passed (re-initialize)
  else if (m_simStep == 5 && vdjob->State->CurrentState->Number.value == 2) {
    mt.Monitoring->MachineTool->FeedOverride->Value = 0.0;
    mt.Monitoring->MachineTool->IsWarmUp = true;
    SwitchOnStacklightColor(UA_SignalColor::UA_SIGNALCOLOR_YELLOW);
    mt.Monitoring->MachineTool->OperationMode = UA_MachineOperationMode::UA_MACHINEOPERATIONMODE_SETUP;

    ActiveProgramStateMachine->SetState(0);
    VdJobProgramStateMachine->SetState(0);
    VdJobStateMachine->SetState(0);

    m_simStep = 0;
  }
}

void MRMachineTool::SwitchOnStacklightColor(UA_SignalColor color) {
  for (auto &light : mt.Monitoring->Stacklight->OrderedObjects.value) {
    if (light->SignalColor.value == color) {
      light->SignalOn = 1;
    } else {
      light->SignalOn = 0;
    }
  }
}
