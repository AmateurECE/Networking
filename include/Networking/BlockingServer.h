///////////////////////////////////////////////////////////////////////////////
// NAME:            BlockingServer.h
//
// AUTHOR:          Ethan D. Twardy <edtwardy@mtu.edu>
//
// DESCRIPTION:     Implementation of a server that blocks the current thread
//                  to receive and handle data from the network.
//
// CREATED:         04/02/2020
//
// LAST EDITED:     04/02/2020
////

#ifndef __ET_BLOCKINGSERVER__
#define __ET_BLOCKINGSERVER__

#include <namespaces/Networking.h>

#include <Networking/Interfaces/IServer.h>

#include <memory>

class Networking::BlockingServer : public Networking::Interfaces::IServer
{
public:
  BlockingServer(std::unique_ptr<Interfaces::IDelegator>,
                 std::unique_ptr<Interfaces::IListener>);

  virtual void start() final override;

private:
  std::unique_ptr<Interfaces::IDelegator> m_delegator;
  std::unique_ptr<Interfaces::IListener> m_listener;
};

#endif // __ET_BLOCKINGSERVER__

///////////////////////////////////////////////////////////////////////////////
