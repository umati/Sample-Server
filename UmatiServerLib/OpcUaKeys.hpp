#pragma once

#include <string>
#include <open62541/types.h>

/**
 * @brief Helper for OPC UA connection encryption
 * 
 * Generating a Public/Private-Key-Pair is not fully implemented.
 */
class OpcUaKeys
{
public:
  OpcUaKeys(std::string privFile, std::string pubFile);
  void Load();
  UA_ByteString PrivateKey = UA_BYTESTRING_NULL;
  UA_ByteString PublicCert = UA_BYTESTRING_NULL;
  virtual ~OpcUaKeys();

protected: // Not fully implemented, so hidden
  void generatePrivateKey();
  void generateCertificate();

  UA_ByteString readFile(std::string filename);

protected:
  std::string PrivFile;
  std::string PubFile;
};
