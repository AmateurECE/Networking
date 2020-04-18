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

Networking::NetworkHost::NetworkHost(NetworkAddress address)
  : m_addresses{address}
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
        (getNetworkAddress(hostString.substr(0, colonIndex),
                           static_cast<unsigned short>(portNumberInt)));
    }
  catch (const std::invalid_argument& e)
    {
      getAddresses(hostString.substr(0, colonIndex),
                   static_cast<unsigned short>(portNumberInt));
    }
}

Networking::NetworkHost
::NetworkHost(std::string ipOrHostname, unsigned short portHostOrder)
  : m_addresses{}
{
  try
    {
      m_addresses.push_back(getNetworkAddress(ipOrHostname, portHostOrder));
    }
  catch (const std::invalid_argument& e)
    {
      getAddresses(ipOrHostname, portHostOrder);
    }
}

std::string Networking::NetworkHost::getHostname() const
{}

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

void Networking::NetworkHost
::getAddresses(std::string hostname, unsigned short portHostOrder)
{}

///////////////////////////////////////////////////////////////////////////////
