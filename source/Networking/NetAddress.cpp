///////////////////////////////////////////////////////////////////////////////
// NAME:            NetAddress.cpp
//
// AUTHOR:          Ethan D. Twardy <edtwardy@mtu.edu>
//
// DESCRIPTION:     Implementation of the NetAddress class.
//
// CREATED:         04/04/2020
//
// LAST EDITED:     04/04/2020
////

#include <Networking/NetAddress.h>

#include <arpa/inet.h>
#include <string.h>

#include <system_error>

#define str(x) str_Impl(x)
#define str_Impl(x) #x

Networking::NetAddress::NetAddress(struct sockaddr_in address)
  : m_address{address}
{}

Networking::NetAddress::NetAddress(const std::string& ipOrHostname)
{
  throw std::logic_error{__FILE__":" str(__LINE__) ": Unimplemented"
      " constructor NetAddress(const std::string&)"};
}

Networking::NetAddress::NetAddress(unsigned int ipHostOrder,
                                   unsigned short portHostOrder)
{
  memset(&m_address, 0, sizeof(m_address));
  m_address.sin_family = AF_INET;
  m_address.sin_addr.s_addr = htonl(ipHostOrder);
  m_address.sin_port = htons(portHostOrder);
}

unsigned int Networking::NetAddress::getIPHostOrder() const
{
  return ntohl(m_address.sin_addr.s_addr);
}

std::string Networking::NetAddress::getIPDotNotation() const
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

unsigned short Networking::NetAddress::getPortHostOrder() const
{
  return ntohs(m_address.sin_port);
}

const struct sockaddr_in& Networking::NetAddress::getSockAddr() const
{
  return const_cast<const sockaddr_in&>(m_address);
}

///////////////////////////////////////////////////////////////////////////////
