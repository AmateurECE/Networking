///////////////////////////////////////////////////////////////////////////////
// NAME:            Networking.h
//
// AUTHOR:          Ethan D. Twardy <edtwardy@mtu.edu>
//
// DESCRIPTION:     The Networking namespace
//
// CREATED:         03/27/2020
//
// LAST EDITED:     04/17/2020
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

  // utility class encapsulating useful logic for dealing with inet addresses.
  class NetworkHost;
  class NetworkAddress;
  class UnixHost;

  namespace TCP
  {
    template<class HostType = NetworkHost>
    class TCPListener;
    template<class HostType = NetworkHost>
    class TCPRequest;
    template<class HostType = NetworkHost>
    class TCPClient;

    template<class HostType = NetworkHost>
    class TLSListener;
    template<class HostType = NetworkHost>
    class TLSClient;
    template<class HostType = NetworkHost>
    class TLSException;
  };
};

#endif // __ET_NETWORKING__

///////////////////////////////////////////////////////////////////////////////
