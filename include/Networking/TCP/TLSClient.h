///////////////////////////////////////////////////////////////////////////////
// NAME:            TLSClient.h
//
// AUTHOR:          Ethan D. Twardy <edtwardy@mtu.edu>
//
// DESCRIPTION:     Client for initiating TLS connections to a TLS server.
//
// CREATED:         04/09/2020
//
// LAST EDITED:     04/17/2020
////

#ifndef __ET_TLSCLIENT__
#define __ET_TLSCLIENT__

#include <namespaces/Networking.h>
#include <Networking/NetworkHost.h>

#include <iostream>

#include <openssl/ssl.h>

class Networking::TCP::TLSClient
{
public:
  TLSClient(NetworkHost hostAddress,
            std::function<void(BIO*)> userHandler,
            bool useTwoWayAuthentication,
            std::string customCACertificatePath,
            std::function<void(const std::string&)> logStream);

  void connect();

  class Builder;

private:
  SSL_CTX* createContext(std::string);

  std::unique_ptr<SSL_CTX, std::function<void(SSL_CTX*)>> m_sslContext;

  NetworkHost m_hostAddress;
  std::function<void(BIO*)> m_userHandler;
  const bool m_useTwoWayAuthentication;
  std::function<void(const std::string&)> m_logStream;
};

class Networking::TCP::TLSClient::Builder
{
public:
  Builder() = default;
  Builder setHostAddress(NetworkHost);
  Builder setUserHandler(std::function<void(BIO*)>);
  Builder setTwoWayAuthentication(bool);
  Builder setCustomCACertificatePath(std::string);
  Builder setLogStream(std::function<void(const std::string&)>);

  TLSClient build() const;

private:
  NetworkHost m_hostAddress = NetworkHost{INADDR_LOOPBACK, 443};
  std::function<void(BIO*)> m_userHandler = [](BIO*){ return; };
  bool m_useTwoWayAuthentication = false;

  // Pass "" to use default CA certificate paths.
  std::string m_customCACertificatePath = "";

  // By default, simply send error messages to cerr.
  std::function<void(const std::string&)> m_logStream =
    [](const std::string& message)
  {
    std::cerr << message << '\n';
  };
};

#endif // __ET_TLSCLIENT__

///////////////////////////////////////////////////////////////////////////////
