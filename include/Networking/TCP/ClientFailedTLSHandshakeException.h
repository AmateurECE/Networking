///////////////////////////////////////////////////////////////////////////////
// NAME:            ClientFailedTLSHandshakeException.h
//
// AUTHOR:          Ethan D. Twardy <edtwardy@mtu.edu>
//
// DESCRIPTION:     This exception occurs when the client fails TLS handshake.
//                  The user has the option either of configuring the server to
//                  generate this exception when a TLS Handshake failure
//                  occurs (e.g. so that the user can add the IP to a ban list,
//                  etc.) or of simply creating a log message and returning.
//
// CREATED:         04/05/2020
//
// LAST EDITED:     04/05/2020
////

#ifndef __ET_CLIENTFAILEDTLSHANDSHAKEEXCEPTION__
#define __ET_CLIENTFAILEDTLSHANDSHAKEEXCEPTION__

#include <namespaces/Networking.h>
#include <Networking/NetAddress.h>

#include <exception>
#include <string>

class Networking::TCP::ClientFailedTLSHandshakeException
  : public std::exception
{
public:
  ClientFailedTLSHandshakeException(std::string what, NetAddress client);
  virtual const char* what() const noexcept override;
  const NetAddress& getClientAddress() const;

private:
  std::string m_what;
  NetAddress m_client;
};

#endif // __ET_CLIENTFAILEDTLSHANDSHAKEEXCEPTION__

///////////////////////////////////////////////////////////////////////////////
