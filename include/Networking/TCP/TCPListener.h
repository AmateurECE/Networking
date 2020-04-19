///////////////////////////////////////////////////////////////////////////////
// NAME:            TCPListener.h
//
// AUTHOR:          Ethan D. Twardy <edtwardy@mtu.edu>
//
// DESCRIPTION:     Listener for TCP connections.
//
// CREATED:         04/02/2020
//
// LAST EDITED:     04/19/2020
////

#ifndef __ET_TCPLISTENER__
#define __ET_TCPLISTENER__

#include <namespaces/Networking.h>

#include <Networking/Interfaces/IListener.h>

#include <functional>
#include <memory>
#include <iostream>

struct sockaddr;

template<class HostType>
class Networking::TCP::TCPListener : public Networking::Interfaces::IListener
{
public:
  TCPListener(HostType acceptedClients, unsigned int theBacklogSize,
              bool reuseAddress, bool blocking,
              std::function<void(unsigned int,const HostType&)> userHandler,
              std::function<void(const std::string&)> logStream);

  virtual std::unique_ptr<Interfaces::IRequest> listen() final override;

  class Builder;

private:
  int getConfiguredSocket(bool reuseAddress, bool blocking) const;
  const struct sockaddr* getSockAddr() const;

  HostType m_listeningAddress;
  std::function<void(unsigned int,const HostType&)> m_userHandler;
  std::function<void(const std::string&)> m_logStream;
  std::shared_ptr<int> m_listeningSocket;
};

template<class HostType>
class Networking::TCP::TCPListener<HostType>::Builder
{
public:
  Builder();
  Builder setListeningAddress(HostType);
  Builder setBacklogSize(unsigned int);
  Builder setReuseAddress(bool);
  Builder setBlocking(bool);
  using UserHandler = std::function<void(unsigned int,const HostType&)>;
  Builder setUserHandler(UserHandler userHandler);
  Builder setLogStream(std::function<void(const std::string&)> logStream);

  TCPListener build() const;

private:
  HostType listeningAddress;
  unsigned int backlogSize = 8;
  bool reuseAddress = true;
  bool blocking = true;
  UserHandler userHandler = [](unsigned int,const HostType&){ return; };
  std::function<void(const std::string&)> logStream =
    [](const std::string& message)
  {
    std::cerr << message << '\n';
  };
};

#include <Networking/TCP/TCPListener.tcc>

#endif // __ET_TCPLISTENER__

///////////////////////////////////////////////////////////////////////////////
