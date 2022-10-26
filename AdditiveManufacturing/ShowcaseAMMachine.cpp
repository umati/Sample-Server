/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2022 (c) Patrick Moeller, 3Yourmind GmbH
 */

#include "ShowcaseAMMachine.hpp"

ShowcaseAMMachine::ShowcaseAMMachine(UA_Server *pServer) : InstantiatedMachineTool(pServer) {
  MachineName = "ShowcaseAMMachine";
  CreateObject();
}

void ShowcaseAMMachine::CreateObject() {
  InstantiatedMachineTool::CreateObject();
  InstantiateIdentification();
  InstantiateMonitoring();
  InstantiateEquipment();
  InstantiateProduction();
}

void ShowcaseAMMachine::InstantiateIdentification() {
  InstantiatedMachineTool::InstantiateIdentification();
  InstantiateOptional(machine.Identification->YearOfConstruction, m_pServer, n);
  InstantiateOptional(machine.Identification->ProductCode, m_pServer, n);
  InstantiateOptional(machine.Identification->SoftwareRevision, m_pServer, n);
  InstantiateOptional(machine.Identification->DeviceClass, m_pServer, n);
  InstantiateOptional(machine.Identification->Location, m_pServer, n);
  InstantiateOptional(machine.Identification->Model, m_pServer, n);

  machine.Identification->Manufacturer = {"", "3YOURMIND"};
  machine.Identification->ProductCode = "AM_123_SC";
  machine.Identification->YearOfConstruction = 2022;
  machine.Identification->SoftwareRevision = "v1.0.0";
  machine.Identification->DeviceClass = "Additive Manufacturing machine";
  machine.Identification->ProductInstanceUri = "http://www.3yourmind.com/#ShowcaseAMMachine";
  machine.Identification->Location = "FormNext 10 A75/IMTS E3 134744/VIRTUAL 0 0/N 52.491225 E 13.388392";  // FIXME: add formnext booth location
  machine.Identification->Model = {"", MachineName};
  machine.Identification->AMTechnologyIdentifier = "MEX";
}

void ShowcaseAMMachine::InstantiateMonitoring() {
  InstantiateMonitoringMT();
  InstantiateMonitoringStacklight({UA_SignalColor::UA_SIGNALCOLOR_RED, UA_SignalColor::UA_SIGNALCOLOR_YELLOW, UA_SignalColor::UA_SIGNALCOLOR_GREEN});
  InstantiateMonitoringChannel(1);  // FIXME: Is this needed?
}

void ShowcaseAMMachine::InstantiateEquipment() {
  InstantiateOptional(machine.Equipment->Materials, m_pServer, n);
  InstantiateOptional(machine.Equipment->Consumables, m_pServer, n);

  for (size_t i = 1; i <= 2; ++i) {
    std::stringstream consumable;
    consumable << "Consumable" << i;
    std::stringstream material;
    material << "Material" << i;
    std::stringstream manufacturer;
    manufacturer << "Manufacturer" << i;
    std::stringstream quantity;
    quantity << i << "KG";

    auto &material = machine.Equipment->Materials->Material.Add<AdditiveManufacturing::Material_t>(m_pServer, n, {m_nsIndex, material.str()});

    material.ReadyForProduction = (i % 2) > 0;
    material.Identifier = i * 10 + 2;
    material.ExternalIdentifier = i * 10 + 3;
    material.Function = "Main";
    material.Manufacturer = manufacturer.str();
    material.Name = material.str();
    material.RemainingQuantity = quantity.str();

    InstantiateOptional(material.ReadyForProduction, m_pServer, n);
    InstantiateOptional(material.ExternalIdentifier, m_pServer, n);
    InstantiateOptional(material.Function, m_pServer, n);
    InstantiateOptional(material.Manufacturer, m_pServer, n);
    InstantiateOptional(material.Name, m_pServer, n);
    InstantiateOptional(material.RemainingQuantity, m_pServer, n);

    auto &consumable = machine.Equipment->Consumables->Consumable.Add<AdditiveManufacturing::Consumable_t>(m_pServer, n, {m_nsIndex, consumable.str()});

    consumable.Identifier = i * 10 + 4;
    consumable.ExternalIdentifier = i * 10 + 5;
    consumable.Manufacturer = manufacturer.str();
    consumable.Name = material.str();
    consumable.RemainingQuantity = quantity.str();

    InstantiateOptional(consumable.ExternalIdentifier, m_pServer, n);
    InstantiateOptional(consumable.Manufacturer, m_pServer, n);
    InstantiateOptional(consumable.Name, m_pServer, n);
    InstantiateOptional(consumable.RemainingQuantity, m_pServer, n);
  }

  void ShowcaseAMMachine::InstantiateProduction() {
    InstantiateOptional(machine.Production->ActiveProgram->State, m_pServer, n);
    machine.Production->ActiveProgram->NumberInList = 0;
    machine.Production->ActiveProgram->Name = "Print Job";
    machine.Production->ActiveProgram->State->CurrentState->Value = {"en", "Running"};
    machine.Production->ActiveProgram->State->CurrentState->Number = 1;
    machine.Production->ActiveProgram->State->CurrentState->Id =
      UA_NODEID_NUMERIC(nsFromUri(m_pServer, constants::NsMachineToolUri), UA_MACHINETOOLID_PRODUCTIONSTATEMACHINETYPE_RUNNING);
  }

  // FIXME: Is that relevant?
  void ShowcaseAMMachine::Simulate() {
    ++m_simStep;
    if ((m_simStep % 2) == 1) {
      SimulateStacklight();
    }

    machine.Monitoring->MachineTool->PowerOnDuration = m_simStep / 3600;
  }
