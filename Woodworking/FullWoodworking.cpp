/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2021 (c) Christoph Ruckstetter, Michael Weinig AG
 */

#include "FullWoodworking.hpp"

#include <open62541/server.h>
#include <open62541/types_generated.h>

#include "../TypeDefinition/Woodworking/WwBaseEvent.hpp"
#include "../UmatiServerLib/BindRefl.hpp"
#include "../UmatiServerLib/OpcUaCondition.hpp"
#include "../UmatiServerLib/ServerHelper.hpp"

FullWoodworking::FullWoodworking(UA_Server *pServer) : InstantiatedWoodworking(pServer) {
  MachineName = "FullWoodworking";
  CreateObject();
}

void FullWoodworking::CreateObject() {
  InstantiatedWoodworking::CreateObject();
  InstantiateIdentification();
  InstantiateMachineOverview();
  InstantiateMachineFlags();
  InstantiateMachineValues();
  InstantiateEventsDispatcher();
  InstantiateManufacturerSpecific();
  InstantiateSubunits();
}

void FullWoodworking::InstantiateIdentification() {
  InstantiatedWoodworking::InstantiateIdentification();

  InstantiateOptional(ww.Identification->LocationPlant, m_pServer, n);
  InstantiateOptional(ww.Identification->LocationGPS, m_pServer, n);
  InstantiateOptional(ww.Identification->CustomerCompanyName, m_pServer, n);
  InstantiateOptional(ww.Identification->ManufacturerUri, m_pServer, n);
  InstantiateOptional(ww.Identification->ProductCode, m_pServer, n);
  InstantiateOptional(ww.Identification->HardwareRevision, m_pServer, n);
  InstantiateOptional(ww.Identification->SoftwareRevision, m_pServer, n);
  InstantiateOptional(ww.Identification->MonthOfConstruction, m_pServer, n);
  InstantiateOptional(ww.Identification->InitialOperationDate, m_pServer, n);
  InstantiateOptional(ww.Identification->AssetId, m_pServer, n);
  InstantiateOptional(ww.Identification->ComponentName, m_pServer, n);
  InstantiateOptional(ww.Identification->Location, m_pServer, n);

  ww.Identification->LocationPlant = "TBB";
  ww.Identification->LocationGPS = "49.628661 9.654903";
  ww.Identification->CustomerCompanyName = {"", "Weinig Expo Center"};
  ww.Identification->ManufacturerUri = "https://www.weinig.com";
  ww.Identification->ProductCode = "123456";
  ww.Identification->HardwareRevision = "1.0";
  ww.Identification->SoftwareRevision = "1.1";
  ww.Identification->MonthOfConstruction = 1;
  ww.Identification->InitialOperationDate = std::chrono::system_clock::now();
  ww.Identification->AssetId = "123456";
  ww.Identification->ComponentName = {"", "Machine"};
  ww.Identification->Location = "BXP 9 9502/VIRTUAL 0 0/N 49.628661 E 9.654903";
}

