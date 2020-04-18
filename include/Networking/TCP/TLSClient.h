///////////////////////////////////////////////////////////////////////////////
// NAME:            TLSClient.h
//
// AUTHOR:          Ethan D. Twardy <edtwardy@mtu.edu>
//
// DESCRIPTION:     Client for initiating TLS connections to a TLS server.
//
// CREATED:         04/09/2020
//
// LAST EDITED:     04/18/2020
////

#ifndef __ET_TLSCLIENT__
#define __ET_TLSCLIENT__

#include <namespaces/Networking.h>
#include <Networking/NetworkHost.h>

#include <iostream>

#include <openssl/ssl.h>

template<class HostType>
class Networking::TCP::TLSClient
{
public:
  TLSClient(HostType hostAddress,
            std::function<void(BIO*)> userHandler,
            bool useTwoWayAuthentication,
            std::string customCACertificatePath,
            std::function<void(const std::string&)> logStream);

  void connect();

  class Builder;

private:
  SSL_CTX* createContext(std::string);
  std::string getHostString() const;

  std::unique_ptr<SSL_CTX, std::function<void(SSL_CTX*)>> m_sslContext;

  HostType m_hostAddress;
  std::function<void(BIO*)> m_userHandler;
  const bool m_useTwoWayAuthentication;
  std::function<void(const std::string&)> m_logStream;
};

template<class HostType>
class Networking::TCP::TLSClient<HostType>::Builder
{
public:
  Builder();
  Builder setHostAddress(HostType);
  Builder setUserHandler(std::function<void(BIO*)>);
  Builder setTwoWayAuthentication(bool);
  Builder setCustomCACertificatePath(std::string);
  Builder setLogStream(std::function<void(const std::string&)>);

  TLSClient<HostType> build() const;

private:

  HostType m_hostAddress;
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

#include <Networking/TCP/TLSClient.tcc>

#endif // __ET_TLSCLIENT__

///////////////////////////////////////////////////////////////////////////////
