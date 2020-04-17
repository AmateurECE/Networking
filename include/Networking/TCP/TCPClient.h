///////////////////////////////////////////////////////////////////////////////
// NAME:            TCPClient.h
//
// AUTHOR:          Ethan D. Twardy <edtwardy@mtu.edu>
//
// DESCRIPTION:     Class that implements a TCP Client.
//
// CREATED:         04/03/2020
//
// LAST EDITED:     04/17/2020
////

#ifndef __ET_TCPCLIENT__
#define __ET_TCPCLIENT__

#include <namespaces/Networking.h>
#include <Networking/NetworkHost.h>

#include <functional>
#include <iostream>
#include <memory>

class Networking::TCP::TCPClient
{
public:
  TCPClient(NetworkHost hostAddress,
            std::function<void(int)> userHandler,
            // By default, simply send error messages to cerr.
            std::function<void(const std::string&)> logStream
            =[](const std::string& message)
              {
                std::cerr << message << '\n';
              });
  void connect();

private:
  std::shared_ptr<int> m_socket;
  NetworkHost m_hostAddress;
  std::function<void(int)> m_userHandler;
  std::function<void(const std::string&)> m_logStream;
};

#endif // __ET_TCPCLIENT__

///////////////////////////////////////////////////////////////////////////////
