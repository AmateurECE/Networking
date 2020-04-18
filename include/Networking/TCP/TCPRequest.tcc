///////////////////////////////////////////////////////////////////////////////
// NAME:            TCPRequest.cpp
//
// AUTHOR:          Ethan D. Twardy <edtwardy@mtu.edu>
//
// DESCRIPTION:     Implementation of the TCP request.
//
// CREATED:         04/02/2020
//
// LAST EDITED:     04/17/2020
////

#include <Networking/TCP/TCPRequest.h>

#include <unistd.h>

template<class HostType>
Networking::TCP::TCPRequest<HostType>
::TCPRequest(int socket, HostType connectingAddress,
             std::function<void(unsigned int,const HostType&)>& userHandler,
             std::function<void(const std::string&)> logStream)
  : m_socket{0}, m_connectingAddress{connectingAddress},
    m_userHandler{userHandler}, m_logStream{logStream}
{
  m_socket = std::shared_ptr<int>{new int, [](int* pInt){
      ::close(*pInt);
      delete pInt;
    }};
  *m_socket = socket;
}

template<>
void Networking::TCP::TCPRequest<Networking::NetworkHost>::handle()
{
  // TODO: Add specialization for UnixHost
  m_logStream("Handling connection from ("
              + m_connectingAddress.getIPDotNotation() + ", "
              + std::to_string(m_connectingAddress.getPortHostOrder()) + ")");
  m_userHandler(*m_socket, m_connectingAddress);
}

///////////////////////////////////////////////////////////////////////////////