void FullWoodworking::InstantiateMachineFlags() {
  InstantiatedWoodworking::InstantiateMachineFlags();

  InstantiateOptional(ww.State->Machine->Flags->AirPresent, m_pServer, n);
  InstantiateOptional(ww.State->Machine->Flags->DustChipSuction, m_pServer, n);
  InstantiateOptional(ww.State->Machine->Flags->Safety, m_pServer, n);
  InstantiateOptional(ww.State->Machine->Flags->Remote, m_pServer, n);
  InstantiateOptional(ww.State->Machine->Flags->WorkpiecePresent, m_pServer, n);
  InstantiateOptional(ww.State->Machine->Flags->Moving, m_pServer, n);
  InstantiateOptional(ww.State->Machine->Flags->Hold, m_pServer, n);
  InstantiateOptional(ww.State->Machine->Flags->RecipeInSetup, m_pServer, n);
  InstantiateOptional(ww.State->Machine->Flags->RecipeInHold, m_pServer, n);
  InstantiateOptional(ww.State->Machine->Flags->ManualActivityRequired, m_pServer, n);
  InstantiateOptional(ww.State->Machine->Flags->LoadingEnabled, m_pServer, n);
  InstantiateOptional(ww.State->Machine->Flags->WaitUnload, m_pServer, n);
  InstantiateOptional(ww.State->Machine->Flags->WaitLoad, m_pServer, n);
  InstantiateOptional(ww.State->Machine->Flags->EnergySaving, m_pServer, n);
  InstantiateOptional(ww.State->Machine->Flags->ExternalEmergency, m_pServer, n);
  InstantiateOptional(ww.State->Machine->Flags->MaintenanceRequired, m_pServer, n);
  InstantiateOptional(ww.State->Machine->Flags->FeedRuns, m_pServer, n);

  ww.State->Machine->Flags->AirPresent = true;
  ww.State->Machine->Flags->DustChipSuction = true;
  ww.State->Machine->Flags->Safety = false;
  ww.State->Machine->Flags->Remote = false;
  ww.State->Machine->Flags->WorkpiecePresent = true;
  ww.State->Machine->Flags->Moving = true;
  ww.State->Machine->Flags->Hold = false;
  ww.State->Machine->Flags->RecipeInSetup = false;
  ww.State->Machine->Flags->RecipeInHold = false;
  ww.State->Machine->Flags->ManualActivityRequired = false;
  ww.State->Machine->Flags->LoadingEnabled = false;
  ww.State->Machine->Flags->WaitUnload = false;
  ww.State->Machine->Flags->WaitLoad = false;
  ww.State->Machine->Flags->EnergySaving = false;
  ww.State->Machine->Flags->ExternalEmergency = false;
  ww.State->Machine->Flags->MaintenanceRequired = false;
  ww.State->Machine->Flags->FeedRuns = true;
}

void FullWoodworking::InstantiateMachineOverview() { InstantiatedWoodworking::InstantiateMachineOverview(); }

