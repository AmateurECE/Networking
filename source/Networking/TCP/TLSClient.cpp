///////////////////////////////////////////////////////////////////////////////
// NAME:            TLSClient.cpp
//
// AUTHOR:          Ethan D. Twardy <edtwardy@mtu.edu>
//
// DESCRIPTION:     Implementation of the TLSClient.
//
// CREATED:         04/09/2020
//
// LAST EDITED:     04/10/2020
////

#include <Networking/TCP/TLSClient.h>
#include <Networking/TCP/TLSException.h>

#include <sys/stat.h>

#include <openssl/err.h>

#define str(x) _str(x)
#define _str(x) #x

static std::string getSSLErrors()
{
  BIO* bio = BIO_new(BIO_s_mem());
  ERR_print_errors(bio);
  char* buf = nullptr;
  BIO_get_mem_data(bio, &buf);
  return std::string{buf};
}

Networking::TCP::TLSClient
::TLSClient(NetAddress hostAddress, std::function<void(BIO*)> userHandler,
            bool useTwoWayAuthentication, std::string customCACertificatePath,
            std::function<void(const std::string&)> logStream)
  : m_sslContext{createContext(customCACertificatePath), [](SSL_CTX* ctx)
    {
      SSL_CTX_free(ctx);
    }}, m_hostAddress{hostAddress}, m_userHandler{userHandler},
    m_useTwoWayAuthentication{useTwoWayAuthentication},
    m_logStream{logStream}
{}

SSL_CTX*
Networking::TCP::TLSClient
::createContext(std::string customCACertificatePath)
{
  SSL_CTX* context = nullptr;
  const SSL_METHOD* method = TLS_method();
  if (nullptr == method)
    {
      throw std::runtime_error{std::string{__FILE__":" str(__LINE__) ":"}
        + "Could not create TLS_method; error trace:\n" + getSSLErrors()};
    }

  context = SSL_CTX_new(method);
  if (nullptr == context)
    {
      throw std::runtime_error{std::string{__FILE__":" str(__LINE__) ":"}
        + "Could not create SSL_CTX; error trace:\n" + getSSLErrors()};
    }

  // Ensure the chain is verified according to RFC 4158
  // pass 'nullptr' for callback function because we don't care to do any
  // extra processing.
  SSL_CTX_set_verify(context, SSL_VERIFY_PEER, nullptr);
  SSL_CTX_set_verify_depth(context, 4);

  // Disable SSLv2/3 and compression.
  const long flags = SSL_OP_NO_SSLv2 | SSL_OP_NO_SSLv3 | SSL_OP_NO_COMPRESSION;
  SSL_CTX_set_options(context, flags);

  long result = 1;

  if ("" == customCACertificatePath)
    {
      // Use the default trusted CA Certificate paths
      result = SSL_CTX_set_default_verify_paths(context);
    }
  else
    {
      errno = 0;
      struct stat fileStats;
      memset(&fileStats, 0, sizeof(fileStats));
      if (0 != stat(customCACertificatePath.c_str(), &fileStats))
        {
          throw std::system_error{errno, std::generic_category()};
        }

      if (fileStats.st_mode & S_IFDIR)
        {
          // customCACertificatePath points to a directory
          result = SSL_CTX_load_verify_locations
            (context, nullptr, customCACertificatePath.c_str());
        }
      else
        {
          // customCACertificatePath points to a plain file
          result = SSL_CTX_load_verify_locations
            (context, customCACertificatePath.c_str(), nullptr);
        }
    }

  if (1 != result)
    {
      throw std::runtime_error{std::string{__FILE__":" str(__LINE__) ":"}
        + "Could not set CA Certificate path; error trace:\n"
          + getSSLErrors()};
    }

  return context;
}

void Networking::TCP::TLSClient::connect()
{
  // Similar to TLSListener, we wrap the BIO pointer in a shared_ptr, but then
  // we pass the raw pointer to the user. Why? Because the SSL library
  // functions require the raw pointer, so the user will have to get it from
  // the shared_ptr anyways. We bother wrapping it at all so that we can ensure
  // the BIO is freed even if the normal flow of logic is interrupted (e.g. by
  // exception).
  std::shared_ptr<BIO> sslBIO{
    [this]() -> BIO*{
      BIO* theBio = BIO_new_ssl_connect(m_sslContext.get());
      if (nullptr == theBio)
        {
          throw std::runtime_error{std::string{__FILE__":" str(__LINE__) ":"}
            + "Could not create connect BIO; error trace:\n" + getSSLErrors()};
        }
      return theBio;
    }(), [](BIO* bio){
      BIO_free_all(bio);
    }};
  BIO* stream = sslBIO.get();

  long result = 1;
  std::string hostString = m_hostAddress.getIPDotNotation() + ":"
    + std::to_string(m_hostAddress.getPortHostOrder());
  result = BIO_set_conn_hostname(stream, hostString.c_str());
  if (1 != result)
    {
      throw std::runtime_error{std::string{__FILE__":" str(__LINE__) ":"}
        + "Could not set connection hostname; error trace:\n"
          + getSSLErrors()};
    }

  SSL* ssl = nullptr;
  BIO_get_ssl(stream, &ssl);
  if (nullptr == ssl)
    {
      throw std::runtime_error{std::string{__FILE__":" str(__LINE__) ":"}
        + "Could not retrieve SSL wrapper; error trace:\n" + getSSLErrors()};
    }

  // Disable weak ciphers.
  const char* const PREFERRED_CIPHERS
    = "HIGH:!aNULL:!kRSA:!PSK:!SRP:!MD5:!RC4";
  result = SSL_set_cipher_list(ssl, PREFERRED_CIPHERS);
  if (1 != result)
    {
      throw std::runtime_error{std::string{__FILE__":" str(__LINE__) ":"}
        + "Could not set preferred ciphers; error trace:\n" + getSSLErrors()};
    }

  result = BIO_do_connect(stream);
  if (1 != result)
    {
      result = SSL_get_verify_result(ssl);
      throw TLSException{std::string{__FILE__":" str(__LINE__) ":"}
        + "Could not connect to host; error trace:\n" + getSSLErrors()
          + "\n" + X509_verify_cert_error_string(result), m_hostAddress};
    }

  result = BIO_do_handshake(stream);
  if (1 != result)
    {
      throw TLSException{std::string{__FILE__":" str(__LINE__) ":"}
        + "Could not create TLS connection; error trace:\n" + getSSLErrors(),
          m_hostAddress};
    }

  // Verify that an x509 certificate WAS provided
  X509* cert = SSL_get_peer_certificate(ssl);
  if (nullptr == cert)
    {
      throw TLSException{std::string{__FILE__":" str(__LINE__) ":"}
        + "Host did not provide an x509 certificate; error trace:\n"
          + getSSLErrors(),
          m_hostAddress};
    }
  X509_free(cert);

  result = SSL_get_verify_result(ssl);
  if (X509_V_OK != result)
    {
      throw TLSException{std::string{__FILE__":" str(__LINE__) ":"}
        + "Error occurred during chain verification; error trace:\n"
          + getSSLErrors(),
          m_hostAddress};
    }

  // We have successfully connect to the client.
  m_logStream("Successfully connected to " + m_hostAddress.getIPDotNotation());
  m_userHandler(stream);
}


///////////////////////////////////////////////////////////////////////////////
