///////////////////////////////////////////////////////////////////////////////
// NAME:            TCPClient.h
//
// AUTHOR:          Ethan D. Twardy <edtwardy@mtu.edu>
//
// DESCRIPTION:     Class that implements a TCP Client.
//
// CREATED:         04/03/2020
//
// LAST EDITED:     04/03/2020
////

#ifndef __ET_TCPCLIENT__
#define __ET_TCPCLIENT__

#include <namespaces/Networking.h>

#include <functional>

#include <netinet/in.h>

class Networking::TCP::TCPClient
{
public:
  TCPClient(unsigned int theHostAddress, unsigned short thePort,
            std::function<void(int)> userHandler, std::ostream& logStream);
  // TODO: Make TCPClient compliant w/ Rule Of Three/Five.
  // TODO: TCPClient factory class
  // TODO: Allow non-blocking configuration for TCPClient
  ~TCPClient();
  void connect();

private:
  int m_socket;
  struct sockaddr_in m_hostAddress;
  std::function<void(int)> m_userHandler;
  std::ostream& m_logStream;
};

#endif // __ET_TCPCLIENT__

///////////////////////////////////////////////////////////////////////////////
