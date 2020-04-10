///////////////////////////////////////////////////////////////////////////////
// NAME:            NetAddress.cpp
//
// AUTHOR:          Ethan D. Twardy <edtwardy@mtu.edu>
//
// DESCRIPTION:     Implementation of the NetAddress class.
//
// CREATED:         04/04/2020
//
// LAST EDITED:     04/10/2020
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

struct sockaddr_in
Networking::NetAddress
::getSockAddr(const std::string& ipOrHostname,
              unsigned short portHostOrder) const
{
  struct sockaddr_in address;
  memset(&address, 0, sizeof(address));
  address.sin_family = AF_INET;
  address.sin_port = htons(portHostOrder);

  int result = inet_pton(AF_INET, ipOrHostname.c_str(), &(address.sin_addr));
  if (1 != result)
    {
      throw std::invalid_argument{"Value \"" + ipOrHostname
          + "\" is not a valid IP address."};
    }

  return address;
}

Networking::NetAddress::NetAddress(const std::string& hostString)
{
  std::string::size_type colonIndex = std::string::npos;
  colonIndex = hostString.find(':');

  if (std::string::npos == colonIndex)
    {
      throw std::invalid_argument{"NetAddress(const std::string& hostString):"
          " hostString must be in the form"
          " \"<IPv4 | IPv6 | Hostname>:<port>\""};
    }

  int portNumberInt = std::stoi(hostString.substr(colonIndex + 1));
  if (portNumberInt > std::numeric_limits<unsigned short>::max())
    {
      throw std::out_of_range{"Value \"" + hostString.substr(colonIndex + 1)
          + "\" is not in the range of valid port numbers."};
    }

  m_address = getSockAddr(hostString.substr(0, colonIndex),
                          static_cast<unsigned short>(portNumberInt));
}

Networking::NetAddress::NetAddress(const std::string& ipOrHostname,
                                   unsigned short portHostOrder)
  : m_address{getSockAddr(ipOrHostname, portHostOrder)}
{}

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
