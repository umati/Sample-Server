/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2020-2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 */

#include "SimulatedInstance.hpp"

#include <cstdint>

SimulatedInstance::SimulatedInstance():
  m_mqttSettings(MqttSettings{})
 { rnd.seed(static_cast<unsigned int>(reinterpret_cast<std::uintptr_t>(this))); }
SimulatedInstance::SimulatedInstance(MqttSettings settings): m_mqttSettings(settings)
 { rnd.seed(static_cast<unsigned int>(reinterpret_cast<std::uintptr_t>(this))); }


SimulatedInstance::~SimulatedInstance() {}
