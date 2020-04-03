///////////////////////////////////////////////////////////////////////////////
// NAME:            TCPListener.h
//
// AUTHOR:          Ethan D. Twardy <edtwardy@mtu.edu>
//
// DESCRIPTION:     Listener for TCP connections.
//
// CREATED:         04/02/2020
//
// LAST EDITED:     04/02/2020
////

#ifndef __ET_TCPLISTENER__
#define __ET_TCPLISTENER__

#include <namespaces/Networking.h>

#include <Networking/Interfaces/IListener.h>

#include <netinet/in.h>

#include <ostream>

class Networking::TCP::TCPListener : public Networking::Interfaces::IListener
{
public:
  TCPListener(unsigned short thePort, unsigned int theClientAddresses,
              unsigned int theBacklogSize, bool reuseAddress, bool blocking,
              std::function<void(unsigned int,const sockaddr_in&)> userHandler,
              std::ostream& logStream);
  // TODO: Make TCP::TCPListener compliant w/ Rule Of Three/Five
  virtual ~TCPListener();
  virtual std::unique_ptr<Interfaces::IRequest> listen() final override;

private:
  int getConfiguredSocket(bool reuseAddress, bool blocking) const;

  int m_listeningSocket;
  struct sockaddr_in m_listeningAddress;
  std::function<void(unsigned int,const sockaddr_in&)> m_userHandler;
  std::ostream& m_logStream;
};

#endif // __ET_TCPLISTENER__

///////////////////////////////////////////////////////////////////////////////
