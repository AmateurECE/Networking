///////////////////////////////////////////////////////////////////////////////
// NAME:            Server.h
//
// AUTHOR:          Ethan D. Twardy <edtwardy@mtu.edu>
//
// DESCRIPTION:     Declaration for Server class, which implements IServer
//                  interface.
//
// CREATED:         03/27/2020
//
// LAST EDITED:     03/27/2020
////

#ifndef __ET_SERVER__
#define __ET_SERVER__

#include <namespaces/Networking.h>
#include <Networking/IServer.h>

class Networking::Server : public Networking::IServer
{
public:
  virtual void start() final override;
  virtual void startAsync() final override;
};

#endif // __ET_SERVER__

///////////////////////////////////////////////////////////////////////////////
