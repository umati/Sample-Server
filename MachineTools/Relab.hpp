/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2020-2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 * Copyright 2020-2021 (c) Götz Görisch, VDW - Verein Deutscher Werkzeugmaschinenfabriken e.V.
 */
#pragma once
#include <array>
#include "InstantiatedMachineTool.hpp"
#include "ADS/AdsLib.h"
#include "ADS/AmsRouter.h"
#include "ADS/AdsVariable.h"

class Relab : public InstantiatedMachineTool {
 public:
  struct AdsSettings {
    AmsNetId LocalNetId;
    AmsNetId RemoteNetId;
    std::string RemoteNetIdStr;
    uint16_t RemotePort; 

    AdsSettings(std::array<uint8_t, 6> lid, std::array<uint8_t, 6> rid, std::string RemoteNetId, uint16_t RemotePort) 
    : LocalNetId{lid[0], lid[1], lid[2],lid[3],lid[4],lid[5] },
      RemoteNetId{rid[0], rid[1], rid[2], rid[3], rid[4],rid[5] },
      RemoteNetIdStr(RemoteNetId),
      RemotePort(RemotePort) {}
  };

  Relab(UA_Server *pServer, AdsSettings adsSettings);
  Relab(UA_Server *pServer, MqttSettings mqttSettings, AdsSettings adsSettings);

  void Simulate() override;
  virtual ~Relab() = default;

 protected:
  void CreateObject() override;

  void InstantiateIdentification();
  void InstantiateMonitoring();
  void InstantiateTools();
  void InstantiateProduction();

  int m_simStep = 0;
  long      nErr, nPort;
  AmsAddr   Addr;
  std::string remoteAddr;
  AdsDevice* route;
};
