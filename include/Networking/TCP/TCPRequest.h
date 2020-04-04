///////////////////////////////////////////////////////////////////////////////
// NAME:            TCPRequest.h
//
// AUTHOR:          Ethan D. Twardy <edtwardy@mtu.edu>
//
// DESCRIPTION:     Encapsulates a TCP Connection Request.
//
// CREATED:         04/02/2020
//
// LAST EDITED:     04/03/2020
////

#ifndef __ET_TCPREQUEST__
#define __ET_TCPREQUEST__

#include <Networking/Interfaces/IRequest.h>

#include <netinet/in.h>

#include <functional>
#include <memory>
#include <ostream>

class Networking::TCP::TCPRequest : public Networking::Interfaces::IRequest
{
public:
  TCPRequest(int socket, struct sockaddr_in connectingAddress,
             std::function<void(unsigned int,const sockaddr_in&)>& userHandler,
             std::ostream& logStream);
  virtual void handle() final override;

private:
  std::shared_ptr<int> m_socket;
  struct sockaddr_in m_connectingAddress;
  std::function<void(unsigned int,const sockaddr_in&)>& m_userHandler;
  std::ostream& m_logStream;
};

#endif // __ET_TCPREQUEST__

///////////////////////////////////////////////////////////////////////////////
