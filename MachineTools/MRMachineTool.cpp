#include "MRMachineTool.hpp"
#include "../UmatiServerLib/BindRefl.hpp"
#include "../UmatiServerLib/OpcUaCondition.hpp"
#include "../UmatiServerLib/ServerHelper.hpp"
#include "../TypeDefinition/MachineTool/Alert.hpp"
#include "../TypeDefinition/MachineTool/NotificationEvent.hpp"
#include <open62541/types_generated.h>

MRMachineTool::MRMachineTool(UA_Server *pServer)
    : InstantiatedMachineTool(pServer)
{
    MachineName = "MRMachineTool";
    CreateObject();
}

void MRMachineTool::CreateObject()
{
    InstantiatedMachineTool::CreateObject();

    InstantiateIdentification();
    InstantiateMonitoring();
    InstantiateProduction();


    InstantiateOptional(mt.Notification->Messages, m_pServer, n);
    writeEventNotifier(m_pServer, mt.Notification->Messages.NodeId);
}

void MRMachineTool::InstantiateIdentification()
{
    InstantiatedMachineTool::InstantiateIdentification();

    mt.Identification->Location = "HVN_7";
    mt.Identification->SerialNumber = std::string{"070-101-098-14"};
    mt.Identification->Manufacturer = {"","ISW UA4MT Team"};
    mt.Identification->Model = {"","T3IUTH"};
    mt.Identification->ProductInstanceUri = "http://isw.uni-stuttgart.de/#T3IUTH/070-101-098-14";
}

void MRMachineTool::InstantiateMonitoring()
{
    InstantiateMonitoringMT();
    mt.Monitoring->MachineTool->OperationMode = UA_MachineOperationMode::UA_MACHINEOPERATIONMODE_SETUP;

    InstantiateOptional(mt.Monitoring->MachineTool->FeedOverride, m_pServer, n);
    InstantiateOptional(mt.Monitoring->MachineTool->IsWarmUp, m_pServer, n);
    mt.Monitoring->MachineTool->FeedOverride->Value = 0.0;
    mt.Monitoring->MachineTool->FeedOverride->EURange->high = 100.0;
    mt.Monitoring->MachineTool->FeedOverride->EURange->low = 0.0;
    mt.Monitoring->MachineTool->FeedOverride->EngineeringUnits->DisplayName = {"","%"};
    mt.Monitoring->MachineTool->IsWarmUp = true;

    InstantiateMonitoringStacklight({
            UA_SignalColor::UA_SIGNALCOLOR_GREEN,
            UA_SignalColor::UA_SIGNALCOLOR_YELLOW,
            UA_SignalColor::UA_SIGNALCOLOR_RED,
            });
    SwitchOnStacklightColor(UA_SignalColor::UA_SIGNALCOLOR_YELLOW);
}

void MRMachineTool::InstantiateProduction()
{
    InstantiateOptional(mt.Production->ActiveProgram->State, m_pServer, n);
    InstantiateOptional(mt.Production->ActiveProgram->JobNodeId, m_pServer, n);
    InstantiateOptional(mt.Production->ActiveProgram->JobIdentifier, m_pServer, n);
    InstantiateOptional(mt.Production->ProductionPlan, m_pServer, n);

    mt.Production->ActiveProgram->Name = "heart.nc";
    mt.Production->ActiveProgram->NumberInList = 0;
    mt.Production->ActiveProgram->State->CurrentState->Value = {"","Initializing"};
    mt.Production->ActiveProgram->State->CurrentState->Number = 0;
    mt.Production->ActiveProgram->State->CurrentState->Id = UA_NODEID_NUMERIC(nsFromUri(m_pServer, constants::NsMachineToolUri), UA_MACHINETOOLID_PRODUCTIONSTATEMACHINETYPE_INITIALIZING);

    auto &job = mt.Production->ProductionPlan->OrderedObjects.Add(m_pServer, n, {m_nsIndex, "VDJob"});
    job.Identifier = std::string("Valentines_Day");
    job.RunsCompleted = 0;
    job.RunsPlanned = 7;
    job.State->CurrentState->Value = {"","Initializing"};
    job.State->CurrentState->Number = 0;
    job.State->CurrentState->Id = UA_NODEID_NUMERIC(nsFromUri(m_pServer, constants::NsMachineToolUri), UA_MACHINETOOLID_PRODUCTIONSTATEMACHINETYPE_INITIALIZING);
    job.NumberInList = 0;
    auto &program = mt.Production->ProductionPlan->OrderedObjects.value.front()->ProductionPrograms->OrderedObjects.Add<machineTool::ProductionProgram_t>(m_pServer, n, {m_nsIndex, "heart.nc"});

    InstantiateOptional(program.State, m_pServer, n);
    program.Name = "heart.nc";
    program.State->CurrentState->Value = {"","Initializing"};
    program.State->CurrentState->Number = 0;
    program.State->CurrentState->Id = UA_NODEID_NUMERIC(nsFromUri(m_pServer, constants::NsMachineToolUri), UA_MACHINETOOLID_PRODUCTIONSTATEMACHINETYPE_INITIALIZING);

    mt.Production->ActiveProgram->JobNodeId = *mt.Production->ProductionPlan->OrderedObjects.value.front().NodeId.NodeId;
    mt.Production->ActiveProgram->JobIdentifier = job.Identifier;
}

