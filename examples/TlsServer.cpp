///////////////////////////////////////////////////////////////////////////////
// NAME:            TlsServer.cpp
//
// AUTHOR:          Ethan D. Twardy <edtwardy@mtu.edu>
//
// DESCRIPTION:     Example of using the TLSListener to deploy a TLS server.
//
// CREATED:         04/04/2020
//
// LAST EDITED:     04/19/2020
////

#include <namespaces/Networking.h>
#include <Networking/BlockingServer.h>
#include <Networking/DelegatorSTSP.h>
#include <Networking/TCP/TLSListener.h>

#include <openssl/ssl.h>

int main()
{
  using namespace Networking;
  using namespace Networking::Interfaces;
  using namespace Networking::TCP;

  auto closure = [](SSL*, NetworkAddress client)
    {
      std::cerr << "Received TLS connection from " << client.string() << "\n";
    };

  auto builder = TLSListener<NetworkAddress>::Builder()
    .setListeningAddress(NetworkAddress{"127.0.0.1", 13001})
    .setCertificateFile("../cert.pem")
    .setPrivateKeyFile("../April04TestKey.pem")
    .setUserHandler(closure);
  std::unique_ptr<IListener> listener
    = std::make_unique<TLSListener<NetworkAddress>>(builder.build());
  std::unique_ptr<IDelegator> delegator = std::make_unique<DelegatorSTSP>();
  BlockingServer server = BlockingServer(std::move(delegator),
                                         std::move(listener));
  server.start();
}

///////////////////////////////////////////////////////////////////////////////
