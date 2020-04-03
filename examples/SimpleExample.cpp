///////////////////////////////////////////////////////////////////////////////
// NAME:            SimpleExample.cpp
//
// AUTHOR:          Ethan D. Twardy <edtwardy@mtu.edu>
//
// DESCRIPTION:     A Simple Example of using the library to deploy a TCP
//                  server.
//
// CREATED:         04/02/2020
//
// LAST EDITED:     04/02/2020
////

#include <Networking/BlockingServer.h>
#include <Networking/DelegatorSTSP.h>
#include <Networking/TCP/TCPListener.h>

#include <arpa/inet.h>

#include <iostream>
#include <memory>

int main()
{
  // The user handler. Called whenever a connection is received.
  auto closure = [](int socket, const sockaddr_in& connectingAddress)
    {
      char nameBuffer[INET_ADDRSTRLEN];
      memset(nameBuffer, 0, sizeof(nameBuffer));
      if (nameBuffer != inet_ntop(PF_INET,
                                  &(connectingAddress.sin_addr.s_addr),
                                  nameBuffer,
                                  sizeof(nameBuffer)))
        {
          throw std::system_error{errno, std::generic_category(),
              "inet_ntop failed"};
        }
      fprintf(stderr, "Received connection from (%s, %5i)\n", nameBuffer,
              ntohs(connectingAddress.sin_port));
    };

  using namespace Networking;
  std::unique_ptr<Interfaces::IListener> listener
    = std::make_unique<TCP::TCPListener>(13001, INADDR_LOOPBACK, 8, true, true,
                                         closure, std::cerr);
  std::unique_ptr<Interfaces::IDelegator> delegator
    = std::make_unique<DelegatorSTSP>();
  BlockingServer server = BlockingServer{std::move(delegator),
                                         std::move(listener)};
  server.start();
}

///////////////////////////////////////////////////////////////////////////////
