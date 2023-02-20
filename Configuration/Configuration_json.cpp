/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 */

#include "Configuration_json.hpp"

#include <fstream>
#include <nlohmann/json.hpp>
#include <sstream>

namespace Configuration {
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Encryption_t, ServerCert, ServerKey, TrustedClients, IssuerCerts, Revocation);
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(UserPassAuthentication_t, Username, Password);

void from_json(const nlohmann::json &j, Configuration &p) {
  // if we also allow "null" values, then we need to add an "is_string()"
  // check
  if (j.count("Hostname") != 0) {
    p.Hostname = j.at("Hostname").get<decltype(p.Hostname)::value_type>();
  }

  if (j.count("Encryption") != 0) {
    p.Encryption = j.at("Encryption").get<decltype(p.Encryption)::value_type>();
  }

  if (j.count("UserPassAuthentication") != 0) {
    p.UserPassAuthentication = j.at("UserPassAuthentication").get<decltype(p.UserPassAuthentication)::value_type>();
  }

  if (j.count("MQTTPubSub") != 0) {
    MQTTPubSubConfiguration_t config;
    auto bUrl = j.at("MQTTPubSub").at("BrokerUrl").get<std::string>();
    config.BrokerUrl = bUrl;
    auto prefix = j.at("MQTTPubSub").at("Prefix").get<std::string>();
    config.Prefix = prefix;
    auto pId = j.at("MQTTPubSub").at("PublisherId").get<std::string>();
    config.PublisherId = pId;
    
    if (j.at("MQTTPubSub").count("MqttCaFile") != 0) {
     config.MqttCaFile = j.at("MQTTPubSub").at("MqttCaFile").get<decltype(p.MQTTPubSub->MqttCaFile)::value_type>(); }
    if (j.at("MQTTPubSub").count("Username") != 0) {
     config.Username = j.at("MQTTPubSub").at("Username").get<decltype(p.MQTTPubSub->Username)::value_type>(); }
    if (j.at("MQTTPubSub").count("Password") != 0) {
     config.Password = j.at("MQTTPubSub").at("Password").get<decltype(p.MQTTPubSub->Password)::value_type>(); }
    p.MQTTPubSub = config;
  }

  if (j.count("Ads") != 0) {
    ADSConfiguration_t config;
    auto localNetId = j.at("Ads").at("LocalNetId").get<std::array<uint8_t,6>>();
    auto remotNetId = j.at("Ads").at("RemoteNetId").get<std::array<uint8_t,6>>();
    auto remotePort = j.at("Ads").at("RemotePort").get<uint16_t>();

    config.LocalNetId = localNetId;
    config.RemoteNetId = remotNetId;
    config.RemotePort = remotePort;
    p.Ads = config;
  }
}

Configuration FromJsonFile(std::string filename) {
  std::ifstream ifs(filename);
  if (!ifs.good()) {
    std::stringstream ss;
    ss << "File not found: '" << filename << "'";
    throw std::runtime_error("File not found.");
  }
  nlohmann::json j;
  ifs >> j;
  return j.get<Configuration>();
}
}  // namespace Configuration
