///////////////////////////////////////////////////////////////////////////////
// NAME:            SimpleServer.cpp
//
// AUTHOR:          Ethan D. Twardy <edtwardy@mtu.edu>
//
// DESCRIPTION:     A Simple Example of using the library to deploy a TCP
//                  server. Compile using:
//
//                      g++ -g -Wall -Wextra -O0 --std=c++17
//                          examples/SimpleExample.cpp -I include/
//                          -lnetworking -L build/ -o TestBinary
//
// CREATED:         04/02/2020
//
// LAST EDITED:     04/05/2020
////

#include <Networking/BlockingServer.h>
#include <Networking/DelegatorSTSP.h>
#include <Networking/NetAddress.h>
#include <Networking/TCP/TCPListener.h>

#include <iostream>
#include <memory>

int main()
{
  // The user handler. Called whenever a connection is received.
  auto closure = [](int socket, const Networking::NetAddress& requestor)
    {
      std::cerr << "Received connection from ("
      << requestor.getIPDotNotation() << ", "
      << requestor.getPortHostOrder() << ")\n";
    };

  using namespace Networking;
  using namespace Networking::Interfaces;
  using namespace Networking::TCP;
  TCPListener::Builder builder = TCPListener::Builder()
    .setPort(13001)
    .setUserHandler(closure);
  std::unique_ptr<IListener> listener
    = std::make_unique<TCPListener>(builder.build());
  std::unique_ptr<IDelegator> delegator = std::make_unique<DelegatorSTSP>();
  BlockingServer server = BlockingServer{std::move(delegator),
                                         std::move(listener)};
  server.start();
}

///////////////////////////////////////////////////////////////////////////////
