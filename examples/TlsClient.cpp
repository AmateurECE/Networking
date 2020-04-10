///////////////////////////////////////////////////////////////////////////////
// NAME:            TlsClient.cpp
//
// AUTHOR:          Ethan D. Twardy <edtwardy@mtu.edu>
//
// DESCRIPTION:     An example using the TLSClient.
//
// CREATED:         04/09/2020
//
// LAST EDITED:     04/10/2020
////

#include <Networking/NetAddress.h>
#include <Networking/TCP/TLSClient.h>

int main()
{
  auto handler = [](BIO*)
    {
      std::cerr << "Hello from the handler!\n";
    };

  using namespace Networking::TCP;

  // Connect to localhost:13001
  Networking::NetAddress host{INADDR_LOOPBACK, 13001};

  // Create the client.
  TLSClient client = TLSClient::Builder()
    .setHostAddress(host)
    .setUserHandler(handler)
    .setCustomCACertificatePath("../cert.pem")
    .build();

  // Connect to the server. When the connection has been established, the
  // client will call our handler and the message will be printed.
  client.connect();
}

///////////////////////////////////////////////////////////////////////////////
