///////////////////////////////////////////////////////////////////////////////
// NAME:            TCPRequest.cpp
//
// AUTHOR:          Ethan D. Twardy <edtwardy@mtu.edu>
//
// DESCRIPTION:     Implementation of the TCP request.
//
// CREATED:         04/02/2020
//
// LAST EDITED:     04/02/2020
////

#include <Networking/TCP/TCPRequest.h>

#include <unistd.h>

Networking::TCP::TCPRequest
::TCPRequest(int socket, struct sockaddr_in connectingAddress,
             std::function<void(unsigned int,const sockaddr_in&)>& userHandler,
             std::ostream& logStream)
  : m_socket{socket}, m_connectingAddress{connectingAddress},
    m_userHandler{userHandler}, m_logStream{logStream}
{}

Networking::TCP::TCPRequest::~TCPRequest()
{
  ::close(m_socket);
}

void Networking::TCP::TCPRequest::handle()
{
  m_userHandler(m_socket, m_connectingAddress);
}

///////////////////////////////////////////////////////////////////////////////
