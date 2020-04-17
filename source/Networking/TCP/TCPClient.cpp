///////////////////////////////////////////////////////////////////////////////
// NAME:            TCPClient.cpp
//
// AUTHOR:          Ethan D. Twardy <edtwardy@mtu.edu>
//
// DESCRIPTION:     Implementation of the TCP client.
//
// CREATED:         04/03/2020
//
// LAST EDITED:     04/17/2020
////

#include <Networking/TCP/TCPClient.h>

#include <system_error>

#include <sys/socket.h>
#include <unistd.h>

Networking::TCP::TCPClient
::TCPClient(NetworkHost hostAddress,
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

void Networking::TCP::TCPClient::connect()
{
  errno = 0;
  const struct sockaddr_in& hostAddress = m_hostAddress.getSockAddr();

  if (-1 == ::connect(*m_socket,
                      reinterpret_cast<const struct sockaddr*>(&hostAddress),
                      sizeof(struct sockaddr_in)))
    {
      throw std::system_error{errno, std::generic_category()};
    }

  m_logStream("Successfully connected to ("
              + m_hostAddress.getIPDotNotation() + ", "
              + std::to_string(m_hostAddress.getPortHostOrder()) + ")");
  m_userHandler(*m_socket);
}

///////////////////////////////////////////////////////////////////////////////
