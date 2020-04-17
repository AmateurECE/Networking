///////////////////////////////////////////////////////////////////////////////
// NAME:            NetworkHost.h
//
// AUTHOR:          Ethan D. Twardy <edtwardy@mtu.edu>
//
// DESCRIPTION:     Utility class encapsulating useful logic for dealing with
//                  inet addresses.
//
// CREATED:         04/04/2020
//
// LAST EDITED:     04/17/2020
////

#ifndef __ET_NETADDRESS__
#define __ET_NETADDRESS__

#include <namespaces/Networking.h>

#include <netinet/in.h>

#include <string>

class Networking::NetworkHost
{
public:
  NetworkHost(struct sockaddr_in);

  // Must be in the form "<IPv4 | IPv6 | Hostname>:<port>"
  // If a hostname is given, the ctor will attempt to resolve it using DNS.
  NetworkHost(const std::string& hostString);

  // ipOrHostname may be in the form IPv4 or IPv6 or Hostname
  // If a hostname is given, the ctor will attempt to resolve it using DNS.
  NetworkHost(const std::string& ipOrHostname, unsigned short portHostOrder);

  NetworkHost(unsigned int ipHostOrder, unsigned short portHostOrder);

  unsigned int getIPHostOrder() const;
  std::string getIPDotNotation() const;
  unsigned short getPortHostOrder() const;
  const struct sockaddr_in& getSockAddr() const;

private:
  struct sockaddr_in m_address;

  struct sockaddr_in getSockAddr(const std::string& ipOrHostname,
                                 unsigned short portHostOrder) const;
};

#endif // __ET_NETADDRESS__

///////////////////////////////////////////////////////////////////////////////
