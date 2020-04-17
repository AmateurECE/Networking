///////////////////////////////////////////////////////////////////////////////
// NAME:            TLSException.cpp
//
// AUTHOR:          Ethan D. Twardy <edtwardy@mtu.edu>
//
// DESCRIPTION:     Implementation of the custom exception.
//
// CREATED:         04/05/2020
//
// LAST EDITED:     04/17/2020
////

#include <Networking/TCP/TLSException.h>

Networking::TCP::TLSException
::TLSException(std::string what, NetworkHost client)
  : m_what{what}, m_client{client}
{}

const char*
Networking::TCP::TLSException
::what() const noexcept
{
  return m_what.c_str();
}

const Networking::NetworkHost&
Networking::TCP::TLSException
::getClientAddress() const
{
  return m_client;
}

///////////////////////////////////////////////////////////////////////////////
