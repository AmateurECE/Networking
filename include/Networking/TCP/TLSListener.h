///////////////////////////////////////////////////////////////////////////////
// NAME:            TLSListener.h
//
// AUTHOR:          Ethan D. Twardy <edtwardy@mtu.edu>
//
// DESCRIPTION:     Listener implementing TLS.
//
// CREATED:         04/04/2020
//
// LAST EDITED:     05/01/2020
////

#ifndef __ET_TLSLISTENER__
#define __ET_TLSLISTENER__

#include <namespaces/Networking.h>
#include <Networking/TCP/TCPListener.h>

#include <memory>

// Need forward declaration for compilation
typedef struct ssl_ctx_st SSL_CTX;
typedef struct ssl_st SSL;

template<class HostType>
class Networking::TCP::TLSListener : public Networking::Interfaces::IListener
{
public:
  enum HandshakeFailureAction
    {
      NOTHING,
      THROW
    };

  // TODO: Implement two-way authentication
  TLSListener(HostType acceptedClients, unsigned int theBacklogSize,
              bool reuseAddress, bool blocking, bool maskSigPipe,
              bool useTwoWayAuthentication, HandshakeFailureAction action,
              std::string certficateFile, std::string privateKeyFile,
              std::function<void(SSL*,const HostType&)> userHandler,
              std::function<void(const std::string&)> logStream);

  virtual std::unique_ptr<Interfaces::IRequest> listen() final override;

  class Builder;

private:
  struct TLSHandler;

  SSL_CTX* createContext() const;

  const std::string m_certificateFile;
  const std::string m_privateKeyFile;
  std::shared_ptr<SSL_CTX> m_sslContext;
  std::unique_ptr<struct TLSHandler> m_tlsHandler;
  TCPListener<HostType> m_listener;
  const bool m_useTwoWayAuthentication;
  std::function<void(SSL*,const HostType&)> m_userHandler;
  std::function<void(const std::string&)> m_logStream;
};

template<class HostType>
struct Networking::TCP::TLSListener<HostType>::TLSHandler
{
  TLSHandler(std::shared_ptr<SSL_CTX> sslContext,
             std::function<void(SSL*,const HostType&)> userHandler,
             HandshakeFailureAction handshakeFailureAction,
             std::function<void(const std::string&)> logStream);
  void operator()(unsigned int, const HostType&);

private:
  std::shared_ptr<SSL> m_ssl;
  std::shared_ptr<SSL_CTX> m_sslContext;
  std::function<void(SSL*,const HostType&)> m_userHandler;
  HandshakeFailureAction m_handshakeFailureAction;
  std::function<void(const std::string&)> m_logStream;
};

template<class HostType>
class Networking::TCP::TLSListener<HostType>::Builder
{
public:
  Builder();
  Builder setListeningAddress(HostType);
  Builder setBacklogSize(unsigned int);
  Builder setReuseAddress(bool);
  Builder setBlocking(bool);
  Builder setMaskSigPipe(bool);
  Builder setTwoWayAuthentication(bool);
  Builder setHandshakeFailureAction(HandshakeFailureAction);
  Builder setCertificateFile(std::string);
  Builder setPrivateKeyFile(std::string);
  using UserHandler = std::function<void(SSL*,const HostType&)>;
  Builder setUserHandler(UserHandler userHandler);
  Builder setLogStream(std::function<void(const std::string&)> logStream);

  TLSListener build() const;

private:
  HostType listeningAddress;
  unsigned int backlogSize = 8;
  bool reuseAddress = true;
  bool blocking = true;
  bool maskSigPipe = true;
  bool twoWayAuthentication = false;
  HandshakeFailureAction failureAction = HandshakeFailureAction::NOTHING;
  std::string certificateFile = ""; // NO DEFAULT
  std::string privateKeyFile = ""; // NO DEFAULT
  UserHandler userHandler = [](SSL*,const HostType&){ return; };
  std::function<void(const std::string&)> logStream =
    [](const std::string& message)
  {
    std::cerr << message << '\n';
  };
};

#include <Networking/TCP/TLSListener.tcc>

#endif // __ET_TLSLISTENER__

///////////////////////////////////////////////////////////////////////////////
