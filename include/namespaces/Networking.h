///////////////////////////////////////////////////////////////////////////////
// NAME:            Networking.h
//
// AUTHOR:          Ethan D. Twardy <edtwardy@mtu.edu>
//
// DESCRIPTION:     The Networking namespace
//
// CREATED:         03/27/2020
//
// LAST EDITED:     04/02/2020
////

#ifndef __ET_NETWORKING__
#define __ET_NETWORKING__

namespace Networking
{
  namespace Interfaces
  {
    class IServer;    
    class IDelegator;
    class IListener;
    class IRequest;
  };

  // options for servers
  class BlockingServer;
  class NonBlockingServer;

  // options for delegators
  class DelegatorSTSP;  // Single-thread, Single-process
  class DelegatorMP;    // Multi-process
  class DelegatorMT;    // Multi-thread

  namespace TCP
  {
    class TCPListener;
    class TCPRequest;
    class TCPClient;
  };
};

#endif // __ET_NETWORKING__

///////////////////////////////////////////////////////////////////////////////
