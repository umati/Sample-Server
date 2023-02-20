/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 */

#include <optional>
#include <string>
#include <vector>
#include <array>
#include <sstream>

namespace Configuration {
struct Encryption_t {
  std::string ServerCert;
  std::string ServerKey;
  std::vector<std::string> TrustedClients;
  std::vector<std::string> IssuerCerts;
  std::vector<std::string> Revocation;
  /**
  std::string TrustedClientsDir;
  std::string IssuerDir;
  std::string RevocationDir;
  */
};

struct UserPassAuthentication_t {
  std::string Username;
  std::string Password;
};

struct MQTTPubSubConfiguration_t {
  std::optional<std::string> MqttCaFile;
  std::string Prefix;
  std::string BrokerUrl;  // Format opc.mqtt://...:1883
  std::optional<std::string> Username;
  std::optional<std::string> Password; 
  std::string PublisherId;
};

struct ADSConfiguration_t {
  std::array<uint8_t, 6> LocalNetId;
  std::array<uint8_t, 6> RemoteNetId;
  uint16_t RemotePort; 
  
  std::string getLocalNetId() {
    std::stringstream ss;
    ss << unsigned(LocalNetId[0]);

    for(auto mIter = std::next(LocalNetId.begin()); mIter != LocalNetId.end(); ++mIter) {
      ss << '.' << unsigned(*mIter);
    }

    return ss.str();
  }

  std::string getRemoteNetId() {
    std::stringstream ss;
    ss << unsigned(RemoteNetId[0]);

    for(auto mIter = std::next(RemoteNetId.begin()); mIter != (RemoteNetId.end() - 2); ++mIter) {
      ss << '.' << unsigned(*mIter);
    }

    return ss.str();
  }
};

struct Configuration {
  std::optional<std::string> Hostname;
  std::optional<std::vector<UserPassAuthentication_t>> UserPassAuthentication;
  std::optional<Encryption_t> Encryption;
  std::optional<MQTTPubSubConfiguration_t> MQTTPubSub;
  std::optional<ADSConfiguration_t> Ads;
};

Configuration DefaultConfiguration();

}  // namespace Configuration
