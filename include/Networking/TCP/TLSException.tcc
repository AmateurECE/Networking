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

template<class HostType>
Networking::TCP::TLSException<HostType>
::TLSException(std::string what, HostType client)
  : m_what{what}, m_client{client}
{}

template<class HostType>
const char* Networking::TCP::TLSException<HostType>
::what() const noexcept
{
  return m_what.c_str();
}

template<class HostType>
const HostType& Networking::TCP::TLSException<HostType>
::getClient() const
{
  return m_client;
}

///////////////////////////////////////////////////////////////////////////////
