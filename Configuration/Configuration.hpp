/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright (c) 2021,2023 Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 * Copyright (c) 2022 Alen Galinec
 */

#include <cstdint>
#include <optional>
#include <string>
#include <vector>

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

struct Configuration {
  std::optional<std::string> Hostname;
  std::optional<std::uint16_t> Port;
  std::optional<std::vector<UserPassAuthentication_t>> UserPassAuthentication;
  std::optional<Encryption_t> Encryption;
};

Configuration DefaultConfiguration();

}  // namespace Configuration