void FullWoodworking::InstantiateMachineValues() {
  InstantiateOptional(ww.State->Machine->Values, m_pServer, n);

  InstantiateOptional(ww.State->Machine->Values->AxisOverride, m_pServer, n);
  InstantiateOptional(ww.State->Machine->Values->SpindleOverride, m_pServer, n);
  InstantiateOptional(ww.State->Machine->Values->FeedSpeed, m_pServer, n);
  InstantiateOptional(ww.State->Machine->Values->ActualCycle, m_pServer, n);
  InstantiateOptional(ww.State->Machine->Values->AbsoluteMachineOffTime, m_pServer, n);
  InstantiateOptional(ww.State->Machine->Values->AbsoluteStandbyTime, m_pServer, n);
  InstantiateOptional(ww.State->Machine->Values->RelativeStandbyTime, m_pServer, n);
  InstantiateOptional(ww.State->Machine->Values->AbsoluteReadyTime, m_pServer, n);
  InstantiateOptional(ww.State->Machine->Values->RelativeReadyTime, m_pServer, n);
  InstantiateOptional(ww.State->Machine->Values->AbsoluteWorkingTime, m_pServer, n);
  InstantiateOptional(ww.State->Machine->Values->RelativeWorkingTime, m_pServer, n);
  InstantiateOptional(ww.State->Machine->Values->AbsoluteErrorTime, m_pServer, n);
  InstantiateOptional(ww.State->Machine->Values->RelativeErrorTime, m_pServer, n);
  InstantiateOptional(ww.State->Machine->Values->AbsoluteMachineOnTime, m_pServer, n);
  InstantiateOptional(ww.State->Machine->Values->RelativeMachineOnTime, m_pServer, n);
  InstantiateOptional(ww.State->Machine->Values->AbsolutePowerPresentTime, m_pServer, n);
  InstantiateOptional(ww.State->Machine->Values->RelativePowerPresentTime, m_pServer, n);
  InstantiateOptional(ww.State->Machine->Values->AbsoluteProductionTime, m_pServer, n);
  InstantiateOptional(ww.State->Machine->Values->RelativeProductionTime, m_pServer, n);
  InstantiateOptional(ww.State->Machine->Values->AbsoluteProductionWithoutWorkpieceTime, m_pServer, n);
  InstantiateOptional(ww.State->Machine->Values->RelativeProductionWithoutWorkpieceTime, m_pServer, n);
  InstantiateOptional(ww.State->Machine->Values->AbsoluteProductionWaitWorkpieceTime, m_pServer, n);
  InstantiateOptional(ww.State->Machine->Values->RelativeProductionWaitWorkpieceTime, m_pServer, n);
  InstantiateOptional(ww.State->Machine->Values->AbsoluteRunsGood, m_pServer, n);
  InstantiateOptional(ww.State->Machine->Values->RelativeRunsGood, m_pServer, n);
  InstantiateOptional(ww.State->Machine->Values->AbsoluteRunsTotal, m_pServer, n);
  InstantiateOptional(ww.State->Machine->Values->RelativeRunsTotal, m_pServer, n);
  InstantiateOptional(ww.State->Machine->Values->AbsoluteRunsAborted, m_pServer, n);
  InstantiateOptional(ww.State->Machine->Values->RelativeRunsAborted, m_pServer, n);
  InstantiateOptional(ww.State->Machine->Values->AbsoluteLength, m_pServer, n);
  InstantiateOptional(ww.State->Machine->Values->RelativeLength, m_pServer, n);
  InstantiateOptional(ww.State->Machine->Values->AbsolutePiecesIn, m_pServer, n);
  InstantiateOptional(ww.State->Machine->Values->RelativePiecesIn, m_pServer, n);
  InstantiateOptional(ww.State->Machine->Values->AbsolutePiecesOut, m_pServer, n);
  InstantiateOptional(ww.State->Machine->Values->RelativePiecesOut, m_pServer, n);

  ww.State->Machine->Values->AxisOverride = 100;
  ww.State->Machine->Values->SpindleOverride = 100;
  ww.State->Machine->Values->FeedSpeed = 250;
  ww.State->Machine->Values->ActualCycle = 1245;
  ww.State->Machine->Values->AbsoluteMachineOffTime = 53124;
  ww.State->Machine->Values->AbsoluteStandbyTime = 235231;
  ww.State->Machine->Values->RelativeStandbyTime = 1234;
  ww.State->Machine->Values->AbsoluteReadyTime = 321425;
  ww.State->Machine->Values->RelativeReadyTime = 54321;
  ww.State->Machine->Values->AbsoluteWorkingTime = 146782;
  ww.State->Machine->Values->RelativeWorkingTime = 5321;
  ww.State->Machine->Values->AbsoluteErrorTime = 315321;
  ww.State->Machine->Values->RelativeErrorTime = 3215;
  ww.State->Machine->Values->AbsoluteMachineOnTime = 54763;
  ww.State->Machine->Values->RelativeMachineOnTime = 7534;
  ww.State->Machine->Values->AbsolutePowerPresentTime = 75437;
  ww.State->Machine->Values->RelativePowerPresentTime = 6356;
  ww.State->Machine->Values->AbsoluteProductionTime = 342534;
  ww.State->Machine->Values->RelativeProductionTime = 54325;
  ww.State->Machine->Values->AbsoluteProductionWithoutWorkpieceTime = 34626;
  ww.State->Machine->Values->RelativeProductionWithoutWorkpieceTime = 754;
  ww.State->Machine->Values->AbsoluteProductionWaitWorkpieceTime = 52879;
  ww.State->Machine->Values->RelativeProductionWaitWorkpieceTime = 325;
  ww.State->Machine->Values->AbsoluteRunsGood = 7526;
  ww.State->Machine->Values->RelativeRunsGood = 53;
  ww.State->Machine->Values->AbsoluteRunsTotal = 7543;
  ww.State->Machine->Values->RelativeRunsTotal = 35;
  ww.State->Machine->Values->AbsoluteRunsAborted = 75432;
  ww.State->Machine->Values->RelativeRunsAborted = 5;
  ww.State->Machine->Values->AbsoluteLength = 32642;
  ww.State->Machine->Values->RelativeLength = 543;
  ww.State->Machine->Values->AbsolutePiecesIn = 7543;
  ww.State->Machine->Values->RelativePiecesIn = 2534;
  ww.State->Machine->Values->AbsolutePiecesOut = 7432;
  ww.State->Machine->Values->RelativePiecesOut = 234;
}

