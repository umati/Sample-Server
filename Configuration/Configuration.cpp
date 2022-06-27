/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 * Copyright 2022 (c) Alen Galinec
 */

#include "Configuration.hpp"

namespace Configuration {
Configuration DefaultConfiguration() {
  Configuration ret;
  ret.Hostname = "localhost";
  ret.Port = 4840;
  Encryption_t enc;
  enc.ServerKey = "Server_key.pem";
  enc.ServerCert = "Server_cert.pem";
  ret.Encryption = enc;
  return ret;
}
}  // namespace Configuration
