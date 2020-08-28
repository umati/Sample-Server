#include "MRMachineTool.hpp"
#include "../UmatiServerLib/BindRefl.hpp"
#include "../UmatiServerLib/OpcUaCondition.hpp"
#include "../UmatiServerLib/ServerHelper.hpp"
#include "../TypeDefinition/MachineTool/Alert.hpp"
#include <open62541/types_generated.h>
#include "../TypeDefinition/MachineTool/ProductionJobTransitionEvent.hpp"

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
    InstantiateOptional(mt.Identification->YearOfConstruction, m_pServer, n);
    InstantiateOptional(mt.Identification->SoftwareRevision, m_pServer, n);
    InstantiateOptional(mt.Identification->ProductCode, m_pServer, n);

    mt.Identification->Location = "AMB 0 1/N 48.781340 E 9.165731";
    mt.Identification->SerialNumber = std::string{"070-101-098-14"};
    mt.Identification->Manufacturer = {"","ISW UA4MT Team"};
    mt.Identification->Model = {"","T3IUTH"};
    mt.Identification->ProductInstanceUri = "http://isw.uni-stuttgart.de/#T3IUTH/070-101-098-14";
    mt.Identification->ProductCode = "2684-rga-t842574-b7";
    mt.Identification->YearOfConstruction = 2020;
    mt.Identification->SoftwareRevision= "4.30.2";
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
    n.Remove(mt.Monitoring->Stacklight->NodeVersion.NodeId);
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
    mt.Production->ActiveProgram->State->CurrentState->Value = {"en","Initializing"};
    mt.Production->ActiveProgram->State->CurrentState->Number = 0;
    mt.Production->ActiveProgram->State->CurrentState->Id = UA_NODEID_NUMERIC(nsFromUri(m_pServer, constants::NsMachineToolUri), UA_MACHINETOOLID_PRODUCTIONSTATEMACHINETYPE_INITIALIZING);

    auto &job = mt.Production->ProductionPlan->OrderedObjects.Add(m_pServer, n, {m_nsIndex, "VDJob"});
    job.Identifier = std::string("VDJob");
    job.RunsCompleted = 0;
    job.RunsPlanned->Value = 7;
    job.RunsPlanned->IsValid = true;
    job.State->CurrentState->Value = {"en","Initializing"};
    job.State->CurrentState->Number = 0;
    job.State->CurrentState->Id = UA_NODEID_NUMERIC(nsFromUri(m_pServer, constants::NsMachineToolUri), UA_MACHINETOOLID_PRODUCTIONSTATEMACHINETYPE_INITIALIZING);
    job.NumberInList = 0;
    auto &program = mt.Production->ProductionPlan->OrderedObjects.value.front()->ProductionPrograms->OrderedObjects.Add<machineTool::ProductionProgram_t>(m_pServer, n, {m_nsIndex, "heart.nc"});

    InstantiateOptional(program.State, m_pServer, n);
    program.Name = "heart.nc";
    program.State->CurrentState->Value = {"en","Initializing"};
    program.State->CurrentState->Number = 0;
    program.State->CurrentState->Id = UA_NODEID_NUMERIC(nsFromUri(m_pServer, constants::NsMachineToolUri), UA_MACHINETOOLID_PRODUCTIONSTATEMACHINETYPE_INITIALIZING);

    mt.Production->ActiveProgram->JobNodeId = *mt.Production->ProductionPlan->OrderedObjects.value.front().NodeId.NodeId;
    mt.Production->ActiveProgram->JobIdentifier = job.Identifier;

    writeEventNotifier(m_pServer, mt.Production->ActiveProgram->State.NodeId);
    writeEventNotifier(m_pServer, job.State.NodeId);
    writeEventNotifier(m_pServer, program.State.NodeId);
}

