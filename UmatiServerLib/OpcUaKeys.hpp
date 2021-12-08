/* This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * Copyright 2020-2021 (c) Christian von Arnim, ISW University of Stuttgart (for umati and VDW e.V.)
 */

#pragma once

#include <open62541/types.h>

#include <string>
#include <vector>

/**
 * @brief Helper for OPC UA connection encryption
 *
 * Generating a Public/Private-Key-Pair is not fully implemented.
 */
class OpcUaKeys {
 public:
  OpcUaKeys(
    std::string privFile,
    std::string pubFile,
    std::vector<std::string> trustedClients,
    std::vector<std::string> issuerCerts,
    std::vector<std::string> revocation);
  void Load();
  UA_ByteString PrivateKey = UA_BYTESTRING_NULL;
  UA_ByteString PublicCert = UA_BYTESTRING_NULL;

  std::vector<UA_ByteString> Trusted;
  std::vector<UA_ByteString> Issuer;
  std::vector<UA_ByteString> Revoked;
  virtual ~OpcUaKeys();

  void GenerateKeys();
  void StoreKeys();

 protected:  // Not fully implemented, so hidden
  void generatePrivateKey();
  void generateCertificate();

  void writeFile(std::string filename, const UA_ByteString& content);
  UA_ByteString readFile(std::string filename);
  std::vector<UA_ByteString> readDir(std::string dirname);
  std::vector<UA_ByteString> readFiles(std::vector<std::string> filenames);

 protected:
  std::string PrivKeyFile;
  std::string PubCertFile;
  std::vector<std::string> TrustedClients;
  std::vector<std::string> IssuerCerts;
  std::vector<std::string> Revocation;
  /*std::string TrustedDir;
  std::string IssuerDir;
  std::string RevocationDir;*/
};
