///////////////////////////////////////////////////////////////////////////////
// NAME:            NetworkAddress.h
//
// AUTHOR:          Ethan D. Twardy <edtwardy@mtu.edu>
//
// DESCRIPTION:     Class for encapsulating one single address specification.
//
// CREATED:         04/17/2020
//
// LAST EDITED:     05/01/2020
////

#ifndef __ET_NETWORKADDRESS__
#define __ET_NETWORKADDRESS__

#include <namespaces/Networking.h>

#include <netinet/in.h>

#include <string>

class Networking::NetworkAddress
{
public:
  NetworkAddress(struct sockaddr_in);
  NetworkAddress(unsigned int ipHostOrder, unsigned short portHostOrder);
  NetworkAddress(const std::string& ipAddress, unsigned short portHostOrder);

  unsigned int getIPHostOrder() const;
  std::string getIPDotNotation() const;
  unsigned short getPortHostOrder() const;
  const struct sockaddr_in& getSockAddr() const;  

  std::string string() const;

  bool operator==(const NetworkAddress&) const;
  bool operator!=(const NetworkAddress&) const;

private:
  struct sockaddr_in m_address;
};

#endif // __ET_NETWORKADDRESS__

///////////////////////////////////////////////////////////////////////////////
