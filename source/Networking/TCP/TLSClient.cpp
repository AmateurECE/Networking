///////////////////////////////////////////////////////////////////////////////
// NAME:            TLSClient.cpp
//
// AUTHOR:          Ethan D. Twardy <edtwardy@mtu.edu>
//
// DESCRIPTION:     Implementation of the TLSClient.
//
// CREATED:         04/09/2020
//
// LAST EDITED:     04/09/2020
////

#include <Networking/TCP/TLSClient.h>

// std::unique_ptr<SSL_CTX> m_sslContext;
// NetAddress m_hostAddress;
// std::function<void(BIO*)> m_userHandler;
// const bool m_useTwoWayAuthentication;
// std::function<void(const std::string&)> m_logStream;

Networking::TCP::TLSClient
::TLSClient(NetAddress hostAddress, std::function<void(BIO*)> userHandler,
            bool useTwoWayAuthentication,
            std::function<void(const std::string&)> logStream)
  : m_sslContext{createContext(), [](SSL_CTX* ctx)
    {
      SSL_CTX_free(ctx);
    }}, m_hostAddress{hostAddress}, m_userHandler{userHandler},
    m_useTwoWayAuthentication{useTwoWayAuthentication},
    m_logStream{logStream}
{}

SSL_CTX*
Networking::TCP::TLSClient
::createContext()
{}

void Networking::TCP::TLSClient::connect()
{}


///////////////////////////////////////////////////////////////////////////////
