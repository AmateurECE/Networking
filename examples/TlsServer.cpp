///////////////////////////////////////////////////////////////////////////////
// NAME:            TlsServer.cpp
//
// AUTHOR:          Ethan D. Twardy <edtwardy@mtu.edu>
//
// DESCRIPTION:     Example of using the TLSListener to deploy a TLS server.
//
// CREATED:         04/04/2020
//
// LAST EDITED:     04/17/2020
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

  TLSListener<>::Builder builder = TLSListener<>::Builder()
    .setPort(13001)
    .setCertificateFile("../cert.pem")
    .setPrivateKeyFile("../April04TestKey.pem");
  std::unique_ptr<IListener> listener
    = std::make_unique<TLSListener<>>(builder.build());
  std::unique_ptr<IDelegator> delegator = std::make_unique<DelegatorSTSP>();
  BlockingServer server = BlockingServer(std::move(delegator),
                                         std::move(listener));
  server.start();
}

///////////////////////////////////////////////////////////////////////////////
