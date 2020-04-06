///////////////////////////////////////////////////////////////////////////////
// NAME:            SimpleClient.cpp
//
// AUTHOR:          Ethan D. Twardy <edtwardy@mtu.edu>
//
// DESCRIPTION:     A simple example of using the library to deploy a TCP
//                  client.
//
// CREATED:         04/03/2020
//
// LAST EDITED:     04/05/2020
////

#include <Networking/TCP/TCPClient.h>

#include <iostream>

int main()
{
  auto handler = [](int socket)
    {
      std::cerr << "Socket connected.";
    };

  using namespace Networking;
  NetAddress host{INADDR_LOOPBACK, 13001};
  TCP::TCPClient client = TCP::TCPClient{host, handler};
  client.connect();
}

///////////////////////////////////////////////////////////////////////////////
