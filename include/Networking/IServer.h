///////////////////////////////////////////////////////////////////////////////
// NAME:            iServer.h
//
// AUTHOR:          Ethan D. Twardy <edtwardy@mtu.edu>
//
// DESCRIPTION:     Server interface
//
// CREATED:         03/27/2020
//
// LAST EDITED:     03/27/2020
////

#ifndef __ET_ISERVER__
#define __ET_ISERVER__

#include <namespaces/Networking.h>

class Networking::IServer
{
public:
  virtual void start() = 0;
  virtual void startAsync() = 0;
};

#endif // __ET_ISERVER__

///////////////////////////////////////////////////////////////////////////////
