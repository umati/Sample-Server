#include "Configuration_json.hpp"

#include <fstream>
#include <nlohmann/json.hpp>
#include <sstream>

namespace Configuration {
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(Encryption_t, ServerCert, ServerKey, TrustedClients, IssuerCerts, Revocation);

void from_json(const nlohmann::json &j, Configuration &p) {
  // if we also allow "null" values, then we need to add an "is_string()"
  // check
  if (j.count("Hostname") != 0) {
    p.Hostname = j.at("Hostname").get<std::string>();
  }

  if (j.count("Encryption") != 0) {
    p.Encryption = j.at("Encryption").get<Encryption_t>();
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
