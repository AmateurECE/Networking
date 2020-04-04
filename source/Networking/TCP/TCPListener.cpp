///////////////////////////////////////////////////////////////////////////////
// NAME:            TCPListener.cpp
//
// AUTHOR:          Ethan D. Twardy <edtwardy@mtu.edu>
//
// DESCRIPTION:     Implementation of the TCP Listener.
//
// CREATED:         04/02/2020
//
// LAST EDITED:     04/04/2020
////

#include <Networking/TCP/TCPListener.h>
#include <Networking/TCP/TCPRequest.h>

#include <fcntl.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <system_error>

Networking::TCP::TCPListener
::TCPListener(unsigned int theClientAddresses, unsigned short thePort,
              unsigned int theBacklogSize, bool reuseAddress, bool blocking,
              std::function<void(unsigned int,const sockaddr_in&)> userHandler,
              std::ostream& logStream)
  : m_listeningSocket{0}, m_listeningAddress{0, .sin_addr = {0}},
    m_userHandler{userHandler}, m_logStream{logStream}
{
  // Create a shared_ptr
  m_listeningSocket
    = std::shared_ptr<int>(new int, [](int *pInt) {
        ::close(*pInt);
        delete pInt;
      });
  *m_listeningSocket = getConfiguredSocket(reuseAddress, blocking);

  // TODO: Enable IPv6
  m_listeningAddress.sin_family = AF_INET;
  m_listeningAddress.sin_port = htons(thePort);
  m_listeningAddress.sin_addr.s_addr = htonl(theClientAddresses);
  errno = 0;
  if (-1 == ::bind(*m_listeningSocket,
                   reinterpret_cast<struct sockaddr*>(&m_listeningAddress),
                   sizeof(m_listeningAddress)))
    {
      throw std::system_error{errno, std::generic_category()};
    }

  if (0 != ::listen(*m_listeningSocket, theBacklogSize))
    {
      throw std::system_error{errno, std::generic_category()};
    }

  m_logStream << "TCPListener is bound and listening on port " << thePort;
}

int
Networking::TCP::TCPListener
::getConfiguredSocket(bool reuseAddress, bool blocking) const
{
  errno = 0;
  int theSocket = 0;
  if (-1 == (theSocket = ::socket(PF_INET, SOCK_STREAM, 0)))
    {
      throw std::system_error{errno, std::generic_category()};
    }

  // Set the socket to reuse local addresses (or not)
  int optVal = reuseAddress;
  if (-1 == ::setsockopt(theSocket, SOL_SOCKET, SO_REUSEADDR,
                         reinterpret_cast<const void*>(&optVal),
                         sizeof(optVal)))
    {
      throw std::system_error{errno, std::generic_category()};
    }

  // Set the socket to blocking/non-blocking
  if (!blocking && -1 == ::fcntl(theSocket, F_SETFL, O_NONBLOCK))
    {
      throw std::system_error{errno, std::generic_category()};
    }

  return theSocket;
}

std::unique_ptr<Networking::Interfaces::IRequest>
Networking::TCP::TCPListener::listen()
{
  int receivingSocket = -1;
  struct sockaddr_in connectingEntity = {0, .sin_family = 0};
  size_t addrSize = sizeof(struct sockaddr_in);

  // TODO: Don't throw if non-blocking and EAGAIN
  //   If the socket is configured to be non-blocking and there are no waiting
  //   connections on the queue, accept() returns with EAGAIN or EWOULDBLOCK.
  //   Don't throw in this scenario.
  if (-1 == (receivingSocket = ::accept
             (*m_listeningSocket,
              reinterpret_cast<struct sockaddr*>(&connectingEntity),
              reinterpret_cast<socklen_t*>(&addrSize))))
    {
      throw std::system_error{errno, std::generic_category()};
    }

  try
    {
      return std::make_unique<TCP::TCPRequest>(receivingSocket,
                                               connectingEntity,
                                               m_userHandler,
                                               m_logStream);
    }
  catch (const std::bad_alloc& e)
    {
      ::close(receivingSocket);
      throw;
    }
}

///////////////////////////////////////////////////////////////////////////////
// TCPListener::Builder
////


Networking::TCP::TCPListener::Builder
Networking::TCP::TCPListener::Builder
::setClientAddress(unsigned int theClientAddress)
{ clientAddress = theClientAddress; return *this; }

Networking::TCP::TCPListener::Builder
Networking::TCP::TCPListener::Builder
::setPort(unsigned short thePort)
{ port = thePort; return *this; }

Networking::TCP::TCPListener::Builder
Networking::TCP::TCPListener::Builder
::setBacklogSize(unsigned int theBacklogSize)
{ backlogSize = theBacklogSize; return *this; }

Networking::TCP::TCPListener::Builder
Networking::TCP::TCPListener::Builder
::setReuseAddress(bool isReuseAddress)
{ reuseAddress = isReuseAddress; return *this; }

Networking::TCP::TCPListener::Builder
Networking::TCP::TCPListener::Builder
::setBlocking(bool isBlocking)
{ blocking = isBlocking; return *this; }

Networking::TCP::TCPListener::Builder
Networking::TCP::TCPListener::Builder
::setUserHandler(std::function<void(unsigned int,
                                    const sockaddr_in&)> theUserHandler)
{ userHandler = theUserHandler; return *this; }

Networking::TCP::TCPListener
Networking::TCP::TCPListener::Builder::build(std::ostream& logStream) const
{
  return TCPListener{clientAddress, port, backlogSize, reuseAddress, blocking,
      userHandler, logStream};
}

///////////////////////////////////////////////////////////////////////////////
// TCPListener::Factory
////

Networking::TCP::TCPListener
Networking::TCP::TCPListener::Factory
::makeBlocking(unsigned short thePort, UserHandler userHandler,
               std::ostream& logStream)
{
  return Builder()
    .setPort(thePort)
    .setUserHandler(userHandler)
    .build(logStream);
}

Networking::TCP::TCPListener
Networking::TCP::TCPListener::Factory
::makeNonBlocking(unsigned short thePort, UserHandler userHandler,
                  std::ostream& logStream)
{
  return Builder()
    .setPort(thePort)
    .setBlocking(false)
    .setUserHandler(userHandler)
    .build(logStream);
}

///////////////////////////////////////////////////////////////////////////////
