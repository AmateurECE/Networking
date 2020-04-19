///////////////////////////////////////////////////////////////////////////////
// NAME:            TLSListener.tcc
//
// AUTHOR:          Ethan D. Twardy <edtwardy@mtu.edu>
//
// DESCRIPTION:     Implementation of the TLS listener.
//
// CREATED:         04/04/2020
//
// LAST EDITED:     04/19/2020
////

#include <Networking/Interfaces/IRequest.h>
#include <Networking/NetworkHost.h>
#include <Networking/TCP/TLSException.h>
#include <Networking/TCP/TLSListener.h>

#include <openssl/ssl.h>
#include <openssl/err.h>

std::string getSSLErrors()
{
  BIO* bio = BIO_new(BIO_s_mem());
  ERR_print_errors(bio);
  char* buf = nullptr;
  BIO_get_mem_data(bio, &buf);
  return std::string{buf};
}

template<class HostType>
Networking::TCP::TLSListener<HostType>
::TLSListener(HostType acceptedClients,
              unsigned int theBacklogSize, bool reuseAddress, bool blocking,
              bool useTwoWayAuthentication, HandshakeFailureAction action,
              std::string certificateFile, std::string privateKeyFile,
              std::function<void(SSL*,const HostType&)> userHandler,
              std::function<void(const std::string&)> logStream)
  : m_certificateFile{certificateFile}, m_privateKeyFile{privateKeyFile},
    m_sslContext{createContext(), [](SSL_CTX* context)
        {
          SSL_CTX_free(context);
        }},
    m_tlsHandler{std::make_unique<struct TLSHandler>
        (m_sslContext, userHandler, action, logStream)},
    m_listener{acceptedClients, theBacklogSize, reuseAddress, blocking,
        std::ref(*m_tlsHandler), logStream},
    m_useTwoWayAuthentication{useTwoWayAuthentication},
    m_userHandler{userHandler}, m_logStream{logStream}
{}

template<class HostType>
std::unique_ptr<Networking::Interfaces::IRequest>
Networking::TCP::TLSListener<HostType>::listen()
{
  return m_listener.listen();
}

template<class HostType>
SSL_CTX* Networking::TCP::TLSListener<HostType>::createContext() const
{
  SSL_CTX* context = nullptr;

  const SSL_METHOD* method = TLS_server_method();
  context = SSL_CTX_new(method);
  if (!context)
    {
      throw std::runtime_error{"Unable to create SSL context: "
          + getSSLErrors()};
    }

  SSL_CTX_set_ecdh_auto(context, 1);
  if (0 >= SSL_CTX_use_certificate_file(context, m_certificateFile.c_str(),
                                        SSL_FILETYPE_PEM))
    {
      throw std::runtime_error{"Unable to create SSL context: "
          + getSSLErrors()};
    }

  if (0 >= SSL_CTX_use_PrivateKey_file(context, m_privateKeyFile.c_str(),
                                       SSL_FILETYPE_PEM))
    {
      throw std::runtime_error{"Unable to create SSL context: "
          + getSSLErrors()};      
    }

  return context;
}

template<class HostType>
Networking::TCP::TLSListener<HostType>::TLSHandler
::TLSHandler(std::shared_ptr<SSL_CTX> sslContext,
             std::function<void(SSL*,const HostType&)> userHandler,
             HandshakeFailureAction handshakeFailureAction,
             std::function<void(const std::string&)> logStream)
  : m_ssl{nullptr}, m_sslContext{sslContext}, m_userHandler{userHandler},
    m_handshakeFailureAction{handshakeFailureAction}, m_logStream{logStream}
{}

