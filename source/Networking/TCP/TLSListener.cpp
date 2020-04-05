///////////////////////////////////////////////////////////////////////////////
// NAME:            TLSListener.cpp
//
// AUTHOR:          Ethan D. Twardy <edtwardy@mtu.edu>
//
// DESCRIPTION:     Implementation of the TLS listener.
//
// CREATED:         04/04/2020
//
// LAST EDITED:     04/05/2020
////

#include <Networking/TCP/TLSListener.h>
#include <Networking/TCP/TLSRequest.h>

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

bool Networking::TCP::TLSListener::hasInitializedOpenSSL = false;

Networking::TCP::TLSListener
::TLSListener(unsigned int theClientAddresses, unsigned short thePort,
              unsigned int theBacklogSize, bool reuseAddress, bool blocking,
              bool useTwoWayAuthentication, std::string certificateFile,
              std::string privateKeyFile,
              std::function<void(SSL*,const NetAddress&)> userHandler,
              std::function<void(const std::string&)> logStream)
  : m_certificateFile{certificateFile}, m_privateKeyFile{privateKeyFile},
    m_sslContext{createContext(), [](SSL_CTX* context)
        {
          SSL_CTX_free(context);
        }},
    m_tlsHandler{std::make_unique<struct TLSHandler>(m_sslContext,
                                                     userHandler)},
    m_listener{theClientAddresses, thePort, theBacklogSize, reuseAddress,
        blocking, std::ref(*m_tlsHandler), logStream},
    m_useTwoWayAuthentication{useTwoWayAuthentication},
    m_userHandler{userHandler}, m_logStream{logStream}
{}

std::unique_ptr<Networking::Interfaces::IRequest>
Networking::TCP::TLSListener::listen()
{
  return std::make_unique<TLSRequest>();
}

SSL_CTX* Networking::TCP::TLSListener::createContext() const
{
  // TODO: Add mutexes to make initialization safe
  if (!hasInitializedOpenSSL)
    {
      hasInitializedOpenSSL = true;
      SSL_load_error_strings();
      OpenSSL_add_ssl_algorithms();
      // TODO: Creative solution for cleanup_openssl()
    }

  SSL_CTX* context = nullptr;

  const SSL_METHOD* method = SSLv23_server_method();
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

Networking::TCP::TLSListener::TLSHandler
::TLSHandler(std::shared_ptr<SSL_CTX> sslContext,
             std::function<void(SSL*,const NetAddress&)> userHandler)
  : m_ssl{nullptr}, m_sslContext{sslContext}, m_userHandler{userHandler}
{}

void
Networking::TCP::TLSListener::TLSHandler
::operator()(unsigned int socket, const NetAddress& clientAddress)
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
      // TODO: Do we need to throw here?
      //   Maybe we should simply log an error and return instead?
      //   Maybe we should throw a custom exception?
      throw std::runtime_error{"Error in SSL session negotiation: "
          + getSSLErrors()};
    }

  // TODO: Implement two-way authentication
  m_userHandler(sslRaw, clientAddress);
}

///////////////////////////////////////////////////////////////////////////////
// TLSListener::Builder
////

Networking::TCP::TLSListener::Builder
Networking::TCP::TLSListener::Builder
::setClientAddress(unsigned int theClientAddress)
{ clientAddress = theClientAddress; return *this; }

Networking::TCP::TLSListener::Builder
Networking::TCP::TLSListener::Builder
::setPort(unsigned short thePort)
{ port = thePort; return *this; }

Networking::TCP::TLSListener::Builder
Networking::TCP::TLSListener::Builder
::setBacklogSize(unsigned int theBacklogSize)
{ backlogSize = theBacklogSize; return *this; }

Networking::TCP::TLSListener::Builder
Networking::TCP::TLSListener::Builder
::setReuseAddress(bool isReuseAddress)
{ reuseAddress = isReuseAddress; return *this; }

Networking::TCP::TLSListener::Builder
Networking::TCP::TLSListener::Builder
::setBlocking(bool isBlocking)
{ blocking = isBlocking; return *this; }

Networking::TCP::TLSListener::Builder
Networking::TCP::TLSListener::Builder
::setTwoWayAuthentication(bool theTwoWayAuthentication)
{ twoWayAuthentication = theTwoWayAuthentication; return *this; }

Networking::TCP::TLSListener::Builder
Networking::TCP::TLSListener::Builder
::setCertificateFile(std::string theCertificateFile)
{ certificateFile = theCertificateFile; return *this; }

Networking::TCP::TLSListener::Builder
Networking::TCP::TLSListener::Builder
::setPrivateKeyFile(std::string thePrivateKeyFile)
{ privateKeyFile = thePrivateKeyFile; return *this; }

Networking::TCP::TLSListener::Builder
Networking::TCP::TLSListener::Builder
::setUserHandler(UserHandler theUserHandler)
{ userHandler = theUserHandler; return *this; }

Networking::TCP::TLSListener::Builder
Networking::TCP::TLSListener::Builder
::setLogStream(std::function<void(const std::string&)> theLogStream)
{ logStream = theLogStream; return *this; }

Networking::TCP::TLSListener
Networking::TCP::TLSListener::Builder::build() const
{
  return TLSListener{clientAddress, port, backlogSize, reuseAddress, blocking,
      twoWayAuthentication, certificateFile, privateKeyFile, userHandler,
      logStream};
}

///////////////////////////////////////////////////////////////////////////////