void MRMachineTool::Simulate()
{
    ++m_simStep;

    // Job in initializing
    if (m_simStep == 30 and mt.Production->ProductionPlan->OrderedObjects.value.front()->State->CurrentState->Number.value == 0)
    {
        mt.Monitoring->MachineTool->FeedOverride->Value = 0.0;
        mt.Monitoring->MachineTool->IsWarmUp = false;
        SwitchOnStacklightColor(UA_SignalColor::UA_SIGNALCOLOR_GREEN);
        mt.Monitoring->MachineTool->OperationMode = UA_MachineOperationMode::UA_MACHINEOPERATIONMODE_AUTOMATIC;

        mt.Production->ProductionPlan->OrderedObjects.value.front()->State->CurrentState->Value = {"", "Running"};
        mt.Production->ProductionPlan->OrderedObjects.value.front()->State->CurrentState->Id = UA_NODEID_NUMERIC(nsFromUri(m_pServer, constants::NsMachineToolUri), UA_MACHINETOOLID_PRODUCTIONSTATEMACHINETYPE_RUNNING);
        mt.Production->ProductionPlan->OrderedObjects.value.front()->State->CurrentState->Number = 1;

        m_simStep = 0;
    }

    // Job in running
    else if (mt.Production->ProductionPlan->OrderedObjects.value.front()->State->CurrentState->Number.value == 1)
    {
        //Program is not running
        if (m_simStep == 2 and mt.Production->ActiveProgram->State->CurrentState->Number.value != 1)
        {
            mt.Production->ActiveProgram->State->CurrentState->Value = {"", "Initializing"};
            mt.Production->ActiveProgram->State->CurrentState->Id = UA_NODEID_NUMERIC(nsFromUri(m_pServer, constants::NsMachineToolUri), UA_MACHINETOOLID_PRODUCTIONSTATEMACHINETYPE_INITIALIZING);
            mt.Production->ActiveProgram->State->CurrentState->Number = 0;

            mt.Production->ProductionPlan->OrderedObjects.value.front()->ProductionPrograms->OrderedObjects.value.front()->State->CurrentState->Value = {"", "Initializing"};
            mt.Production->ProductionPlan->OrderedObjects.value.front()->ProductionPrograms->OrderedObjects.value.front()->State->CurrentState->Id = UA_NODEID_NUMERIC(nsFromUri(m_pServer, constants::NsMachineToolUri), UA_MACHINETOOLID_PRODUCTIONSTATEMACHINETYPE_INITIALIZING);
            mt.Production->ProductionPlan->OrderedObjects.value.front()->ProductionPrograms->OrderedObjects.value.front()->State->CurrentState->Number = 0;
        }
        else if (m_simStep == 3 and mt.Production->ActiveProgram->State->CurrentState->Number.value != 1)
        {
            mt.Production->ActiveProgram->State->CurrentState->Value = {"", "Running"};
            mt.Production->ActiveProgram->State->CurrentState->Id = UA_NODEID_NUMERIC(nsFromUri(m_pServer, constants::NsMachineToolUri), UA_MACHINETOOLID_PRODUCTIONSTATEMACHINETYPE_RUNNING);
            mt.Production->ActiveProgram->State->CurrentState->Number = 1;
            
            mt.Production->ProductionPlan->OrderedObjects.value.front()->ProductionPrograms->OrderedObjects.value.front()->State->CurrentState->Value = {"", "Running"};
            mt.Production->ProductionPlan->OrderedObjects.value.front()->ProductionPrograms->OrderedObjects.value.front()->State->CurrentState->Id = UA_NODEID_NUMERIC(nsFromUri(m_pServer, constants::NsMachineToolUri), UA_MACHINETOOLID_PRODUCTIONSTATEMACHINETYPE_RUNNING);
            mt.Production->ProductionPlan->OrderedObjects.value.front()->ProductionPrograms->OrderedObjects.value.front()->State->CurrentState->Number = 1;

            mt.Monitoring->MachineTool->FeedOverride->Value = 100.0;
        }

        //Program is running
        else if (m_simStep == 17 and mt.Production->ActiveProgram->State->CurrentState->Number.value == 1)
        {
            mt.Production->ActiveProgram->State->CurrentState->Value = {"", "Ended"};
            mt.Production->ActiveProgram->State->CurrentState->Id = UA_NODEID_NUMERIC(nsFromUri(m_pServer, constants::NsMachineToolUri), UA_MACHINETOOLID_PRODUCTIONSTATEMACHINETYPE_ENDED);
            mt.Production->ActiveProgram->State->CurrentState->Number = 2;

            mt.Production->ProductionPlan->OrderedObjects.value.front()->ProductionPrograms->OrderedObjects.value.front()->State->CurrentState->Value = {"", "Ended"};
            mt.Production->ProductionPlan->OrderedObjects.value.front()->ProductionPrograms->OrderedObjects.value.front()->State->CurrentState->Id = UA_NODEID_NUMERIC(nsFromUri(m_pServer, constants::NsMachineToolUri), UA_MACHINETOOLID_PRODUCTIONSTATEMACHINETYPE_ENDED);
            mt.Production->ProductionPlan->OrderedObjects.value.front()->ProductionPrograms->OrderedObjects.value.front()->State->CurrentState->Number = 2;

            mt.Monitoring->MachineTool->FeedOverride->Value = 0.0;
        }

        // one more run completed
        else if (m_simStep == 19 and mt.Production->ProductionPlan->OrderedObjects.value.front()->RunsCompleted.value < 7)
        {
            mt.Production->ProductionPlan->OrderedObjects.value.front()->RunsCompleted.value += 1;

            mt.Production->ProductionPlan->OrderedObjects.value.front()->State->CurrentState->Value = {"", "Running"};
            mt.Production->ProductionPlan->OrderedObjects.value.front()->State->CurrentState->Id = UA_NODEID_NUMERIC(nsFromUri(m_pServer, constants::NsMachineToolUri), UA_MACHINETOOLID_PRODUCTIONSTATEMACHINETYPE_RUNNING);
            mt.Production->ProductionPlan->OrderedObjects.value.front()->State->CurrentState->Number = 1;

            m_simStep = 0;
        }

        // all runs ended/overproduction
        else if (mt.Production->ProductionPlan->OrderedObjects.value.front()->RunsCompleted.value >= 7)
        {
            mt.Monitoring->MachineTool->FeedOverride->Value = 0.0;
            SwitchOnStacklightColor(UA_SignalColor::UA_SIGNALCOLOR_YELLOW);

            mt.Production->ProductionPlan->OrderedObjects.value.front()->RunsCompleted = 0;
            mt.Production->ProductionPlan->OrderedObjects.value.front()->State->CurrentState->Value = {"", "Ended"};
            mt.Production->ProductionPlan->OrderedObjects.value.front()->State->CurrentState->Id = UA_NODEID_NUMERIC(nsFromUri(m_pServer, constants::NsMachineToolUri), UA_MACHINETOOLID_PRODUCTIONSTATEMACHINETYPE_ENDED);
            mt.Production->ProductionPlan->OrderedObjects.value.front()->State->CurrentState->Number = 2;

            mt.Production->ActiveProgram->State->CurrentState->Value = {"", "Ended"};
            mt.Production->ActiveProgram->State->CurrentState->Id = UA_NODEID_NUMERIC(nsFromUri(m_pServer, constants::NsMachineToolUri), UA_MACHINETOOLID_PRODUCTIONSTATEMACHINETYPE_ENDED);
            mt.Production->ActiveProgram->State->CurrentState->Number = 2;

            mt.Production->ProductionPlan->OrderedObjects.value.front()->ProductionPrograms->OrderedObjects.value.front()->State->CurrentState->Value = {"", "Ended"};
            mt.Production->ProductionPlan->OrderedObjects.value.front()->ProductionPrograms->OrderedObjects.value.front()->State->CurrentState->Id = UA_NODEID_NUMERIC(nsFromUri(m_pServer, constants::NsMachineToolUri), UA_MACHINETOOLID_PRODUCTIONSTATEMACHINETYPE_ENDED);
            mt.Production->ProductionPlan->OrderedObjects.value.front()->ProductionPrograms->OrderedObjects.value.front()->State->CurrentState->Number = 2;
        }
    }

    // Job ended and 5 steps passed (re-initialize)
    else if (m_simStep == 5 and mt.Production->ProductionPlan->OrderedObjects.value.front()->State->CurrentState->Number.value == 2)
    {
        mt.Monitoring->MachineTool->FeedOverride->Value = 0.0;
        mt.Monitoring->MachineTool->IsWarmUp = true;
        SwitchOnStacklightColor(UA_SignalColor::UA_SIGNALCOLOR_YELLOW);
        mt.Monitoring->MachineTool->OperationMode = UA_MachineOperationMode::UA_MACHINEOPERATIONMODE_SETUP;

        mt.Production->ActiveProgram->State->CurrentState->Value = {"", "Initializing"};
        mt.Production->ActiveProgram->State->CurrentState->Id = UA_NODEID_NUMERIC(nsFromUri(m_pServer, constants::NsMachineToolUri), UA_MACHINETOOLID_PRODUCTIONSTATEMACHINETYPE_INITIALIZING);
        mt.Production->ActiveProgram->State->CurrentState->Number = 0;

        mt.Production->ProductionPlan->OrderedObjects.value.front()->ProductionPrograms->OrderedObjects.value.front()->State->CurrentState->Value = {"", "Initializing"};
        mt.Production->ProductionPlan->OrderedObjects.value.front()->ProductionPrograms->OrderedObjects.value.front()->State->CurrentState->Id = UA_NODEID_NUMERIC(nsFromUri(m_pServer, constants::NsMachineToolUri), UA_MACHINETOOLID_PRODUCTIONSTATEMACHINETYPE_INITIALIZING);
        mt.Production->ProductionPlan->OrderedObjects.value.front()->ProductionPrograms->OrderedObjects.value.front()->State->CurrentState->Number = 0;

        mt.Production->ProductionPlan->OrderedObjects.value.front()->State->CurrentState->Value = {"", "Initializing"};
        mt.Production->ProductionPlan->OrderedObjects.value.front()->State->CurrentState->Id = UA_NODEID_NUMERIC(nsFromUri(m_pServer, constants::NsMachineToolUri), UA_MACHINETOOLID_PRODUCTIONSTATEMACHINETYPE_INITIALIZING);
        mt.Production->ProductionPlan->OrderedObjects.value.front()->State->CurrentState->Number = 0;

        m_simStep = 0;
    }

}

void MRMachineTool::SwitchOnStacklightColor(UA_SignalColor color)
{
    for (auto &light : mt.Monitoring->Stacklight->OrderedObjects.value)
    {
        if (light->SignalColor.value == color)
        {
            light->SignalOn = 1;
        } 
        else
        {
            light->SignalOn = 0;
        }
    }
}
