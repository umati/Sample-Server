#pragma once

#include <string>
#include <vector>
#include <open62541/types.h>

/**
 * @brief Helper for OPC UA connection encryption
 * 
 * Generating a Public/Private-Key-Pair is not fully implemented.
 */
class OpcUaKeys
{
public:
  OpcUaKeys(
      std::string privFile,
      std::string pubFile,
      std::vector<std::string> trustedClients,
      std::vector<std::string> issuerCerts,
      std::vector<std::string> revocation
      );
  void Load();
  UA_ByteString PrivateKey = UA_BYTESTRING_NULL;
  UA_ByteString PublicCert = UA_BYTESTRING_NULL;

  std::vector<UA_ByteString> Trusted;
  std::vector<UA_ByteString> Issuer;
  std::vector<UA_ByteString> Revoked;
  virtual ~OpcUaKeys();

protected: // Not fully implemented, so hidden
  void generatePrivateKey();
  void generateCertificate();

  UA_ByteString readFile(std::string filename);
  std::vector<UA_ByteString> readDir(std::string dirname);
  std::vector<UA_ByteString> readFiles(std::vector<std::string> filenames);

protected:
  std::string PrivFile;
  std::string PubFile;
  std::vector<std::string> TrustedClients;
  std::vector<std::string> IssuerCerts;
  std::vector<std::string> Revocation;
  /*std::string TrustedDir;
  std::string IssuerDir;
  std::string RevocationDir;*/
};