void FullWoodworking::InstantiateEventsDispatcher() {
  InstantiateOptional(ww.Events, m_pServer, n);
  writeEventNotifier(m_pServer, ww.Events.NodeId);
}

void FullWoodworking::InstantiateManufacturerSpecific() {
  InstantiateOptional(ww.ManufacturerSpecific, m_pServer, n);
  InstantiateOptional(ww.ManufacturerSpecific->LastProgramName, m_pServer, n);
  ww.ManufacturerSpecific->LastProgramName = "A1234";
}

void FullWoodworking::InstantiateSubunits() {
  InstantiateOptional(ww.State->SubUnits, m_pServer, n);
  // auto spindle = ww.State->SubUnits->SubUnit.Add<woodworking::IWwBaseState_t>(m_pServer, n, {m_nsIndex, "Spindle 1"});
  // spindle.Overview->CurrentMode = UA_WwUnitModeEnumeration::UA_WWUNITMODEENUMERATION_MANUAL;
  // spindle.Overview->CurrentState = UA_WwUnitStateEnumeration::UA_WWUNITSTATEENUMERATION_WORKING;

  InstantiateOptional(ww.State->SubUnits->SubUnit, m_pServer, n);
  ww.State->SubUnits->SubUnit->Overview->CurrentState = UA_WwUnitStateEnumeration::UA_WWUNITSTATEENUMERATION_READY;
  ww.State->SubUnits->SubUnit->Overview->CurrentMode = UA_WwUnitModeEnumeration::UA_WWUNITMODEENUMERATION_MANUAL;
}

