///////////////////////////////////////////////////////////////////////////////
// NAME:            TCPRequest.h
//
// AUTHOR:          Ethan D. Twardy <edtwardy@mtu.edu>
//
// DESCRIPTION:     Encapsulates a TCP Connection Request.
//
// CREATED:         04/02/2020
//
// LAST EDITED:     04/17/2020
////

#ifndef __ET_TCPREQUEST__
#define __ET_TCPREQUEST__

#include <Networking/Interfaces/IRequest.h>
#include <Networking/NetworkHost.h>

#include <functional>
#include <memory>
#include <ostream>

template<class HostType>
class Networking::TCP::TCPRequest : public Networking::Interfaces::IRequest
{
public:
  TCPRequest(int socket, HostType connectingAddress,
             std::function<void(unsigned int,const HostType&)>& userHandler,
             std::function<void(const std::string&)> logStream);
  virtual void handle() final override;

private:
  std::shared_ptr<int> m_socket;
  HostType m_connectingAddress;
  std::function<void(unsigned int,const HostType&)>& m_userHandler;
  std::function<void(const std::string&)> m_logStream;
};

#include <Networking/TCP/TCPRequest.tcc>

#endif // __ET_TCPREQUEST__

///////////////////////////////////////////////////////////////////////////////
