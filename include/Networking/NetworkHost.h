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
// LAST EDITED:     04/18/2020
////

#ifndef __ET_NETADDRESS__
#define __ET_NETADDRESS__

#include <namespaces/Networking.h>
#include <Networking/NetworkAddress.h>

#include <iterator>
#include <list>
#include <string>

class Networking::NetworkHost
{
public:
  NetworkHost(NetworkAddress address);

  // Must be in the form "<IPv4 | IPv6 | Hostname>:<port>"
  // If a hostname is given, the ctor will attempt to resolve it using DNS.
  NetworkHost(std::string hostString);

  // ipOrHostname may be in the form IPv4 or IPv6 or Hostname
  // If a hostname is given, the ctor will attempt to resolve it using DNS.
  NetworkHost(std::string ipOrHostname, unsigned short portHostOrder);

  std::string getHostname() const;
  unsigned short getPortHostOrder() const;
  std::string string() const;

  class NetworkHostConstIter;
  typedef NetworkHostConstIter const_iterator;
  const_iterator begin() const;
  const_iterator end() const;
  const_iterator cbegin() const;
  const_iterator cend() const;

private:
  void getAddresses(std::string hostname, unsigned short portHostOrder);
  std::string nameLookup() const;

  std::list<NetworkAddress> m_addresses;
  std::string m_hostname;
};

class Networking::NetworkHost::NetworkHostConstIter
  : public virtual std::iterator<std::input_iterator_tag, NetworkAddress>
{
public:
  NetworkHostConstIter() = delete;
  ~NetworkHostConstIter() = default;
  NetworkHostConstIter(std::list<NetworkAddress>::const_iterator);
  NetworkHostConstIter(const NetworkHostConstIter&) = default;
  NetworkHostConstIter& operator=(const NetworkHostConstIter& that) = default;
  NetworkHostConstIter& operator++(); // Prefix increment
  NetworkHostConstIter operator++(int); // Postfix increment
  bool operator==(const NetworkHostConstIter& that) const;
  bool operator!=(const NetworkHostConstIter& that) const;
  NetworkAddress operator*() const;

private:
  std::list<NetworkAddress>::const_iterator m_iterator;
};

#endif // __ET_NETADDRESS__

///////////////////////////////////////////////////////////////////////////////
