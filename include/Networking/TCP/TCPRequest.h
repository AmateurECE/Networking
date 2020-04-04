///////////////////////////////////////////////////////////////////////////////
// NAME:            TCPRequest.h
//
// AUTHOR:          Ethan D. Twardy <edtwardy@mtu.edu>
//
// DESCRIPTION:     Encapsulates a TCP Connection Request.
//
// CREATED:         04/02/2020
//
// LAST EDITED:     04/04/2020
////

#ifndef __ET_TCPREQUEST__
#define __ET_TCPREQUEST__

#include <Networking/Interfaces/IRequest.h>
#include <Networking/NetAddress.h>

#include <functional>
#include <memory>
#include <ostream>

class Networking::TCP::TCPRequest : public Networking::Interfaces::IRequest
{
public:
  TCPRequest(int socket, NetAddress connectingAddress,
             std::function<void(unsigned int,const NetAddress&)>& userHandler,
             std::ostream& logStream);
  virtual void handle() final override;

private:
  std::shared_ptr<int> m_socket;
  NetAddress m_connectingAddress;
  std::function<void(unsigned int,const NetAddress&)>& m_userHandler;
  std::ostream& m_logStream;
};

#endif // __ET_TCPREQUEST__

///////////////////////////////////////////////////////////////////////////////
