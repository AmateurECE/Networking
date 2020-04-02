///////////////////////////////////////////////////////////////////////////////
// NAME:            IRequest.h
//
// AUTHOR:          Ethan D. Twardy <edtwardy@mtu.edu>
//
// DESCRIPTION:     This object contains all the state necessary to handle
//                  requests from clients.
//
// CREATED:         04/02/2020
//
// LAST EDITED:     04/02/2020
////

#ifndef __ET_IREQUEST__
#define __ET_IREQUEST__

#include <namespaces/Networking.h>

class Networking::Interfaces::IRequest
{
public:
  virtual ~IRequest() {}

  virtual void handle() = 0;
};

#endif // __ET_IREQUEST__

///////////////////////////////////////////////////////////////////////////////
