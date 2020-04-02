///////////////////////////////////////////////////////////////////////////////
// NAME:            DelegatorSTSP.h
//
// AUTHOR:          Ethan D. Twardy <edtwardy@mtu.edu>
//
// DESCRIPTION:     This delegator dispatches the user handler in the current
//                  thread (and obviously the current process).
//
// CREATED:         04/02/2020
//
// LAST EDITED:     04/02/2020
////

#ifndef __ET_DELEGATORSTSP__
#define __ET_DELEGATORSTSP__

#include <namespaces/Networking.h>

#include <Networking/Interfaces/IDelegator.h>

class Networking::DelegatorSTSP : public Networking::Interfaces::IDelegator
{
public:
  DelegatorSTSP() = default;

  virtual void dispatch(std::unique_ptr<Interfaces::IRequest>) final override;
};

#endif // __ET_DELEGATORSTSP__

///////////////////////////////////////////////////////////////////////////////
