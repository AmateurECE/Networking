///////////////////////////////////////////////////////////////////////////////
// NAME:            TCPRequest.cpp
//
// AUTHOR:          Ethan D. Twardy <edtwardy@mtu.edu>
//
// DESCRIPTION:     Implementation of the TCP request.
//
// CREATED:         04/02/2020
//
// LAST EDITED:     04/04/2020
////

#include <Networking/TCP/TCPRequest.h>

#include <unistd.h>

Networking::TCP::TCPRequest
::TCPRequest(int socket, NetAddress connectingAddress,
             std::function<void(unsigned int,const NetAddress&)>& userHandler,
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

void Networking::TCP::TCPRequest::handle()
{
  m_logStream("Handling connection from ("
              + m_connectingAddress.getIPDotNotation() + ", "
              + std::to_string(m_connectingAddress.getPortHostOrder()) + ")");
  m_userHandler(*m_socket, m_connectingAddress);
}

///////////////////////////////////////////////////////////////////////////////