void MRMachineTool::Simulate()
{
    ++m_simStep;

    auto &vdjob = mt.Production->ProductionPlan->OrderedObjects.value.front();
    auto &vdprog = vdjob->ProductionPrograms->OrderedObjects.value.front();
    auto &aprog = mt.Production->ActiveProgram;

    // Job in initializing
    if (m_simStep == 30 and vdjob->State->CurrentState->Number.value == 0)
    {
        mt.Monitoring->MachineTool->FeedOverride->Value = 0.0;
        mt.Monitoring->MachineTool->IsWarmUp = false;
        SwitchOnStacklightColor(UA_SignalColor::UA_SIGNALCOLOR_GREEN);
        mt.Monitoring->MachineTool->OperationMode = UA_MachineOperationMode::UA_MACHINEOPERATIONMODE_AUTOMATIC;

        vdjob->State->CurrentState->Value = {"en", "Running"};
        vdjob->State->CurrentState->Id = UA_NODEID_NUMERIC(nsFromUri(m_pServer, constants::NsMachineToolUri), UA_MACHINETOOLID_PRODUCTIONSTATEMACHINETYPE_RUNNING);
        vdjob->State->CurrentState->Number = 1;

        m_simStep = 0;

        machineTool::ProductionJobTransitionEvent_t jtevent;
        /**
        std::stringstream ss;
        ss << "Transition to Running triggered";
        jtevent.Message = {"en", ss.str()};
        **/
        jtevent.Severity = 20;
        /**
        jtevent.SourceName = "MRMachineTool";
        jtevent.Identifier = vdjob->Identifier.value;
        jtevent.RunsPlanned->Value = vdjob->RunsPlanned->Value.value;
        jtevent.RunsPlanned->IsValid = vdjob->RunsPlanned->IsValid.value;
        jtevent.RunsCompleted = vdjob->RunsCompleted.value;
        jtevent.Transition->Value = {"en","InitializingToRunning"}; 
        jtevent.Transition->Id = UA_NODEID_NUMERIC(nsFromUri(m_pServer, constants::NsMachineToolUri), UA_MACHINETOOLID_PRODUCTIONSTATEMACHINETYPE_INITIALIZINGTORUNNING);
        jtevent.FromState->Value = {"en","Initializing"};
        jtevent.FromState->Id = UA_NODEID_NUMERIC(nsFromUri(m_pServer, constants::NsMachineToolUri), UA_MACHINETOOLID_PRODUCTIONSTATEMACHINETYPE_INITIALIZING);
        jtevent.ToState->Value = {"en","Running"}; 
        jtevent.ToState->Id = UA_NODEID_NUMERIC(nsFromUri(m_pServer, constants::NsMachineToolUri), UA_MACHINETOOLID_PRODUCTIONSTATEMACHINETYPE_RUNNING);
        **/
        OpcUaEvent ev(jtevent, m_pServer, vdjob->State.NodeId);
        std::cout << vdjob->State.NodeId;
/**
    machineTool::NotificationEvent_t notification;
    notification.Identifier = "Custom Event";
    std::stringstream ss;
    ss << "Full MT Msg " << m_simStep;
    notification.Message = {"en", ss.str()};
    notification.Severity = (m_simStep - 8) % 300;
    notification.SourceName = "FullMachineTool";
    OpcUaEvent ev(notification, m_pServer, mt.Notification->Messages.NodeId);
    **/
    }

    // Job in running
    else if (vdjob->State->CurrentState->Number.value == 1)
    {
        //Program is not running
        if (m_simStep == 2 and aprog->State->CurrentState->Number.value != 1)
        {
            aprog->State->CurrentState->Value = {"en", "Initializing"};
            aprog->State->CurrentState->Id = UA_NODEID_NUMERIC(nsFromUri(m_pServer, constants::NsMachineToolUri), UA_MACHINETOOLID_PRODUCTIONSTATEMACHINETYPE_INITIALIZING);
            aprog->State->CurrentState->Number = 0;

            vdprog->State->CurrentState->Value = {"en", "Initializing"};
            vdprog->State->CurrentState->Id = UA_NODEID_NUMERIC(nsFromUri(m_pServer, constants::NsMachineToolUri), UA_MACHINETOOLID_PRODUCTIONSTATEMACHINETYPE_INITIALIZING);
            vdprog->State->CurrentState->Number = 0;

        }
        else if (m_simStep == 3 and aprog->State->CurrentState->Number.value != 1)
        {
            aprog->State->CurrentState->Value = {"en", "Running"};
            aprog->State->CurrentState->Id = UA_NODEID_NUMERIC(nsFromUri(m_pServer, constants::NsMachineToolUri), UA_MACHINETOOLID_PRODUCTIONSTATEMACHINETYPE_RUNNING);
            aprog->State->CurrentState->Number = 1;
            
            vdprog->State->CurrentState->Value = {"en", "Running"};
            vdprog->State->CurrentState->Id = UA_NODEID_NUMERIC(nsFromUri(m_pServer, constants::NsMachineToolUri), UA_MACHINETOOLID_PRODUCTIONSTATEMACHINETYPE_RUNNING);
            vdprog->State->CurrentState->Number = 1;

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

        //Program is running
        else if (m_simStep == 17 and aprog->State->CurrentState->Number.value == 1)
        {
            aprog->State->CurrentState->Value = {"en", "Ended"};
            aprog->State->CurrentState->Id = UA_NODEID_NUMERIC(nsFromUri(m_pServer, constants::NsMachineToolUri), UA_MACHINETOOLID_PRODUCTIONSTATEMACHINETYPE_ENDED);
            aprog->State->CurrentState->Number = 2;

            vdprog->State->CurrentState->Value = {"en", "Ended"};
            vdprog->State->CurrentState->Id = UA_NODEID_NUMERIC(nsFromUri(m_pServer, constants::NsMachineToolUri), UA_MACHINETOOLID_PRODUCTIONSTATEMACHINETYPE_ENDED);
            vdprog->State->CurrentState->Number = 2;

            mt.Monitoring->MachineTool->FeedOverride->Value = 0.0;
        }

        // one more run completed
        else if (m_simStep == 19 and vdjob->RunsCompleted.value < 7)
        {
            vdjob->RunsCompleted.value += 1;

            vdjob->State->CurrentState->Value = {"en", "Running"};
            vdjob->State->CurrentState->Id = UA_NODEID_NUMERIC(nsFromUri(m_pServer, constants::NsMachineToolUri), UA_MACHINETOOLID_PRODUCTIONSTATEMACHINETYPE_RUNNING);
            vdjob->State->CurrentState->Number = 1;

            m_simStep = 0;
        }

        // all runs ended/overproduction
        else if (vdjob->RunsCompleted.value >= 7)
        {
            mt.Monitoring->MachineTool->FeedOverride->Value = 0.0;
            SwitchOnStacklightColor(UA_SignalColor::UA_SIGNALCOLOR_YELLOW);

            vdjob->RunsCompleted = 0;
            vdjob->State->CurrentState->Value = {"en", "Ended"};
            vdjob->State->CurrentState->Id = UA_NODEID_NUMERIC(nsFromUri(m_pServer, constants::NsMachineToolUri), UA_MACHINETOOLID_PRODUCTIONSTATEMACHINETYPE_ENDED);
            vdjob->State->CurrentState->Number = 2;

            aprog->State->CurrentState->Value = {"en", "Ended"};
            aprog->State->CurrentState->Id = UA_NODEID_NUMERIC(nsFromUri(m_pServer, constants::NsMachineToolUri), UA_MACHINETOOLID_PRODUCTIONSTATEMACHINETYPE_ENDED);
            aprog->State->CurrentState->Number = 2;

            vdprog->State->CurrentState->Value = {"en", "Ended"};
            vdprog->State->CurrentState->Id = UA_NODEID_NUMERIC(nsFromUri(m_pServer, constants::NsMachineToolUri), UA_MACHINETOOLID_PRODUCTIONSTATEMACHINETYPE_ENDED);
            vdprog->State->CurrentState->Number = 2;
        }
    }

    // Job ended and 5 steps passed (re-initialize)
    else if (m_simStep == 5 and vdjob->State->CurrentState->Number.value == 2)
    {
        mt.Monitoring->MachineTool->FeedOverride->Value = 0.0;
        mt.Monitoring->MachineTool->IsWarmUp = true;
        SwitchOnStacklightColor(UA_SignalColor::UA_SIGNALCOLOR_YELLOW);
        mt.Monitoring->MachineTool->OperationMode = UA_MachineOperationMode::UA_MACHINEOPERATIONMODE_SETUP;

        aprog->State->CurrentState->Value = {"en", "Initializing"};
        aprog->State->CurrentState->Id = UA_NODEID_NUMERIC(nsFromUri(m_pServer, constants::NsMachineToolUri), UA_MACHINETOOLID_PRODUCTIONSTATEMACHINETYPE_INITIALIZING);
        aprog->State->CurrentState->Number = 0;

        vdprog->State->CurrentState->Value = {"en", "Initializing"};
        vdprog->State->CurrentState->Id = UA_NODEID_NUMERIC(nsFromUri(m_pServer, constants::NsMachineToolUri), UA_MACHINETOOLID_PRODUCTIONSTATEMACHINETYPE_INITIALIZING);
        vdprog->State->CurrentState->Number = 0;

        vdjob->State->CurrentState->Value = {"en", "Initializing"};
        vdjob->State->CurrentState->Id = UA_NODEID_NUMERIC(nsFromUri(m_pServer, constants::NsMachineToolUri), UA_MACHINETOOLID_PRODUCTIONSTATEMACHINETYPE_INITIALIZING);
        vdjob->State->CurrentState->Number = 0;

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
