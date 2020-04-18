///////////////////////////////////////////////////////////////////////////////
// NAME:            TCPClient.cpp
//
// AUTHOR:          Ethan D. Twardy <edtwardy@mtu.edu>
//
// DESCRIPTION:     Implementation of the TCP client.
//
// CREATED:         04/03/2020
//
// LAST EDITED:     04/18/2020
////

#include <Networking/TCP/TCPClient.h>

#include <system_error>

#include <sys/socket.h>
#include <unistd.h>

template<class HostType>
Networking::TCP::TCPClient<HostType>
::TCPClient(HostType hostAddress,
            std::function<void(int)> userHandler,
            std::function<void(const std::string&)> logStream)
  : m_socket{0}, m_hostAddress{hostAddress}, m_userHandler{userHandler},
    m_logStream{logStream}
{
  errno = 0;
  int socket= ::socket(PF_INET, SOCK_STREAM, 0);
  if (-1 == socket)
    {
      throw std::system_error{errno, std::generic_category()};
    }
  m_socket = std::shared_ptr<int>{new int, [](int* pInt){
      ::close(*pInt);
      delete pInt;
    }};
  *m_socket = socket;
}

template<class HostType>
void Networking::TCP::TCPClient<HostType>::connect()
{
  errno = 0;
  const struct sockaddr_in& hostAddress = getHostAddress();

  if (-1 == ::connect(*m_socket,
                      reinterpret_cast<const struct sockaddr*>(&hostAddress),
                      sizeof(struct sockaddr_in)))
    {
      throw std::system_error{errno, std::generic_category()};
    }

  m_userHandler(*m_socket);
}

// TODO: Write a specialization for NetworkHost
template<>
const struct sockaddr_in&
Networking::TCP::TCPClient<Networking::NetworkAddress>
::getHostAddress() const
{
  return m_hostAddress.getSockAddr();
}

///////////////////////////////////////////////////////////////////////////////
