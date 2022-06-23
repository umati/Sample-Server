/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 * Copyright 2022 (c) Alen Galinec
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

  if (j.count("Port") != 0) {
    p.Port = j.at("Port").get<decltype(p.Port)::value_type>();
  }

  if (j.count("Encryption") != 0) {
    p.Encryption = j.at("Encryption").get<decltype(p.Encryption)::value_type>();
  }

  if (j.count("UserPassAuthentication") != 0) {
    p.UserPassAuthentication = j.at("UserPassAuthentication").get<decltype(p.UserPassAuthentication)::value_type>();
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
