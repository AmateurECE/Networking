///////////////////////////////////////////////////////////////////////////////
// NAME:            TLSClient.h
//
// AUTHOR:          Ethan D. Twardy <edtwardy@mtu.edu>
//
// DESCRIPTION:     Client for initiating TLS connections to a TLS server.
//
// CREATED:         04/09/2020
//
// LAST EDITED:     04/09/2020
////

#ifndef __ET_TLSCLIENT__
#define __ET_TLSCLIENT__

#include <namespaces/Networking.h>
#include <Networking/NetAddress.h>

#include <iostream>

#include <openssl/ssl.h>

class Networking::TCP::TLSClient
{
public:
  TLSClient(NetAddress hostAddress,
            std::function<void(BIO*)> userHandler,
            bool useTwoWayAuthentication = false,

            // Pass "" to use default CA certificate paths.
            std::string customCACertificatePath = "",

            // By default, simply send error messages to cerr.
            std::function<void(const std::string&)> logStream
            =[](const std::string& message)
              {
                std::cerr << message << '\n';
              });

  void connect();

private:
  SSL_CTX* createContext(std::string);

  std::unique_ptr<SSL_CTX, std::function<void(SSL_CTX*)>> m_sslContext;

  NetAddress m_hostAddress;
  std::function<void(BIO*)> m_userHandler;
  const bool m_useTwoWayAuthentication;
  std::function<void(const std::string&)> m_logStream;
};

#endif // __ET_TLSCLIENT__

///////////////////////////////////////////////////////////////////////////////
