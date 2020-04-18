///////////////////////////////////////////////////////////////////////////////
// NAME:            TlsClient.cpp
//
// AUTHOR:          Ethan D. Twardy <edtwardy@mtu.edu>
//
// DESCRIPTION:     An example using the TLSClient.
//
// CREATED:         04/09/2020
//
// LAST EDITED:     04/18/2020
////

#include <Networking/NetworkHost.h>
#include <Networking/TCP/TLSClient.h>

int main()
{
  auto handler = [](BIO*)
    {
      std::cerr << "Hello from the handler!\n";
    };

  using namespace Networking::TCP;

  // Connect to localhost:13001
  Networking::NetworkAddress host{"127.0.0.1", 13001};

  // Create the client.
  auto client = TLSClient<Networking::NetworkAddress>::Builder()
    .setHostAddress(host)
    .setUserHandler(handler)
    .setCustomCACertificatePath("../cert.pem")
    .build();

  // Connect to the server. When the connection has been established, the
  // client will call our handler and the message will be printed.
  client.connect();
}

///////////////////////////////////////////////////////////////////////////////
