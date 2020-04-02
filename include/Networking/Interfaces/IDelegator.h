///////////////////////////////////////////////////////////////////////////////
// NAME:            IDelegator.h
//
// AUTHOR:          Ethan D. Twardy <edtwardy@mtu.edu>
//
// DESCRIPTION:     Selects an environment to dispatch handlers for requests.
//                  This could be separate threads or processes, or it could be
//                  the current thread. It is up to the implementor.
//
// CREATED:         04/02/2020
//
// LAST EDITED:     04/02/2020
////

#ifndef __ET_IDELEGATOR__
#define __ET_IDELEGATOR__

#include <namespaces/Networking.h>

class Networking::Interfaces::IDelegator
{
public:
  virtual ~IDelegator() {}

  virtual void dispatch(std::unique_ptr<IRequest>) = 0;
};

#endif // __ET_IDELEGATOR__

///////////////////////////////////////////////////////////////////////////////
