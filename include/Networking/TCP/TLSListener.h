///////////////////////////////////////////////////////////////////////////////
// NAME:            TLSListener.h
//
// AUTHOR:          Ethan D. Twardy <edtwardy@mtu.edu>
//
// DESCRIPTION:     Listener implementing TLS.
//
// CREATED:         04/04/2020
//
// LAST EDITED:     04/05/2020
////

#ifndef __ET_TLSLISTENER__
#define __ET_TLSLISTENER__

#include <namespaces/Networking.h>
#include <Networking/TCP/TCPListener.h>

#include <memory>

// Need forward declaration for compilation
typedef struct ssl_ctx_st SSL_CTX;
typedef struct ssl_st SSL;

class Networking::TCP::TLSListener : public Networking::Interfaces::IListener
{
public:
  enum HandshakeFailureAction
    {
      NOTHING,
      THROW
    };

  TLSListener(unsigned int theClientAddresses, unsigned short thePort,
              unsigned int theBacklogSize, bool reuseAddress, bool blocking,
              bool useTwoWayAuthentication, HandshakeFailureAction action,
              std::string certficateFile, std::string privateKeyFile,
              std::function<void(SSL*,const NetAddress&)> userHandler,
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
  TCPListener m_listener;
  const bool m_useTwoWayAuthentication;
  std::function<void(SSL*,const NetAddress&)> m_userHandler;
  std::function<void(const std::string&)> m_logStream;

  static bool hasInitializedOpenSSL;
};

struct Networking::TCP::TLSListener::TLSHandler
{
  TLSHandler(std::shared_ptr<SSL_CTX> sslContext,
             std::function<void(SSL*,const NetAddress&)> userHandler,
             HandshakeFailureAction handshakeFailureAction,
             std::function<void(const std::string&)> logStream);
  void operator()(unsigned int, const NetAddress&);

private:
  std::shared_ptr<SSL> m_ssl;
  std::shared_ptr<SSL_CTX> m_sslContext;
  std::function<void(SSL*,const NetAddress&)> m_userHandler;
  HandshakeFailureAction m_handshakeFailureAction;
  std::function<void(const std::string&)> m_logStream;
};

class Networking::TCP::TLSListener::Builder
{
public:
  Builder() = default;
  Builder setClientAddress(unsigned int);
  Builder setPort(unsigned short);
  Builder setBacklogSize(unsigned int);
  Builder setReuseAddress(bool);
  Builder setBlocking(bool);
  Builder setTwoWayAuthentication(bool);
  Builder setHandshakeFailureAction(HandshakeFailureAction);
  Builder setCertificateFile(std::string);
  Builder setPrivateKeyFile(std::string);
  using UserHandler = std::function<void(SSL*,const NetAddress&)>;
  Builder setUserHandler(UserHandler userHandler);
  Builder setLogStream(std::function<void(const std::string&)> logStream);

  TLSListener build() const;

private:
  unsigned int clientAddress = INADDR_ANY;
  unsigned short port = 0;
  unsigned int backlogSize = 8;
  bool reuseAddress = true;
  bool blocking = true;
  bool twoWayAuthentication = false;
  HandshakeFailureAction failureAction = HandshakeFailureAction::NOTHING;
  std::string certificateFile = ""; // NO DEFAULT
  std::string privateKeyFile = ""; // NO DEFAULT
  UserHandler userHandler = [](SSL*,const NetAddress&){ return; };
  std::function<void(const std::string&)> logStream =
    [](const std::string& message)
  {
    std::cerr << message << '\n';
  };
};

#endif // __ET_TLSLISTENER__

///////////////////////////////////////////////////////////////////////////////