void FullWoodworking::Simulate() {
  ++m_simStep;

  switch (m_simStep % 12) {
    case 0: {
      ww.State->Machine->Overview->CurrentMode = UA_WwUnitModeEnumeration::UA_WWUNITMODEENUMERATION_SLEEP;
      break;
    }
    case 2: {
      ww.State->Machine->Overview->CurrentMode = UA_WwUnitModeEnumeration::UA_WWUNITMODEENUMERATION_SETUP;
      break;
    }
    case 4: {
      ww.State->Machine->Overview->CurrentMode = UA_WwUnitModeEnumeration::UA_WWUNITMODEENUMERATION_MANUAL;
      break;
    }
    case 6: {
      ww.State->Machine->Overview->CurrentMode = UA_WwUnitModeEnumeration::UA_WWUNITMODEENUMERATION_SEMIAUTOMATIC;
      break;
    }
    case 8: {
      ww.State->Machine->Overview->CurrentMode = UA_WwUnitModeEnumeration::UA_WWUNITMODEENUMERATION_AUTOMATIC;
      break;
    }
    case 10: {
      ww.State->Machine->Overview->CurrentMode = UA_WwUnitModeEnumeration::UA_WWUNITMODEENUMERATION_OTHER;
      break;
    }
  }

  switch (m_simStep % 10) {
    case 0: {
      ww.State->Machine->Overview->CurrentState = UA_WwUnitStateEnumeration::UA_WWUNITSTATEENUMERATION_OFFLINE;
      break;
    }
    case 2: {
      ww.State->Machine->Overview->CurrentState = UA_WwUnitStateEnumeration::UA_WWUNITSTATEENUMERATION_STANDBY;
      break;
    }
    case 4: {
      ww.State->Machine->Overview->CurrentState = UA_WwUnitStateEnumeration::UA_WWUNITSTATEENUMERATION_READY;
      break;
    }
    case 6: {
      ww.State->Machine->Overview->CurrentState = UA_WwUnitStateEnumeration::UA_WWUNITSTATEENUMERATION_WORKING;

      break;
    }
    case 8: {
      ww.State->Machine->Overview->CurrentState = UA_WwUnitStateEnumeration::UA_WWUNITSTATEENUMERATION_ERROR;
      break;
    }
  }

  ww.State->Machine->Values->ActualCycle = m_simStep;
  switch (ww.State->Machine->Overview->CurrentState.value) {
    case UA_WwUnitStateEnumeration::UA_WWUNITSTATEENUMERATION_OFFLINE: {
      ww.State->Machine->Values->AbsoluteMachineOffTime = ww.State->Machine->Values->AbsoluteMachineOffTime.value + 1;
      ww.State->Machine->Flags->RecipeInRun = false;
      ww.State->Machine->Values->FeedSpeed = 0;
      break;
    }
    case UA_WwUnitStateEnumeration::UA_WWUNITSTATEENUMERATION_STANDBY: {
      ww.State->Machine->Values->AbsoluteStandbyTime = ww.State->Machine->Values->AbsoluteStandbyTime.value + 1;
      ww.State->Machine->Values->RelativeStandbyTime = ww.State->Machine->Values->RelativeStandbyTime.value + 1;
      ww.State->Machine->Values->AbsoluteMachineOnTime = ww.State->Machine->Values->AbsoluteMachineOnTime.value + 1;
      ww.State->Machine->Values->RelativeMachineOnTime = ww.State->Machine->Values->RelativeMachineOnTime.value + 1;
      ww.State->Machine->Values->AbsolutePowerPresentTime = ww.State->Machine->Values->AbsolutePowerPresentTime.value + 1;
      ww.State->Machine->Values->RelativePowerPresentTime = ww.State->Machine->Values->RelativePowerPresentTime.value + 1;
      ww.State->Machine->Flags->RecipeInRun = false;
      ww.State->Machine->Values->FeedSpeed = 0;
      break;
    }
    case UA_WwUnitStateEnumeration::UA_WWUNITSTATEENUMERATION_READY: {
      ww.State->Machine->Values->AbsoluteReadyTime = ww.State->Machine->Values->AbsoluteReadyTime.value + 1;
      ww.State->Machine->Values->RelativeReadyTime = ww.State->Machine->Values->RelativeReadyTime.value + 1;
      ww.State->Machine->Values->AbsoluteMachineOnTime = ww.State->Machine->Values->AbsoluteMachineOnTime.value + 1;
      ww.State->Machine->Values->RelativeMachineOnTime = ww.State->Machine->Values->RelativeMachineOnTime.value + 1;
      ww.State->Machine->Values->AbsolutePowerPresentTime = ww.State->Machine->Values->AbsolutePowerPresentTime.value + 1;
      ww.State->Machine->Values->RelativePowerPresentTime = ww.State->Machine->Values->RelativePowerPresentTime.value + 1;
      ww.State->Machine->Flags->RecipeInRun = false;
      ww.State->Machine->Values->FeedSpeed = 0;
      break;
    }
    case UA_WwUnitStateEnumeration::UA_WWUNITSTATEENUMERATION_WORKING: {
      ww.State->Machine->Flags->RecipeInRun = true;
      ww.State->Machine->Values->AbsoluteWorkingTime = ww.State->Machine->Values->AbsoluteWorkingTime.value + 1;
      ww.State->Machine->Values->RelativeWorkingTime = ww.State->Machine->Values->RelativeWorkingTime.value + 1;
      ww.State->Machine->Values->AbsoluteMachineOnTime = ww.State->Machine->Values->AbsoluteMachineOnTime.value + 1;
      ww.State->Machine->Values->RelativeMachineOnTime = ww.State->Machine->Values->RelativeMachineOnTime.value + 1;
      ww.State->Machine->Values->AbsolutePowerPresentTime = ww.State->Machine->Values->AbsolutePowerPresentTime.value + 1;
      ww.State->Machine->Values->RelativePowerPresentTime = ww.State->Machine->Values->RelativePowerPresentTime.value + 1;
      ww.State->Machine->Values->AbsoluteProductionTime = ww.State->Machine->Values->AbsoluteProductionTime.value + 1;
      ww.State->Machine->Values->RelativeProductionTime = ww.State->Machine->Values->RelativeProductionTime.value + 1;
      ww.State->Machine->Values->FeedSpeed = 250;
      ww.State->Machine->Values->AbsoluteRunsGood = ww.State->Machine->Values->AbsoluteRunsGood.value + 1;
      ww.State->Machine->Values->RelativeRunsGood = ww.State->Machine->Values->RelativeRunsGood.value + 1;
      ww.State->Machine->Values->AbsoluteRunsTotal = ww.State->Machine->Values->AbsoluteRunsTotal.value + 1;
      ww.State->Machine->Values->RelativeRunsTotal = ww.State->Machine->Values->RelativeRunsTotal.value + 1;
      ww.State->Machine->Values->AbsoluteLength = ww.State->Machine->Values->AbsoluteLength.value + (ww.State->Machine->Values->FeedSpeed.value / 60);
      ww.State->Machine->Values->RelativeLength = ww.State->Machine->Values->RelativeLength.value + (ww.State->Machine->Values->FeedSpeed.value / 60);
      if (m_simStep % 5 == 0) {
        ww.State->Machine->Values->AbsolutePiecesIn = ww.State->Machine->Values->AbsolutePiecesIn.value + 1;
        ww.State->Machine->Values->RelativePiecesIn = ww.State->Machine->Values->RelativePiecesIn.value + 1;
      } else if (m_simStep % 5 == 4) {
        ww.State->Machine->Values->AbsolutePiecesOut = ww.State->Machine->Values->AbsolutePiecesOut.value + 1;
        ww.State->Machine->Values->RelativePiecesOut = ww.State->Machine->Values->RelativePiecesOut.value + 1;
      }
      break;
    }
    case UA_WwUnitStateEnumeration::UA_WWUNITSTATEENUMERATION_ERROR: {
      ww.State->Machine->Values->AbsoluteErrorTime = ww.State->Machine->Values->AbsoluteErrorTime.value + 1;
      ww.State->Machine->Values->RelativeErrorTime = ww.State->Machine->Values->RelativeErrorTime.value + 1;
      ww.State->Machine->Values->AbsoluteMachineOnTime = ww.State->Machine->Values->AbsoluteMachineOnTime.value + 1;
      ww.State->Machine->Values->RelativeMachineOnTime = ww.State->Machine->Values->RelativeMachineOnTime.value + 1;
      ww.State->Machine->Values->AbsolutePowerPresentTime = ww.State->Machine->Values->AbsolutePowerPresentTime.value + 1;
      ww.State->Machine->Values->RelativePowerPresentTime = ww.State->Machine->Values->RelativePowerPresentTime.value + 1;
      ww.State->Machine->Flags->RecipeInRun = false;
      ww.State->Machine->Values->FeedSpeed = 0;
      break;
    }
  }

  if ((m_simStep % 10) == 1) {
    woodworking::WwBaseEvent_t event;
    event.EventCategory = UA_WwEventCategoryEnumeration::UA_WWEVENTCATEGORYENUMERATION_DIAGNOSTIC;
    std::stringstream msgId;
    msgId << "id" << m_simStep;
    event.MessageId = msgId.str();
    event.PathParts = "Machine";
    std::stringstream msg;
    msg << "Example Message " << m_simStep;
    event.Message = {"", msg.str()};
    event.SourceName = "Machine";
    event.Severity = 1;
    OpcUaEvent ev(event, m_pServer, ww.Events.NodeId);
  }
}
