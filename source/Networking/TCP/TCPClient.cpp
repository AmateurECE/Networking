///////////////////////////////////////////////////////////////////////////////
// NAME:            TCPClient.cpp
//
// AUTHOR:          Ethan D. Twardy <edtwardy@mtu.edu>
//
// DESCRIPTION:     Implementation of the TCP client.
//
// CREATED:         04/03/2020
//
// LAST EDITED:     04/03/2020
////

#include <Networking/TCP/TCPClient.h>

#include <system_error>

#include <sys/socket.h>
#include <unistd.h>

Networking::TCP::TCPClient::TCPClient(unsigned int theHostAddress,
                                      unsigned short thePort,
                                      std::function<void(int)> userHandler,
                                      std::ostream& logStream)
  : m_socket{0}, m_hostAddress{0, .sin_family=0}, m_userHandler{userHandler},
    m_logStream{logStream}
{
  errno = 0;
  m_socket = ::socket(PF_INET, SOCK_STREAM, 0);
  if (-1 == m_socket)
    {
      throw std::system_error{errno, std::generic_category()};
    }

  // TODO: Overload ctor to allow hostname/ipstr instantiation.
  m_hostAddress.sin_family = AF_INET;
  m_hostAddress.sin_port = htons(thePort);
  m_hostAddress.sin_addr.s_addr = htonl(theHostAddress);
}

Networking::TCP::TCPClient::~TCPClient()
{
  ::close(m_socket);
}

void Networking::TCP::TCPClient::connect()
{
  errno = 0;
  if (-1 == ::connect(m_socket,
                      reinterpret_cast<struct sockaddr*>(&m_hostAddress),
                      sizeof(m_hostAddress)))
    {
      throw std::system_error{errno, std::generic_category()};
    }
  m_userHandler(m_socket);
}

///////////////////////////////////////////////////////////////////////////////
