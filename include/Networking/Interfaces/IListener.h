///////////////////////////////////////////////////////////////////////////////
// NAME:            IListener.h
//
// AUTHOR:          Ethan D. Twardy <edtwardy@mtu.edu>
//
// DESCRIPTION:     Listens for incoming requests and instantiates objects to
//                  handle them.
//
// CREATED:         04/02/2020
//
// LAST EDITED:     04/02/2020
////

#ifndef __ET_ILISTENER__
#define __ET_ILISTENER__

#include <namespaces/Networking.h>

#include <memory>

class Networking::Interfaces::IListener
{
public:
  virtual ~IListener() {}

  virtual std::unique_ptr<IRequest> listen() = 0;
};

#endif // __ET_ILISTENER__

///////////////////////////////////////////////////////////////////////////////
