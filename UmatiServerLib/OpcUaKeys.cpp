#include "OpcUaKeys.hpp"
#include <iostream>
#include <fstream>

#include <mbedtls/pk.h>
#include <mbedtls/entropy.h>
#include <mbedtls/entropy_poll.h>
#include <mbedtls/rsa.h>
#include <mbedtls/error.h>
#include <mbedtls/ctr_drbg.h>
#include <mbedtls/x509_crt.h>
#include <cstring> // memset
#include <stdexcept>
#include <iomanip>
#include "../arch/gmtime.hpp"
#include <sstream>
#include <open62541/types_generated_handling.h>

template <typename T>
class ReturnCodeWatcher
{
public:
  T Good;
  ReturnCodeWatcher(T good) : Good(good) {}
  ReturnCodeWatcher(const ReturnCodeWatcher &other) : Good(other.Good)
  {
  }

  ReturnCodeWatcher &operator=(const ReturnCodeWatcher &other)
  {
    Good = other.Good;
    return *this;
  }

  T operator=(const T code)
  {
    if (code != Good)
    {
      throw std::runtime_error("Bad return code.");
    }
    return code;
  }
};

OpcUaKeys::OpcUaKeys(std::string privFile, std::string pubFile) : PrivFile(privFile), PubFile(pubFile)
{
}

void OpcUaKeys::Load()
{
  PrivateKey = readFile(PrivFile);
  PublicCert = readFile(PubFile);
}

OpcUaKeys::~OpcUaKeys()
{
  UA_ByteString_clear(&PrivateKey);
  UA_ByteString_clear(&PublicCert);
}

UA_ByteString OpcUaKeys::readFile(std::string filename)
{
  UA_ByteString ret = UA_BYTESTRING_NULL;
  std::ifstream ifs(filename, std::ios::binary);
  if (!ifs.good())
  {
    std::stringstream ss;
    ss << "File not found: '" << filename << "'";
    throw std::runtime_error("File not found.");
  }

  ifs.seekg(0, std::ios_base::end);
  std::streampos fileSize = ifs.tellg();
  UA_ByteString_allocBuffer(&ret, fileSize);
  ifs.seekg(0, std::ios_base::beg);
  ifs.read((char*)ret.data, fileSize);
  return ret;
}

void OpcUaKeys::generatePrivateKey()
{
  //./gen_key  type=rsa rsa_keysize=4096 filename=priv.der format=der
  std::cout << "Generate private key, this may take a while." << std::endl;
  // See MbedTLS gen_key
  const unsigned int keyLen = 4096;
  ReturnCodeWatcher<int> ret(0);
  mbedtls_pk_context key;
  int i;
  char *p, *q;
  mbedtls_entropy_context entropy;
  mbedtls_ctr_drbg_context ctr_drbg;
  mbedtls_pk_init(&key);
  mbedtls_ctr_drbg_init(&ctr_drbg);

  mbedtls_entropy_init(&entropy);

  ret = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, NULL, 0);
  ret = mbedtls_pk_setup(&key, mbedtls_pk_info_from_type(MBEDTLS_PK_RSA));
  ret = mbedtls_rsa_gen_key(mbedtls_pk_rsa(key), mbedtls_ctr_drbg_random, &ctr_drbg, keyLen, 65537);
  unsigned char output_buf[16000];
  int len = mbedtls_pk_write_key_der(&key, output_buf, 16000);
  if (len < 0)
  {
    throw std::runtime_error("Save private der-Key failed.");
  }
  // mbedtls_pk_write_key_der writes from the back of the buffer
  unsigned char *c = output_buf + sizeof(output_buf) - len;
  std::ofstream derFile(PrivFile, std::ios::out | std::ios::binary);
  derFile.write((char *)c, len);
  derFile.close();
  mbedtls_pk_free(&key);
  mbedtls_ctr_drbg_free(&ctr_drbg);
  mbedtls_entropy_free(&entropy);
  std::cout << "Private key generated" << std::endl;
}

void OpcUaKeys::generateCertificate()
{
  //./cert_write issuer_key=priv.der selfsign=1 output_file=priv_der.pub issuer_name=CN=domain.example.com,O=ExampleServer,C=DE
  std::string serialStr("1");
  std::string issuer_name("CN=domain.example.com,O=ExampleServer,C=DE");
  std::string not_before, not_after;

  {
    std::time_t t = std::time(nullptr);
    std::tm tm;
    std::memset(&tm, 0, sizeof(tm));
    UMATI_GMTIME(&t, &tm);
    std::stringstream ss;
    ss << std::put_time(&tm, "%Y%m%d%H%M%S");
    not_before = ss.str();
  }

  {
    std::time_t t = std::time(nullptr);
    t += 5l * 365l * 24l * 60l * 60l; // Add 5 years
    std::tm tm;
    std::memset(&tm, 0, sizeof(tm));
    UMATI_GMTIME(&t, &tm);
    std::stringstream ss;
    ss << std::put_time(&tm, "%Y%m%d%H%M%S");
    not_after = ss.str();
  }

  // See MbedTLS write_cert
  ReturnCodeWatcher<int> ret(0);
  mbedtls_x509_crt issuer_crt;
  mbedtls_pk_context loaded_issuer_key, loaded_subject_key;
  mbedtls_pk_context *issuer_key = &loaded_issuer_key,
                     *subject_key = &loaded_subject_key;
  mbedtls_x509write_cert crt;
  mbedtls_mpi serial;
  mbedtls_entropy_context entropy;
  mbedtls_ctr_drbg_context ctr_drbg;

  mbedtls_x509write_crt_init(&crt);
  mbedtls_x509write_crt_set_md_alg(&crt, MBEDTLS_MD_SHA256);
  mbedtls_pk_init(&loaded_issuer_key);
  mbedtls_pk_init(&loaded_subject_key);
  mbedtls_mpi_init(&serial);
  mbedtls_ctr_drbg_init(&ctr_drbg);
  mbedtls_x509_crt_init(&issuer_crt);

  ret = mbedtls_ctr_drbg_seed(&ctr_drbg, mbedtls_entropy_func, &entropy, NULL, 0);
  ret = mbedtls_mpi_read_string(&serial, 10, serialStr.c_str());

  ret = mbedtls_pk_parse_keyfile(&loaded_issuer_key, PrivFile.c_str(), NULL);
  // Same for self sign
  mbedtls_x509write_crt_set_subject_key(&crt, issuer_key);
  mbedtls_x509write_crt_set_issuer_key(&crt, issuer_key);
  ret = mbedtls_x509write_crt_set_subject_name(&crt, issuer_name.c_str());
  ret = mbedtls_x509write_crt_set_issuer_name(&crt, issuer_name.c_str());
  ret = mbedtls_x509write_crt_set_serial(&crt, &serial);

  ret = mbedtls_x509write_crt_set_validity(&crt, not_before.c_str(), not_after.c_str());
  ret = mbedtls_x509write_crt_set_basic_constraints(&crt, 0, -1);
  unsigned int usage =
      MBEDTLS_X509_KU_DIGITAL_SIGNATURE |
      MBEDTLS_X509_KU_NON_REPUDIATION |
      MBEDTLS_X509_KU_KEY_ENCIPHERMENT |
      MBEDTLS_X509_KU_DATA_ENCIPHERMENT |
      MBEDTLS_X509_KU_KEY_CERT_SIGN;
  ret = mbedtls_x509write_crt_set_key_usage(&crt, usage);
  // TODO Require possibility for set X509v3 Subject Alternative Name
  // See https://github.com/ARMmbed/mbedtls/pull/731

  throw std::runtime_error("Generating public key not implemented.");
}
