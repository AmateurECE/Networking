///////////////////////////////////////////////////////////////////////////////
// NAME:            NetworkAddress.cpp
//
// AUTHOR:          Ethan D. Twardy <edtwardy@mtu.edu>
//
// DESCRIPTION:     Implementation of the NetworkAddress class.
//
// CREATED:         04/17/2020
//
// LAST EDITED:     04/18/2020
////

#include <Networking/NetworkAddress.h>

#include <arpa/inet.h>

#include <system_error>

#define str(x) _str(x)
#define _str(x) #x

Networking::NetworkAddress::NetworkAddress(struct sockaddr_in address)
  : m_address{address}
{}

Networking::NetworkAddress::NetworkAddress(unsigned int ipHostOrder,
                                           unsigned short portHostOrder)
{
  m_address = {};
  m_address.sin_family = AF_INET;
  m_address.sin_port = htons(portHostOrder);
  m_address.sin_addr.s_addr = htonl(ipHostOrder);
}

Networking::NetworkAddress
::NetworkAddress(const std::string& ipAddress,
                 unsigned short portHostOrder)
{
  m_address = {};
  m_address.sin_family = AF_INET;
  m_address.sin_port = htons(portHostOrder);

  int result = inet_pton(AF_INET, ipAddress.c_str(), &(m_address.sin_addr));
  if (1 != result)
    {
      throw std::invalid_argument{"Value \"" + ipAddress
          + "\" is not a valid IP address."};
    }
}

unsigned int Networking::NetworkAddress::getIPHostOrder() const
{
  return ntohl(m_address.sin_addr.s_addr);
}

std::string Networking::NetworkAddress::getIPDotNotation() const
{
  char nameBuffer[INET_ADDRSTRLEN];
  memset(nameBuffer, 0, sizeof(nameBuffer));
  if (nameBuffer != inet_ntop(PF_INET,
                              &(m_address.sin_addr.s_addr),
                              nameBuffer,
                              sizeof(nameBuffer)))
    {
      throw std::system_error{errno, std::generic_category(),
          __FILE__":" str(__LINE__) ": Call to inet_ntop failed"};
    }

  return std::string{const_cast<const char*>(nameBuffer)};
}

unsigned short Networking::NetworkAddress::getPortHostOrder() const
{
  return ntohs(m_address.sin_port);
}

const struct sockaddr_in& Networking::NetworkAddress::getSockAddr() const
{
  return const_cast<const sockaddr_in&>(m_address);
}

std::string Networking::NetworkAddress::string() const
{
  return "(" + getIPDotNotation() + ", " + std::to_string(getPortHostOrder())
    + ")";
}

bool Networking::NetworkAddress
::operator==(const Networking::NetworkAddress& that) const
{
  return
    m_address.sin_family == that.m_address.sin_family
    && m_address.sin_port == that.m_address.sin_port
    && m_address.sin_addr.s_addr == that.m_address.sin_addr.s_addr;
}

bool Networking::NetworkAddress
::operator!=(const Networking::NetworkAddress& that) const
{ return !operator==(that); }

///////////////////////////////////////////////////////////////////////////////
