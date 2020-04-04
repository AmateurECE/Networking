///////////////////////////////////////////////////////////////////////////////
// NAME:            NetAddress.h
//
// AUTHOR:          Ethan D. Twardy <edtwardy@mtu.edu>
//
// DESCRIPTION:     Utility class encapsulating useful logic for dealing with
//                  inet addresses.
//
// CREATED:         04/04/2020
//
// LAST EDITED:     04/04/2020
////

#ifndef __ET_NETADDRESS__
#define __ET_NETADDRESS__

#include <namespaces/Networking.h>

#include <netinet/in.h>

#include <string>

class Networking::NetAddress
{
public:
  NetAddress(struct sockaddr_in);
  NetAddress(const std::string& ipOrHostname);
  NetAddress(unsigned int ipHostOrder, unsigned short portHostOrder);

  unsigned int getIPHostOrder() const;
  std::string getIPDotNotation() const;
  unsigned short getPortHostOrder() const;
  const struct sockaddr_in& getSockAddr() const;

private:
  struct sockaddr_in m_address;
};

#endif // __ET_NETADDRESS__

///////////////////////////////////////////////////////////////////////////////
