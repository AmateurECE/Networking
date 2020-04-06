///////////////////////////////////////////////////////////////////////////////
// NAME:            ClientFailedTLSHandshakeException.cpp
//
// AUTHOR:          Ethan D. Twardy <edtwardy@mtu.edu>
//
// DESCRIPTION:     Implementation of the custom exception.
//
// CREATED:         04/05/2020
//
// LAST EDITED:     04/05/2020
////

#include <Networking/TCP/ClientFailedTLSHandshakeException.h>

Networking::TCP::ClientFailedTLSHandshakeException
::ClientFailedTLSHandshakeException(std::string what, NetAddress client)
  : m_what{"Client failed TLS Handshake: " + what}, m_client{client}
{}

const char*
Networking::TCP::ClientFailedTLSHandshakeException
::what() const noexcept
{
  return m_what.c_str();
}

const Networking::NetAddress&
Networking::TCP::ClientFailedTLSHandshakeException
::getClientAddress() const
{
  return m_client;
}

///////////////////////////////////////////////////////////////////////////////
