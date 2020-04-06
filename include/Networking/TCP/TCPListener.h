///////////////////////////////////////////////////////////////////////////////
// NAME:            TCPListener.h
//
// AUTHOR:          Ethan D. Twardy <edtwardy@mtu.edu>
//
// DESCRIPTION:     Listener for TCP connections.
//
// CREATED:         04/02/2020
//
// LAST EDITED:     04/05/2020
////

#ifndef __ET_TCPLISTENER__
#define __ET_TCPLISTENER__

#include <namespaces/Networking.h>

#include <Networking/Interfaces/IListener.h>

#include <netinet/in.h>

#include <functional>
#include <memory>
#include <iostream>

class Networking::TCP::TCPListener : public Networking::Interfaces::IListener
{
public:
  TCPListener(unsigned int theClientAddresses, unsigned short thePort,
              unsigned int theBacklogSize, bool reuseAddress, bool blocking,
              std::function<void(unsigned int,const NetAddress&)> userHandler,
              std::function<void(const std::string&)> logStream);

  virtual std::unique_ptr<Interfaces::IRequest> listen() final override;

  class Builder;

private:
  int getConfiguredSocket(bool reuseAddress, bool blocking) const;

  std::shared_ptr<int> m_listeningSocket;
  struct sockaddr_in m_listeningAddress;
  std::function<void(unsigned int,const NetAddress&)> m_userHandler;
  std::function<void(const std::string&)> m_logStream;
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
  using UserHandler = std::function<void(unsigned int,const NetAddress&)>;
  Builder setUserHandler(UserHandler userHandler);
  Builder setLogStream(std::function<void(const std::string&)> logStream);

  TCPListener build() const;

private:
  unsigned int clientAddress = INADDR_ANY;
  unsigned short port = 0;
  unsigned int backlogSize = 8;
  bool reuseAddress = true;
  bool blocking = true;
  UserHandler userHandler = [](unsigned int,const NetAddress&){ return; };
  std::function<void(const std::string&)> logStream =
    [](const std::string& message)
  {
    std::cerr << message << '\n';
  };
};

#endif // __ET_TCPLISTENER__

///////////////////////////////////////////////////////////////////////////////
