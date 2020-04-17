///////////////////////////////////////////////////////////////////////////////
// NAME:            TLSException.h
//
// AUTHOR:          Ethan D. Twardy <edtwardy@mtu.edu>
//
// DESCRIPTION:     This exception is thrown when an error occurs during TLS
//                  handshake or certificate verification. The user has the
//                  option either of configuring the TLSListener to generate
//                  this exception when a TLS Handshake failure occurs (e.g. so
//                  that the user can add the IP to a ban list, etc.) or of
//                  simply creating a log message and returning. In the
//                  TLSClient, this exception is thrown when a connection
//                  cannot be made because the server has failed the handshake
//                  or certificate verification.
//
// CREATED:         04/05/2020
//
// LAST EDITED:     04/17/2020
////

#ifndef __ET_TLSEXCEPTION__
#define __ET_TLSEXCEPTION__

#include <namespaces/Networking.h>
#include <Networking/NetworkHost.h>

#include <exception>
#include <string>

class Networking::TCP::TLSException
  : public std::exception
{
public:
  TLSException(std::string what, NetworkHost client);
  virtual const char* what() const noexcept override;
  const NetworkHost& getClientAddress() const;

private:
  std::string m_what;
  NetworkHost m_client;
};

#endif // __ET_TLSEXCEPTION__

///////////////////////////////////////////////////////////////////////////////
