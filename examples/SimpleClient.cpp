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
// LAST EDITED:     04/18/2020
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
  // Can also instantiate like this: NetAddress host{"127.0.0.1:13001"};
  NetworkAddress host{"127.0.0.1", 13001};
  auto client = TCP::TCPClient<NetworkAddress>{host, handler};
  client.connect();
}

///////////////////////////////////////////////////////////////////////////////
