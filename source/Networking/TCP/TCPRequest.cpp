///////////////////////////////////////////////////////////////////////////////
// NAME:            TCPRequest.cpp
//
// AUTHOR:          Ethan D. Twardy <edtwardy@mtu.edu>
//
// DESCRIPTION:     Implementation of the TCP request.
//
// CREATED:         04/02/2020
//
// LAST EDITED:     04/03/2020
////

#include <Networking/TCP/TCPRequest.h>

#include <unistd.h>

Networking::TCP::TCPRequest
::TCPRequest(int socket, struct sockaddr_in connectingAddress,
             std::function<void(unsigned int,const sockaddr_in&)>& userHandler,
             std::ostream& logStream)
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
  m_userHandler(*m_socket, m_connectingAddress);
}

///////////////////////////////////////////////////////////////////////////////
