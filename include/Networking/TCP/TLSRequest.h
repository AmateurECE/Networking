///////////////////////////////////////////////////////////////////////////////
// NAME:            TLSRequest.h
//
// AUTHOR:          Ethan D. Twardy <edtwardy@mtu.edu>
//
// DESCRIPTION:     Request class using TLS over TCP.
//
// CREATED:         04/04/2020
//
// LAST EDITED:     04/04/2020
////

#ifndef __ET_TLSREQUEST__
#define __ET_TLSREQUEST__

#include <namespaces/Networking.h>
#include <Networking/Interfaces/IRequest.h>

class Networking::TCP::TLSRequest : public Networking::Interfaces::IRequest
{
public:
  TLSRequest() = default;
  virtual void handle() final override;

private:
};

#endif // __ET_TLSREQUEST__

///////////////////////////////////////////////////////////////////////////////
