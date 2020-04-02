///////////////////////////////////////////////////////////////////////////////
// NAME:            BlockingServer.cpp
//
// AUTHOR:          Ethan D. Twardy <edtwardy@mtu.edu>
//
// DESCRIPTION:     Implementation of the blocking server.
//
// CREATED:         04/02/2020
//
// LAST EDITED:     04/02/2020
////

#include <Networking/BlockingServer.h>

#include <Networking/Interfaces/IDelegator.h>
#include <Networking/Interfaces/IListener.h>
#include <Networking/Interfaces/IRequest.h>

Networking::BlockingServer
::BlockingServer(std::unique_ptr<Interfaces::IDelegator> delegator,
                 std::unique_ptr<Interfaces::IListener> listener)
  : m_delegator{std::move(delegator)}, m_listener{std::move(listener)}
{}

void Networking::BlockingServer::start()
{
  while(1)
    {
      m_delegator->dispatch(m_listener->listen());
    }
}

///////////////////////////////////////////////////////////////////////////////
