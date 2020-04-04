///////////////////////////////////////////////////////////////////////////////
// NAME:            TCPClient.h
//
// AUTHOR:          Ethan D. Twardy <edtwardy@mtu.edu>
//
// DESCRIPTION:     Class that implements a TCP Client.
//
// CREATED:         04/03/2020
//
// LAST EDITED:     04/04/2020
////

#ifndef __ET_TCPCLIENT__
#define __ET_TCPCLIENT__

#include <namespaces/Networking.h>
#include <Networking/NetAddress.h>

#include <functional>
#include <memory>

class Networking::TCP::TCPClient
{
public:
  TCPClient(NetAddress hostAddress,
            std::function<void(int)> userHandler,
            std::function<void(const std::string&)> logStream);
  // TODO: TCPClient factory class
  // TODO: Allow non-blocking configuration for TCPClient
  void connect();

private:
  std::shared_ptr<int> m_socket;
  NetAddress m_hostAddress;
  std::function<void(int)> m_userHandler;
  std::function<void(const std::string&)> m_logStream;
};

#endif // __ET_TCPCLIENT__

///////////////////////////////////////////////////////////////////////////////
