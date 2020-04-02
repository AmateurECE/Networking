///////////////////////////////////////////////////////////////////////////////
// NAME:            DelegatorSTSP.cpp
//
// AUTHOR:          Ethan D. Twardy <edtwardy@mtu.edu>
//
// DESCRIPTION:     Implementation of a Single-thread/Single-process Delegator
//
// CREATED:         04/02/2020
//
// LAST EDITED:     04/02/2020
////

#include <Networking/DelegatorSTSP.h>

#include <Networking/Interfaces/IRequest.h>

void
Networking::DelegatorSTSP
::dispatch(std::unique_ptr<Interfaces::IRequest> request)
{
  request->handle();
}

///////////////////////////////////////////////////////////////////////////////
