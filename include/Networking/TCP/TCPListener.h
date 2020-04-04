///////////////////////////////////////////////////////////////////////////////
// NAME:            TCPListener.h
//
// AUTHOR:          Ethan D. Twardy <edtwardy@mtu.edu>
//
// DESCRIPTION:     Listener for TCP connections.
//
// CREATED:         04/02/2020
//
// LAST EDITED:     04/03/2020
////

#ifndef __ET_TCPLISTENER__
#define __ET_TCPLISTENER__

#include <namespaces/Networking.h>

#include <Networking/Interfaces/IListener.h>

#include <netinet/in.h>

#include <memory>
#include <ostream>

class Networking::TCP::TCPListener : public Networking::Interfaces::IListener
{
public:
  TCPListener(unsigned int theClientAddresses, unsigned short thePort,
              unsigned int theBacklogSize, bool reuseAddress, bool blocking,
              std::function<void(unsigned int,const sockaddr_in&)> userHandler,
              std::ostream& logStream);

  virtual std::unique_ptr<Interfaces::IRequest> listen() final override;
  // TODO: Make TCPListener Builder/Factory class.

private:
  int getConfiguredSocket(bool reuseAddress, bool blocking) const;

  std::shared_ptr<int> m_listeningSocket;
  struct sockaddr_in m_listeningAddress;
  std::function<void(unsigned int,const sockaddr_in&)> m_userHandler;
  std::ostream& m_logStream;
};

#endif // __ET_TCPLISTENER__

///////////////////////////////////////////////////////////////////////////////