template<class HostType>
void Networking::TCP::TLSListener<HostType>::TLSHandler
::operator()(unsigned int socket, const HostType& clientAddress)
{
  // Use the shared_ptr here because it allows for automatic destruction in
  // case the flow of normal logic is interrupted (e.g. by exception).
  // Unfortunately, however, we must pass the raw pointer to the user,
  // because the OpenSSL library functions require a raw pointer.
  m_ssl = std::shared_ptr<SSL>(SSL_new(m_sslContext.get()), [](SSL* ssl){
      SSL_shutdown(ssl);
      SSL_free(ssl);
    });
  SSL* sslRaw = m_ssl.get();

  SSL_set_fd(sslRaw, socket);
  if (0 >= SSL_accept(sslRaw))
    {
      if (m_handshakeFailureAction == HandshakeFailureAction::NOTHING)
        {
          return;
        }
      else
        {
          const std::string sslErrors = "Client "
            + clientAddress.string()
            + " failed TLS handshake; error trace:\n" + getSSLErrors()
            + "Severing connection.";
          throw TLSException(sslErrors, clientAddress);
        }
    }

  m_userHandler(sslRaw, clientAddress);
}

///////////////////////////////////////////////////////////////////////////////
// TLSListener::Builder
////

template<>
Networking::TCP::TLSListener<Networking::NetworkHost>::Builder
::Builder()
  : listeningAddress{"127.0.0.1", 443}
{}

template<>
Networking::TCP::TLSListener<Networking::NetworkAddress>::Builder
::Builder()
  : listeningAddress{INADDR_LOOPBACK, 443}
{}

template<class HostType>
typename Networking::TCP::TLSListener<HostType>::Builder
Networking::TCP::TLSListener<HostType>::Builder
::setListeningAddress(HostType theListeningAddress)
{ listeningAddress = theListeningAddress; return *this; }

template<class HostType>
typename Networking::TCP::TLSListener<HostType>::Builder
Networking::TCP::TLSListener<HostType>::Builder
::setBacklogSize(unsigned int theBacklogSize)
{ backlogSize = theBacklogSize; return *this; }

template<class HostType>
typename Networking::TCP::TLSListener<HostType>::Builder
Networking::TCP::TLSListener<HostType>::Builder
::setReuseAddress(bool isReuseAddress)
{ reuseAddress = isReuseAddress; return *this; }

template<class HostType>
typename Networking::TCP::TLSListener<HostType>::Builder
Networking::TCP::TLSListener<HostType>::Builder
::setBlocking(bool isBlocking)
{ blocking = isBlocking; return *this; }

template<class HostType>
typename Networking::TCP::TLSListener<HostType>::Builder
Networking::TCP::TLSListener<HostType>::Builder
::setTwoWayAuthentication(bool theTwoWayAuthentication)
{ twoWayAuthentication = theTwoWayAuthentication; return *this; }

template<class HostType>
typename Networking::TCP::TLSListener<HostType>::Builder
Networking::TCP::TLSListener<HostType>::Builder
::setHandshakeFailureAction(HandshakeFailureAction theFailureAction)
{ failureAction = theFailureAction; return *this; }

template<class HostType>
typename Networking::TCP::TLSListener<HostType>::Builder
Networking::TCP::TLSListener<HostType>::Builder
::setCertificateFile(std::string theCertificateFile)
{ certificateFile = theCertificateFile; return *this; }

template<class HostType>
typename Networking::TCP::TLSListener<HostType>::Builder
Networking::TCP::TLSListener<HostType>::Builder
::setPrivateKeyFile(std::string thePrivateKeyFile)
{ privateKeyFile = thePrivateKeyFile; return *this; }

template<class HostType>
typename Networking::TCP::TLSListener<HostType>::Builder
Networking::TCP::TLSListener<HostType>::Builder
::setUserHandler(UserHandler theUserHandler)
{ userHandler = theUserHandler; return *this; }

template<class HostType>
typename Networking::TCP::TLSListener<HostType>::Builder
Networking::TCP::TLSListener<HostType>::Builder
::setLogStream(std::function<void(const std::string&)> theLogStream)
{ logStream = theLogStream; return *this; }

template<class HostType>
typename Networking::TCP::TLSListener<HostType>
Networking::TCP::TLSListener<HostType>::Builder::build() const
{
  return TLSListener<HostType>{listeningAddress, backlogSize, reuseAddress,
      blocking, twoWayAuthentication, failureAction, certificateFile,
      privateKeyFile, userHandler, logStream};
}

///////////////////////////////////////////////////////////////////////////////
