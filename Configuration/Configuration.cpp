#include "Configuration.hpp"

namespace Configuration {
    Configuration DefaultConfiguration() {
        Configuration ret;
        ret.Hostname = "localhost";
        Encryption_t enc;
        enc.ServerKey = "Server_key.pem";
        enc.ServerCert = "Server_cert.pem";
        ret.Encryption = enc;
        return ret;
    }
}
