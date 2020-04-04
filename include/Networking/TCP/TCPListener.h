///////////////////////////////////////////////////////////////////////////////
// NAME:            TCPListener.h
//
// AUTHOR:          Ethan D. Twardy <edtwardy@mtu.edu>
//
// DESCRIPTION:     Listener for TCP connections.
//
// CREATED:         04/02/2020
//
// LAST EDITED:     04/04/2020
////

#ifndef __ET_TCPLISTENER__
#define __ET_TCPLISTENER__

#include <namespaces/Networking.h>

#include <Networking/Interfaces/IListener.h>

#include <netinet/in.h>

#include <memory>
#include <iostream>

class Networking::TCP::TCPListener : public Networking::Interfaces::IListener
{
public:
  TCPListener(unsigned int theClientAddresses, unsigned short thePort,
              unsigned int theBacklogSize, bool reuseAddress, bool blocking,
              // TODO: Make the unsigned int param to userHandler const
              std::function<void(unsigned int,const sockaddr_in&)> userHandler,
              std::ostream& logStream);

  virtual std::unique_ptr<Interfaces::IRequest> listen() final override;

  class Builder;
  class Factory;

private:
  int getConfiguredSocket(bool reuseAddress, bool blocking) const;

  std::shared_ptr<int> m_listeningSocket;
  struct sockaddr_in m_listeningAddress;
  std::function<void(unsigned int,const sockaddr_in&)> m_userHandler;
  std::ostream& m_logStream;
};

class Networking::TCP::TCPListener::Builder
{
public:
  Builder() = default;
  Builder setClientAddress(unsigned int);
  Builder setPort(unsigned short);
  Builder setBacklogSize(unsigned int);
  Builder setReuseAddress(bool);
  Builder setBlocking(bool);
  using UserHandler = std::function<void(unsigned int,const sockaddr_in&)>;
  Builder setUserHandler(UserHandler userHandler);

  TCPListener build(std::ostream& logStream) const;

private:
  unsigned int clientAddress = INADDR_ANY;
  unsigned short port = 0;
  unsigned int backlogSize = 8;
  bool reuseAddress = true;
  bool blocking = true;
  UserHandler userHandler = [](unsigned int,const sockaddr_in&){ return; };
};

class Networking::TCP::TCPListener::Factory
{
public:
  Factory() = default;
  using UserHandler = std::function<void(unsigned int,const sockaddr_in&)>;
  static Networking::TCP::TCPListener makeBlocking(unsigned short thePort,
                                                   UserHandler userHandler,
                                                   std::ostream& logStream);
  static Networking::TCP::TCPListener makeNonBlocking(unsigned short thePort,
                                                   UserHandler userHandler,
                                                   std::ostream& logStream);
};

#endif // __ET_TCPLISTENER__

///////////////////////////////////////////////////////////////////////////////
