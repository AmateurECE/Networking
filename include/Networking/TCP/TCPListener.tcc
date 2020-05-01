///////////////////////////////////////////////////////////////////////////////
// NAME:            TCPListener.tcc
//
// AUTHOR:          Ethan D. Twardy <edtwardy@mtu.edu>
//
// DESCRIPTION:     Implementation of the TCP Listener.
//
// CREATED:         04/02/2020
//
// LAST EDITED:     05/01/2020
////

#include <Networking/TCP/TCPListener.h>
#include <Networking/TCP/TCPRequest.h>

#include <fcntl.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <system_error>

template<class HostType>
Networking::TCP::TCPListener<HostType>
::TCPListener(HostType acceptedClients, unsigned int theBacklogSize,
              bool reuseAddress, bool blocking, bool maskSigPipe,
              std::function<void(unsigned int,const HostType&)> userHandler,
              std::function<void(const std::string&)> logStream)
  : m_listeningAddress{acceptedClients}, m_userHandler{userHandler},
    m_logStream{logStream}
{
  m_listeningSocket
    = std::shared_ptr<int>(new int, [maskSigPipe](int *pInt) {
        ::close(*pInt);
        if (maskSigPipe)
          {
            signal(SIGPIPE, SIG_DFL);
          }
        delete pInt;
      });

  errno = 0;
  if (maskSigPipe && SIG_ERR == signal(SIGPIPE, SIG_IGN))
    {
      throw std::system_error{errno, std::generic_category(),
          __FILE__ ": Could not mask SIGPIPE"};
    }

  *m_listeningSocket = getConfiguredSocket(reuseAddress, blocking);

  doBind();

  if (0 != ::listen(*m_listeningSocket, theBacklogSize))
    {
      throw std::system_error{errno, std::generic_category()};
    }
}

template<>
void Networking::TCP::TCPListener<Networking::NetworkHost>
::doBind() const
{
  const size_t sockLen = sizeof(struct sockaddr_in);
  for (auto const& address : m_listeningAddress)
    {
      if (0 == ::bind(*m_listeningSocket,
                      reinterpret_cast<const struct sockaddr*>
                      (&address.getSockAddr()),
                      sockLen))
        {
          return;
        }
    }
  throw std::system_error{errno, std::generic_category()};
}

template<>
void Networking::TCP::TCPListener<Networking::NetworkAddress>
::doBind() const
{
  const size_t sockLen = sizeof(struct sockaddr_in);
  if (-1 == ::bind(*m_listeningSocket,
                   reinterpret_cast<const struct sockaddr*>
                   (&m_listeningAddress.getSockAddr()),
                   sockLen))
    {
      throw std::system_error{errno, std::generic_category()};
    }
}

template<class HostType>
int Networking::TCP::TCPListener<HostType>
::getConfiguredSocket(bool reuseAddress, bool blocking) const
{
  errno = 0;
  int theSocket = 0;
  // TODO: Enable IPv6
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

template<class HostType>
std::unique_ptr<Networking::Interfaces::IRequest>
Networking::TCP::TCPListener<HostType>::listen()
{
  int receivingSocket = -1;
  struct sockaddr_in connectingEntity;
  size_t addrSize = sizeof(struct sockaddr_in);

  memset(&connectingEntity, 0, sizeof(connectingEntity));
  if (-1 == (receivingSocket = ::accept
             (*m_listeningSocket,
              reinterpret_cast<struct sockaddr*>(&connectingEntity),
              reinterpret_cast<socklen_t*>(&addrSize))))
    {
      throw std::system_error{errno, std::generic_category()};
    }

  try
    {
      return std::make_unique<TCP::TCPRequest<HostType>>
        (receivingSocket, HostType{connectingEntity}, m_userHandler,
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

template<class HostType>
Networking::TCP::TCPListener<HostType>::Builder
::Builder()
  : listeningAddress{"127.0.0.1", 80}
{}

template<class HostType>
typename Networking::TCP::TCPListener<HostType>::Builder
Networking::TCP::TCPListener<HostType>::Builder
::setListeningAddress(HostType theListeningAddress)
{ listeningAddress = theListeningAddress; return *this; }

template<class HostType>
typename Networking::TCP::TCPListener<HostType>::Builder
Networking::TCP::TCPListener<HostType>::Builder
::setBacklogSize(unsigned int theBacklogSize)
{ backlogSize = theBacklogSize; return *this; }

template<class HostType>
typename Networking::TCP::TCPListener<HostType>::Builder
Networking::TCP::TCPListener<HostType>::Builder
::setReuseAddress(bool isReuseAddress)
{ reuseAddress = isReuseAddress; return *this; }

template<class HostType>
typename Networking::TCP::TCPListener<HostType>::Builder
Networking::TCP::TCPListener<HostType>::Builder
::setBlocking(bool isBlocking)
{ blocking = isBlocking; return *this; }

template<class HostType>
typename Networking::TCP::TCPListener<HostType>::Builder
Networking::TCP::TCPListener<HostType>::Builder
::setMaskSigPipe(bool theMaskSigPipe)
{ maskSigPipe = theMaskSigPipe; return *this; }

template<class HostType>
typename Networking::TCP::TCPListener<HostType>::Builder
Networking::TCP::TCPListener<HostType>::Builder
::setUserHandler(UserHandler theUserHandler)
{ userHandler = theUserHandler; return *this; }

template<class HostType>
typename Networking::TCP::TCPListener<HostType>::Builder
Networking::TCP::TCPListener<HostType>::Builder
::setLogStream(std::function<void(const std::string&)> theLogStream)
{ logStream = theLogStream; return *this; }

template<class HostType>
typename Networking::TCP::TCPListener<HostType>
Networking::TCP::TCPListener<HostType>::Builder::build() const
{
  return TCPListener{listeningAddress, backlogSize, reuseAddress, blocking,
      maskSigPipe, userHandler, logStream};
}

///////////////////////////////////////////////////////////////////////////////
