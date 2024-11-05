/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2022 (c) Patrick Moeller, 3Yourmind GmbH
 * Copyright 2022 (c) Dennis Hezel, 3Yourmind GmbH
 */

#include "ShowcaseAMMachine.hpp"

ShowcaseAMMachine::ShowcaseAMMachine(UA_Server *pServer) : InstantiatedAMMachine(pServer) {
  MachineName = "ShowcaseAMMachine";
  CreateObject();
}

void ShowcaseAMMachine::CreateObject() {
  InstantiatedAMMachine::CreateObject();
  mt.Identification->ProductInstanceUri = "http://www.3yourmind.com/#ShowcaseAMMachine";
  mt.Identification->Location = "JIMTOF E7 E7128/VIRTUAL 1 1/N 52.491225 E 13.388392";
  InstantiateEquipment();
}

template <class T>
static void InitiateRemainingQuantity(T &RemainingQuantity, double value) {
  RemainingQuantity->Value = value;
  RemainingQuantity->EngineeringUnits.value.DisplayName.text = "kg";
  RemainingQuantity->EngineeringUnits.value.Description.text = "Kilogram";
  RemainingQuantity->EngineeringUnits.value.UnitId = 4933453;
  RemainingQuantity->EngineeringUnits.value.NamespaceUri = "http://www.opcfoundation.org/UA/units/un/cefact";
}

void ShowcaseAMMachine::InstantiateEquipment() {
  InstantiateOptional(mt.Equipment->Materials, m_pServer, n);
  InstantiateOptional(mt.Equipment->Consumables, m_pServer, n);

  for (size_t i = 1; i <= 2; ++i) {
    std::stringstream consumable_ss;
    consumable_ss << "Consumable" << i;
    std::stringstream material_ss;
    material_ss << "Material" << i;
    std::stringstream manufacturer_ss;
    manufacturer_ss << "Manufacturer" << i;

    auto &material = mt.Equipment->Materials->Material.Add(m_pServer, n, {m_nsIndex, material_ss.str()});

    InstantiateOptional(material.ExternalIdentifier, m_pServer, n);
    InstantiateOptional(material.Manufacturer, m_pServer, n);
    InstantiateOptional(material.Name, m_pServer, n);
    InstantiateOptional(material.RemainingQuantity, m_pServer, n);

    material.ReadyForProduction = (i % 2) > 0;
    material.Identifier = std::to_string(i * 10 + 2);
    material.ExternalIdentifier = std::to_string(i * 10 + 3);
    material.Function = UA_MaterialFunction::UA_MATERIALFUNCTION_MAIN;
    material.Manufacturer = manufacturer_ss.str();
    material.Name = material_ss.str();
    InitiateRemainingQuantity(material.RemainingQuantity, 10.0 * i);

    auto &consumable = mt.Equipment->Consumables->Consumable.Add<AdditiveManufacturing::Consumable_t>(m_pServer, n, {m_nsIndex, consumable_ss.str()});

    InstantiateOptional(consumable.ExternalIdentifier, m_pServer, n);
    InstantiateOptional(consumable.Manufacturer, m_pServer, n);
    InstantiateOptional(consumable.Name, m_pServer, n);
    InstantiateOptional(consumable.RemainingQuantity, m_pServer, n);

    consumable.Identifier = std::to_string(i * 10 + 4);
    consumable.ExternalIdentifier = std::to_string(i * 10 + 5);
    consumable.Manufacturer = manufacturer_ss.str();
    consumable.Name = material_ss.str();
    InitiateRemainingQuantity(consumable.RemainingQuantity, 10.0 * i);
  }
}

void ShowcaseAMMachine::Simulate() {
  ++m_simStep;
  if ((m_simStep % 2) == 1) {
    SimulateStacklight();
  }

  mt.Monitoring->MachineTool->PowerOnDuration = m_simStep / 3600;
}
