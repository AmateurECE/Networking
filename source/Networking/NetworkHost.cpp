///////////////////////////////////////////////////////////////////////////////
// NAME:            NetworkHost.cpp
//
// AUTHOR:          Ethan D. Twardy <edtwardy@mtu.edu>
//
// DESCRIPTION:     Implementation of the NetworkHost class.
//
// CREATED:         04/04/2020
//
// LAST EDITED:     04/18/2020
////

#include <Networking/NetworkHost.h>

#include <system_error>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#define str(x) _str(x)
#define _str(x) #x

Networking::NetworkHost::NetworkHost(NetworkAddress address)
  : m_addresses{address}, m_hostname{nameLookup()}
{}

Networking::NetworkHost::NetworkHost(std::string hostString)
{
  std::string::size_type colonIndex = std::string::npos;
  colonIndex = hostString.find(':');

  if (std::string::npos == colonIndex)
    {
      throw std::invalid_argument{"NetworkHost(const std::string& hostString):"
          " hostString must be in the form"
          " \"<IPv4 | IPv6 | Hostname>:<port>\""};
    }

  int portNumberInt = std::stoi(hostString.substr(colonIndex + 1));
  if (portNumberInt > std::numeric_limits<unsigned short>::max())
    {
      throw std::out_of_range{"Value \"" + hostString.substr(colonIndex + 1)
          + "\" is not in the range of valid port numbers."};
    }

  try
    {
      m_addresses.push_back
        (NetworkAddress(hostString.substr(0, colonIndex),
                        static_cast<unsigned short>(portNumberInt)));
    }
  catch (const std::invalid_argument& e)
    {
      getAddresses(hostString.substr(0, colonIndex),
                   static_cast<unsigned short>(portNumberInt));
    }

  m_hostname = nameLookup();
}

Networking::NetworkHost
::NetworkHost(std::string ipOrHostname, unsigned short portHostOrder)
  : m_addresses{}
{
  try
    {
      m_addresses.push_back(NetworkAddress(ipOrHostname, portHostOrder));
    }
  catch (const std::invalid_argument& e)
    {
      getAddresses(ipOrHostname, portHostOrder);
    }

  m_hostname = nameLookup();
}

unsigned short Networking::NetworkHost::getPortHostOrder() const
{
  return (*cbegin()).getPortHostOrder();
}

std::string Networking::NetworkHost::string() const
{
  return "(" + getHostname() + ", " + std::to_string(getPortHostOrder()) + ")";
}

Networking::NetworkHost::const_iterator
Networking::NetworkHost::cbegin() const
{
  return const_iterator{m_addresses.cbegin()};
}

Networking::NetworkHost::const_iterator
Networking::NetworkHost::cend() const
{
  return const_iterator{m_addresses.cend()};
}

Networking::NetworkHost::const_iterator
Networking::NetworkHost::begin() const
{
  return cbegin();
}

Networking::NetworkHost::const_iterator
Networking::NetworkHost::end() const
{
  return cend();
}

std::string Networking::NetworkHost::getHostname() const
{
  return m_hostname;
}

std::string Networking::NetworkHost::nameLookup() const
{
  if (0 == m_addresses.size())
    {
      throw std::logic_error{__FILE__ ":" str(__LINE__) ": Cannot perform name"
          " lookup for an empty IP address."};
    }

  char hostBuffer[NI_MAXHOST + 1];
  memset(hostBuffer, 0, sizeof(hostBuffer));
  const struct sockaddr& address = reinterpret_cast<const struct sockaddr&>
    ((*cbegin()).getSockAddr());
  int result = getnameinfo(&address, sizeof(struct sockaddr_in), hostBuffer,
                           NI_MAXHOST, NULL, 0, NI_NAMEREQD);
  if (0 == result)
    {
      return std::string{hostBuffer};
    }
  else if (EAI_NONAME && 1 == m_addresses.size())
    {
      if ('\0' != hostBuffer[0])
        {
          return std::string{hostBuffer};
        }
      else
        {
          return (*cbegin()).getIPDotNotation();
        }
    }
  else
    {
      throw std::system_error{result, std::generic_category(),
          gai_strerror(result)};
    }
}

// TODO: Consider also supporting getaddrinfo_a?
// TODO: Consider adding IDN support?
void Networking::NetworkHost
::getAddresses(std::string hostname, unsigned short portHostOrder)
{
  struct addrinfo hints = {}, *response = NULL;
  hints.ai_family = AF_INET; // We don't currently support IPv6.

  errno = 0;
  int status = getaddrinfo(hostname.c_str(), NULL, &hints, &response);
  if (0 != status)
    {
      throw std::system_error{errno, std::generic_category(),
          gai_strerror(status)};
    }

  for (struct addrinfo* element = response; NULL != element;
       element = element->ai_next)
    {
      struct sockaddr_in& address
        = reinterpret_cast<struct sockaddr_in&>(*(element->ai_addr));
      address.sin_port = htons(portHostOrder);
      m_addresses.push_back
        (NetworkAddress{const_cast<const struct sockaddr_in&>(address)});
    }

  freeaddrinfo(response);
}

///////////////////////////////////////////////////////////////////////////////
// NetworkHostConstIter
////

// TODO: Look for using decls in template headers.
using ListIterator = std::list<Networking::NetworkAddress>::const_iterator;
Networking::NetworkHost::NetworkHostConstIter
::NetworkHostConstIter(ListIterator iterator)
  : m_iterator{iterator}
{}

Networking::NetworkAddress
Networking::NetworkHost::NetworkHostConstIter::operator*() const
{ return *m_iterator; }

Networking::NetworkHost::NetworkHostConstIter&
Networking::NetworkHost::NetworkHostConstIter
::operator++() // Prefix increment
{ ++m_iterator; return *this; }

Networking::NetworkHost::NetworkHostConstIter
Networking::NetworkHost::NetworkHostConstIter
::operator++(int) // Postfix increment
{ return {m_iterator++}; }

bool Networking::NetworkHost::NetworkHostConstIter
::operator==(const NetworkHostConstIter& that) const
{ return *m_iterator == *(that.m_iterator); }

bool Networking::NetworkHost::NetworkHostConstIter
::operator!=(const NetworkHostConstIter& that) const
{ return *m_iterator != *(that.m_iterator); }

///////////////////////////////////////////////////////////////////////////////
