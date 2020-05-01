///////////////////////////////////////////////////////////////////////////////
// NAME:            TCPClient.tcc
//
// AUTHOR:          Ethan D. Twardy <edtwardy@mtu.edu>
//
// DESCRIPTION:     Implementation of the TCP client.
//
// CREATED:         04/03/2020
//
// LAST EDITED:     05/01/2020
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
  // TODO: Enable IPv6
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

template<>
void Networking::TCP::TCPClient<Networking::NetworkAddress>::connect()
{
  errno = 0;
  const struct sockaddr_in& hostAddress = m_hostAddress.getSockAddr();

  if (-1 == ::connect(*m_socket,
                      reinterpret_cast<const struct sockaddr*>(&hostAddress),
                      sizeof(struct sockaddr_in)))
    {
      throw std::system_error{errno, std::generic_category()};
    }

  m_userHandler(*m_socket);
}

template<>
void Networking::TCP::TCPClient<Networking::NetworkHost>::connect()
{
  std::string errorStack = "Host Connect Failures:";

  for (auto const& address : m_hostAddress)
    {
      const struct sockaddr& socketAddress
        = reinterpret_cast<const struct sockaddr&>(address.getSockAddr());
      errno = 0;
      if (-1 == ::connect(*m_socket, &socketAddress,
                          sizeof(struct sockaddr_in)))
        {
          errorStack += "\n" + address.string() + " (errno="
            + std::to_string(errno) + ") " + std::string{strerror(errno)};
        }
      else
        {
          m_userHandler(*m_socket);
          return;
        }
    }

  throw std::system_error{errno, std::generic_category(), errorStack};
}

///////////////////////////////////////////////////////////////////////////////
